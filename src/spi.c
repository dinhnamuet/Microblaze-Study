#include "spi.h"

#define DUMMY_BYTE		0xFFU

void spi_setup(struct spidev *dev)
{
	u32 reg_control = 0;
	struct spi_regs *reg = dev->reg;
	spi_reset(dev);
	reg_control |= (dev->is_lsb_first) ? (SPI_CR_LSB_FIRST) : 0;
	reg_control |= dev->mode | SPI_CR_MASTER_MODE | SPI_CR_MANUAL_SS | SPI_CR_SPI_EN;
	reg->reg_control = reg_control;
}

void spi_master_xfer(struct spidev *dev, u8 *tx, u8 *rx, size_t size)
{
	struct spi_regs *reg = dev->reg;
	for (int i = 0; i < size; i++) {
		while (reg->reg_status & SPI_SR_TX_FULL);
		reg->reg_tx_data = (tx) ? tx[i] : DUMMY_BYTE;
		while (!(reg->reg_status & SPI_SR_TX_FULL));
		while (reg->reg_status & SPI_SR_RX_EMPTY);
		if (rx)
			rx[i] = (u8)reg->reg_rx_data;
		else
			UNUSED(reg->reg_rx_data);
		while (!(reg->reg_status & SPI_SR_RX_EMPTY));
	}
}

int spi_sync(struct spidev *dev, struct spi_message *msg)
{
	struct spi_transfer *entry;
	if (!dev || !msg)
		return -ENODEV;
	else if (list_empty(&msg->transfer_list))
		return -EINVAL;
	list_for_each_entry(entry, &msg->transfer_list, list) {
		spi_cs_low(dev, entry->cs);
		spi_master_xfer(dev, entry->tx, entry->rx, entry->size);
		if (entry->cs_change)
			spi_cs_high(dev, entry->cs);
	}
	return EOK;
}

int spi_write(struct spidev *dev, u32 cs, const u8 *buf, size_t size)
{
	struct spi_message msg;
	struct spi_transfer xfer = {
			tx:(u8 *)buf,
			rx:NULL,
			size:size,
			cs:cs,
			cs_change:1
	};
	spi_message_init(&msg);
	spi_message_add_tail(&msg, &xfer);
	return spi_sync(dev, &msg);
}

int spi_read(struct spidev *dev, u32 cs, u8 *buf, size_t size)
{
	struct spi_message msg;
	struct spi_transfer xfer = {
			tx:NULL,
			rx:buf,
			size:size,
			cs:cs,
			cs_change:1
	};
	spi_message_init(&msg);
	spi_message_add_tail(&msg, &xfer);
	return spi_sync(dev, &msg);
}
