#include "uart.h"
#include <stdlib.h>

static void serdev_irq_handler(void *args)
{
	struct serdev_device *serial = (struct serdev_device *)args;
	struct timer_list *tm = serial->tm;
	struct uart_regs *reg = serial->uart;
	if (reg->reg_status & UART_SR_RX_VALID) {
		del_timer(tm);
		serial->state = SERDEV_IN_RX;
		serial->rx_buf[serial->idx++] = reg->reg_rx_fifo;
		if (serial->idx == RX_SIZE) {
			serial->state = SERDEV_IDLE;
			serial->idx = 0;
		} else {
			tm->expires = get_tick(serial->mt) + 5;
			timer_add(serial->mt, tm);
		}
		local_irq_clear(serial->irq_ctrl, TIMER_IRQn);
	}
}
static void tm_handler(void *args)
{
	struct timer_list *tm = (struct timer_list *)args;
	struct serdev_device *dev = tm->private_data;
	if (dev->state != SERDEV_IDLE && dev->idx) {
		dev->state = SERDEV_IDLE;
		if (dev->receive_buf)
			dev->receive_buf(dev, dev->rx_buf, dev->idx);
		dev->idx = 0;
	}
}

int __serdev_device_sync(struct serdev_device *dev, struct serdev_xfer *xfer)
{
	size_t i;
	struct uart_regs *reg;
	if (!dev || !xfer)
		return -ENODEV;
	else if (!xfer->buf)
		return -EINVAL;
	reg = dev->uart;
	for (i = 0; i < xfer->size; i++) {
		if (xfer->is_read) {
			while (!(reg->reg_status & UART_SR_RX_VALID));
			xfer->buf[i] = reg->reg_rx_fifo;
		} else {
			while (reg->reg_status & UART_SR_TX_FULL);
			reg->reg_tx_fifo = xfer->buf[i];
		}
	}
	return i;
}

int serdev_device_open(struct serdev_device *dev)
{
	int ret;
	struct uart_regs *reg = dev->uart;
	dev->state = SERDEV_IDLE;
	dev->rx_buf = malloc(RX_SIZE);
	if (!dev->rx_buf) {
		ret = -ENOMEM;
		goto dev_err;
	}
	ret = request_irq(dev->irq_ctrl, UART_IRQn, serdev_irq_handler, dev);
	if (ret)
		goto irq_err;
	dev->tm = malloc(sizeof(struct timer_list));
	if (!dev->tm) {
		ret = -ENOMEM;
		goto tm_err;
	}
	timer_setup(dev->tm, tm_handler, dev);
	dev->idx = 0;
	reg->reg_control = UART_CR_INT_ENABLE;
	return 0;
tm_err:
	free_irq(dev->irq_ctrl, UART_IRQn);
irq_err:
	free(dev->rx_buf);
dev_err:
	return ret;
}
