#include "def_config.h"
#include "uart.h"
#include <stdlib.h>
#include <string.h>
#include "sleep.h"
#include "spi.h"
#define SPI_CS		0

struct irq_controller *ctrl;
struct spidev *spi;

static void serdev_rx(struct serdev_device *dev, const u8 *buf, size_t size)
{
	serdev_device_writebuf(dev, buf, size);
}
static void do_irq(void) __attribute__((interrupt_handler));

int main(int argc, char *argv[])
{
	struct gpio gpio;
	struct mb_timer *mt;
	struct serdev_device *dev;
	dev = (struct serdev_device *)calloc(1, sizeof(struct serdev_device));
	if (!dev)
		return -ENOMEM;
	ctrl = (struct irq_controller *)calloc(1, sizeof(struct irq_controller));
	if (!ctrl)
		return -ENOMEM;
	spi = (struct spidev *)calloc(1, sizeof(struct spidev));
	if (!spi)
		return -ENOMEM;
	mt = (struct mb_timer *)calloc(1, sizeof(struct mb_timer));
	if (!mt)
		return -ENOMEM;

	irq_remap(&ctrl->reg, XPAR_AXI_INTC_0_BASEADDR);
	serdev_device_remap(&dev->uart, XPAR_AXI_UARTLITE_0_BASEADDR);
	spi_remap(&spi->reg, XPAR_AXI_QUAD_SPI_0_BASEADDR);
	mt_remap(&mt->reg, XPAR_AXI_TIMER_0_BASEADDR);
	gpio_remap(&gpio, XPAR_AXI_GPIO_0_BASEADDR);

	dev->irq_ctrl = ctrl;
	dev->mt = mt;
	mt->irq_ctrl = ctrl;
	dev->receive_buf = serdev_rx;
	spi->mode = SPI_MODE_0;
	spi->is_lsb_first = 0;
	ctrl->do_irq = do_irq;
	spi_setup(spi);

	global_irq_enable(ctrl);
	timer_init(mt);
	serdev_device_open(dev);

	while (1) {
		__asm__ ("nop");
	}
	return EOK;
}

static void do_irq(void)
{
	irq_handle_common(ctrl);
}
