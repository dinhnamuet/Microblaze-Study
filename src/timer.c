#include "timer.h"

static void timer_handler(void *args)
{
	struct timer_list *entry;
    struct mb_timer *mt = (struct mb_timer *)args;
    struct timer_regs *reg = mt->reg;
    inc_tick(mt);
    if (!list_empty(&mt->tm_list)) {
    	entry = list_entry(mt->tm_list.next, struct timer_list, list);
    	if (get_tick(mt) >= entry->expires) {
    		list_del_entry(&entry->list);
    		if (entry->cb)
    			entry->cb(entry);
    	}
    }
    if (reg->reg_csr0 & TIM_CSR_T0INT) /* Interrupt has occurred */
        reg->reg_csr0 |= TIM_CSR_T0INT; /* Clear T0INT (clear to ’0’) */
}

int timer_init(struct mb_timer *mt)
{
    struct timer_regs *reg = mt->reg;
    mt->tick = 0;
    INIT_LIST_HEAD(&mt->tm_list);
    reg->reg_load0  = F_TIMER/TICK_PER_SEC;
    reg->reg_csr0   |= TIM_CSR_LOAD0;
    reg->reg_csr0   &= ~TIM_CSR_LOAD0;
    reg->reg_csr0   = TIM_CSR_ENT0 | TIM_CSR_ENIT0 | TIM_CSR_ARHT0 | TIM_CSR_UDT0;
    return request_irq(mt->irq_ctrl, TIMER_IRQn, timer_handler, mt);
}

void timer_setup(struct timer_list *tm, tm_handler_t cb, void *args)
{
    tm->cb = cb;
    tm->private_data = args;
    INIT_LIST_HEAD(&tm->list);
}

void timer_add(struct mb_timer *mt, struct timer_list *tm)
{
    struct timer_list *entry;
    tm->mt = mt;
    if (list_empty(&mt->tm_list)) {
        list_add_tail(&tm->list, &mt->tm_list);
        return;
    } else {
        list_for_each_entry(entry, &mt->tm_list, list) {
            if (tm->expires >= entry->expires)
            	continue;
            else
            	break;
        }
        list_add(&tm->list, entry->list.prev);
    }
}

void del_timer(struct timer_list *tm)
{
    list_del_entry(&tm->list);
}

void mod_timer(struct timer_list *tm, u64 expires)
{
    tm->expires = expires;
    timer_add(tm->mt, tm);
}
