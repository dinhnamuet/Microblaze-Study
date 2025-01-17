#ifndef __SPI_H__
#define __SPI_H__

#include "xil_types.h"
#include "def_config.h"
#include "list.h"

#define SLAVE(x)						BIT(x)

/* software reset register */
#define SPI_SOFTWARE_RESET_KEY			0x0000000AUL
/* control register */
#define SPI_CR_LSB_FIRST				BIT(9)
#define SPI_CR_MASTER_TRANS_INHIBIT		BIT(8)
#define SPI_CR_MANUAL_SS				BIT(7)
#define SPI_CR_RX_FIFO_RST				BIT(6)
#define SPI_CR_TX_FIFO_RST				BIT(5)
#define SPI_CR_CPHA						BIT(4)
#define SPI_CR_CPOL						BIT(3)
#define SPI_CR_MASTER_MODE				BIT(2)
#define SPI_CR_SPI_EN					BIT(1)
#define SPI_CR_LOOP						BIT(0)
/* status register */
#define SPI_SR_CMD_ERROR				BIT(10)
#define SPI_SR_LB_ERROR					BIT(9)
#define SPI_SR_MSB_ERROR				BIT(8)
#define SPI_SR_SLV_MODE_ERROR			BIT(7)
#define SPI_SR_CPOL_CPHA_ERROR			BIT(6)
#define SPI_SR_SLV_MODE_SELECT			BIT(5)
#define SPI_SR_MODF						BIT(4)
#define SPI_SR_TX_FULL					BIT(3)
#define SPI_SR_TX_EMPTY					BIT(2)
#define SPI_SR_RX_FULL					BIT(1)
#define SPI_SR_RX_EMPTY					BIT(0)
/* SPI MODE */
#define SPI_MODE_0						0x00						/* CPOL = 0, CPHA = 0 */
#define SPI_MODE_1						SPI_CR_CPHA					/* CPOL = 0, CPHA = 1 */
#define SPI_MODE_2						SPI_CR_CPOL					/* CPOL = 1, CPHA = 0 */
#define SPI_MODE_3						SPI_CR_CPOL | SPI_CR_CPHA	/* CPOL = 1, CPHA = 1 */

struct __attribute__((aligned(32))) spi_regs {
	volatile u8 pad_0x00[0x40 - 0x00];
	volatile u32 reg_sw_rst;
	volatile u8 pad_0x40[0x60 - 0x44];
	volatile u32 reg_control;
	volatile u32 reg_status;
	volatile u32 reg_tx_data;
	volatile u32 reg_rx_data;
	volatile u32 reg_chip_select;
	volatile u32 reg_tx_fifo_occupancy;
	volatile u32 reg_rx_fifo_occupancy;
};

struct spidev {
	struct spi_regs *reg;
	u32 mode;
	u8 is_lsb_first;
};

struct spi_transfer {
	u8 *tx;
	u8 *rx;
	size_t size;
	u32 cs;
	unsigned cs_change;
	struct list_head list;
};

struct spi_message {
	struct list_head transfer_list;
};

#define spi_reset(spi)	\
		(spi)->reg->reg_sw_rst = SPI_SOFTWARE_RESET_KEY

static __inline void spi_remap(struct spi_regs **reg, u32 spi_base_addr)
{
	*reg = (struct spi_regs *)spi_base_addr;
}

#define spi_cs_low(spi, cs)		\
		(spi)->reg->reg_chip_select &= ~SLAVE(cs);

#define spi_cs_high(spi, cs)	\
		(spi)->reg->reg_chip_select |= SLAVE(cs);

void spi_setup(struct spidev *dev);
int spi_sync(struct spidev *dev, struct spi_message *msg);
static __inline void spi_message_init(struct spi_message *msg)
{
	INIT_LIST_HEAD(&msg->transfer_list);
}
static __inline void spi_message_add_tail(struct spi_message *msg, struct spi_transfer *xfer)
{
	INIT_LIST_HEAD(&xfer->list);
	list_add_tail(&xfer->list, &msg->transfer_list);
}
void spi_master_xfer(struct spidev *dev, u8 *tx, u8 *rx, size_t size);
int spi_write(struct spidev *dev, u32 cs, const u8 *buf, size_t size);
int spi_read(struct spidev *dev, u32 cs, u8 *buf, size_t size);

#endif
