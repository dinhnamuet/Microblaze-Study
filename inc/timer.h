#ifndef __TIMER_H__
#define __TIMER_H__

#include "def_config.h"
#include "interrupt.h"
#include "list.h"

#define F_TIMER             100000000UL
#define TICK_PER_SEC        1000U

typedef void (*tm_handler_t)(void *);

/* Control/Status Register 0 Bit Definitions */
#define TIM_CSR_CASC        BIT(11) /* Enable cascade mode of timers */
#define TIM_CSR_ENALL       BIT(10) /* Enable All Timers */
#define TIM_CSR_PWMA0       BIT(9)  /* Enable Pulse Width Modulation for Timer0 */
#define TIM_CSR_T0INT       BIT(8)  /* Timer0 Interrupt */
#define TIM_CSR_ENT0        BIT(7)  /* Enable Timer0 */
#define TIM_CSR_ENIT0       BIT(6)  /* Enable Interrupt for Timer0 */
#define TIM_CSR_LOAD0       BIT(5)  /* Load Timer0 */
#define TIM_CSR_ARHT0       BIT(4)  /* Auto Reload/Hold Timer0 */
#define TIM_CSR_CAPT0       BIT(3)  /* Enable External Capture Trigger Timer0 */
#define TIM_CSR_GENT0       BIT(2)  /* Enable External Generate Signal Timer0 */
#define TIM_CSR_UDT0        BIT(1)  /* Up/Down Count Timer0 */
#define TIM_CSR_MDT0        BIT(0)  /* Timer0 Mode */
/* Control/Status Register 1 Bit Definitions */
#define TIM_CSR_T1INT       BIT(8)
#define TIM_CSR_ENT1        BIT(7)
#define TIM_CSR_ENIT1       BIT(6)
#define TIM_CSR_LOAD1       BIT(5)
#define TIM_CSR_ARHT1       BIT(4)
#define TIM_CSR_CAPT1       BIT(3)
#define TIM_CSR_GENT1       BIT(2)
#define TIM_CSR_UDT1        BIT(1)
#define TIM_CSR_MDT1        BIT(0)

struct __attribute__((aligned(32))) timer_regs {
    volatile u32 reg_csr0;
    volatile u32 reg_load0;
    volatile u32 reg_counter0;
    volatile u8 pad_0x0c[0x10-0x0C];
    volatile u32 reg_csr1;
    volatile u32 reg_load1;
    volatile u32 reg_counter1;
};

struct mb_timer {
    struct timer_regs *reg;
    struct irq_controller *irq_ctrl;
    u64 tick;
    struct list_head tm_list;
};

struct timer_list {
	struct mb_timer *mt;
    u64 expires;
    tm_handler_t cb;
    void *private_data;
    struct list_head list;
};

static __inline void mt_remap(struct timer_regs **reg, u32 addr)
{
    *reg = (struct timer_regs *)addr;
}

static __inline u64 get_tick(struct mb_timer *mt)
{
	return mt->tick;
}

static __inline u64 get_expires(struct timer_list *tm)
{
	return tm->expires;
}

static __inline void inc_tick(struct mb_timer *mt)
{
	mt->tick++;
}

int timer_init(struct mb_timer *mt);
void timer_setup(struct timer_list *tm, tm_handler_t cb, void *args);
void timer_add(struct mb_timer *mt, struct timer_list *tm);
void del_timer(struct timer_list *tm);
void mod_timer(struct timer_list *tm, u64 expires);

#endif
