#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include "uart.h"
#include "def_config.h"
#include "mb_interface.h"
#include "list.h"

#define IRQ_2MASK(x)		BIT(x)
#define NR_IRQ              32

/* Master Enable Register */
#define IRQ_MER_HIE         BIT(1)
#define IRQ_MER_ME          BIT(0)

typedef void (*irq_handler_t)(void *);

struct __attribute__((aligned(32))) irq_regs {
	volatile u32 reg_status;
	volatile u32 reg_pending;
	volatile u32 reg_enable;
	volatile u32 reg_ack;
	volatile u32 reg_set_irq_enable;
	volatile u32 reg_clr_irq_enable;
	volatile u32 reg_vector;
	volatile u32 reg_master_en;
	volatile u32 reg_irq_mode;
	volatile u32 reg_level;
	volatile u8 pad_0x24[0x100 - 0x28];
	volatile u32 reg_irq_vector_addr[NR_IRQ];
};

struct irq_struct {
	int irq;
	irq_handler_t handler;
	void *private_data;
	struct list_head list;
};

struct irq_controller {
    struct irq_regs *reg;
    void (*do_irq)(void);
    struct list_head irq_list;
};

static __inline void irq_remap(struct irq_regs **reg, u32 base_addr)
{
    *reg = (struct irq_regs *)base_addr;
};

#define local_irq_enable(ctrl, irq) \
	(ctrl)->reg->reg_set_irq_enable = IRQ_2MASK(irq)

#define local_irq_disable(ctrl, irq) \
	(ctrl)->reg->reg_clr_irq_enable = IRQ_2MASK(irq)

#define local_irq_clear(ctrl, irq) \
	(ctrl)->reg->reg_ack = IRQ_2MASK(irq)

#define irq_pending(ctrl, irq) \
	(ctrl)->reg->reg_pending & IRQ_2MASK(irq)

void global_irq_enable(struct irq_controller *ctrl);
void irq_handle_common(struct irq_controller *ctrl);
int request_irq(struct irq_controller *ctrl, int irq, irq_handler_t handler, void *args);
int free_irq(struct irq_controller *ctrl, int irq);

#endif
