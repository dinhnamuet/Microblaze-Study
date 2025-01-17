#ifndef __DEF_CONF_H__
#define __DEF_CONF_H__

#include "stddef.h"
#include "xil_types.h"
#include "interrupt.h"
#include "timer.h"
#include "xparameters.h"
#include "spi.h"
#include "gpio.h"

#define container_of(ptr, type, member) ({          \
	const typeof(((type *)0)->member)*__mptr = (ptr);    \
		     (type *)((char *)__mptr - offsetof(type, member)); })

#define BIT(x)			(1UL << x)
#define UNUSED(x)		(void)x
/* Error */
#define EOK				0
#define EFAULT			1
#define ENODEV			2
#define ENOMEM			3
#define EEXIST			4
#define EIO				5
#define EINVAL			6
/* IRQ Number */
#define UART_IRQn		0
#define TIMER_IRQn      1

#endif
