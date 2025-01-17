#ifndef __GPIO_H__
#define __GPIO_H__

#include "def_config.h"
#define PIN_2MASK(x)  BIT(x)

struct __attribute__((aligned(32))) gpio_regs {
    volatile u32 reg_level;
    volatile u32 reg_mode;
};

struct gpio {
    struct gpio_regs *reg;
    u32 reg_level;
    u32 reg_mode;
};

static __inline void gpio_remap(struct gpio *gpio, u32 addr)
{
    gpio->reg = (struct gpio_regs *)addr;
    gpio->reg_mode = 0;
    gpio->reg_level = 0;
    gpio->reg->reg_mode = 0;
    gpio->reg->reg_level = 0;
}

void gpio_direction_output(struct gpio *gpio, u32 pin);
void gpio_direction_input(struct gpio *gpio, u32 pin);
int gpio_set_value(struct gpio *gpio, u32 pin, u8 value);
u8 gpio_get_value(struct gpio *gpio, u32 pin);

#endif
