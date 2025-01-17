#ifndef __UART_H__
#define __UART_H__

#include "def_config.h"
#include "interrupt.h"
#include "timer.h"

#define RX_SIZE					100
/* Control Register */
#define UART_CR_INT_ENABLE      BIT(4)
#define UART_CR_RX_FIFO_RST     BIT(1)
#define UART_CR_TX_FIFO_RST     BIT(0)
/* Status Register */
#define UART_SR_PARITY_ERR      BIT(7)
#define UART_SR_FRAME_ERR       BIT(6)
#define UART_SR_OVRUN_ERR       BIT(5)
#define UART_SR_INT_ENABLE      BIT(4)
#define UART_SR_TX_FULL         BIT(3)
#define UART_SR_TX_EMPTY        BIT(2)
#define UART_SR_RX_FULL         BIT(1)
#define UART_SR_RX_VALID        BIT(0)
/* Peripheral */
struct __attribute__((aligned(32))) uart_regs {
    volatile u32 reg_rx_fifo;
    volatile u32 reg_tx_fifo;
    volatile u32 reg_status;
    volatile u32 reg_control;
};

enum serdev_state {
	SERDEV_IDLE,
	SERDEV_IN_TX,
	SERDEV_IN_RX
};

struct serdev_device {
    struct uart_regs *uart;
    struct irq_controller *irq_ctrl;
    void (*receive_buf)(struct serdev_device *, const u8 *, size_t);
    struct mb_timer *mt;
    struct timer_list *tm;
    u8 *rx_buf;
    u8 idx;
    enum serdev_state state;
};

struct serdev_xfer {
    u8 *buf;
    size_t size;
    u8 is_read;
};

static __inline void serdev_device_remap(struct uart_regs **reg, u32 base_addr)
{
    *reg = (struct uart_regs *)base_addr;
};

int serdev_device_open(struct serdev_device *dev);

int __serdev_device_sync(struct serdev_device *dev, struct serdev_xfer *xfer);

static __inline int serdev_device_writebuf(struct serdev_device *dev, const u8 *buf, size_t size)
{
	struct serdev_xfer tx = {
			buf:(u8 *)buf,
			size:size,
			is_read:0
	};
	return __serdev_device_sync(dev, &tx);
}

static __inline int serdev_device_rdbuf(struct serdev_device *dev, u8 *buf, size_t size)
{
	struct serdev_xfer rx = {
			buf:buf,
			size:size,
			is_read:1
	};
	return __serdev_device_sync(dev, &rx);
}

#endif
