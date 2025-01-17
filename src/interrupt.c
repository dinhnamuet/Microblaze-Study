#include "interrupt.h"
#include <stdlib.h>

void global_irq_enable(struct irq_controller *ctrl)
{
	struct irq_regs *reg = ctrl->reg;
	for (int i = 0; i < NR_IRQ; i++)
		reg->reg_irq_vector_addr[i] = (UINTPTR)ctrl->do_irq;	/* Common ISR */
	INIT_LIST_HEAD(&ctrl->irq_list);
	/* Config */
	reg->reg_irq_mode				= 0x00000000;				/* Standard Mode */
	reg->reg_ack					= 0xFFFFFFFF;				/* Clear all IRQ Flags */
	reg->reg_level					= 0xFFFFFFFF;				/* Unmask all IRQ */
	reg->reg_master_en				= IRQ_MER_ME | IRQ_MER_HIE; /* Enable IRQ Controller */
	microblaze_enable_interrupts(); 							/* Enable CPU IRQ */
}

void irq_handle_common(struct irq_controller *ctrl)
{
	struct irq_struct *entry;
	list_for_each_entry(entry, &ctrl->irq_list, list) {
		if (irq_pending(ctrl, entry->irq)) {
			if (entry->handler)
				entry->handler(entry->private_data);
			local_irq_clear(ctrl, entry->irq);
			return;
		}
	}
}

int request_irq(struct irq_controller *ctrl, int irq, irq_handler_t handler, void *args)
{
	struct irq_struct *irq_request, *entry;
	irq_request = (struct irq_struct *)calloc(1, sizeof(struct irq_struct));
	if (!irq_request)
		return -ENOMEM;
	irq_request->irq = irq;
	irq_request->handler = handler;
	irq_request->private_data = args;
	INIT_LIST_HEAD(&irq_request->list);
	if (list_empty(&ctrl->irq_list)) {
		list_add_tail(&irq_request->list, &ctrl->irq_list);
	} else {
		list_for_each_entry(entry, &ctrl->irq_list, list) {
			if (entry->irq < irq_request->irq)
				continue;
			else if (entry->irq == irq_request->irq)
				goto irq_err;
			else
				break;
		}
		list_add(&irq_request->list, entry->list.prev);
	}
	local_irq_enable(ctrl, irq);
	return 0;
irq_err:
	free(irq_request);
	return -EEXIST;
}

int free_irq(struct irq_controller *ctrl, int irq)
{
	struct irq_struct *entry;
	list_for_each_entry(entry, &ctrl->irq_list, list) {
		if (entry->irq == irq) {
			local_irq_disable(ctrl, irq);
			list_del_entry(&entry->list);
			free(entry);
			return 0;
		}
	}
	return -EINVAL;
}
