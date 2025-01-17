#include "gpio.h"

static __inline u8 pin_is_input(struct gpio *gpio, u32 pin)
{
	return (gpio->reg->reg_mode >> pin) & 0x01;
}

void gpio_direction_output(struct gpio *gpio, u32 pin)
{
	gpio->reg_mode &= ~PIN_2MASK(pin);
	gpio->reg->reg_mode = gpio->reg_mode;
}

void gpio_direction_input(struct gpio *gpio, u32 pin)
{
	gpio->reg_mode |= PIN_2MASK(pin);
	gpio->reg->reg_mode = gpio->reg_mode;
}

int gpio_set_value(struct gpio *gpio, u32 pin, u8 value)
{
	if (pin_is_input(gpio, pin))
		return -EFAULT;
	gpio->reg_level &= ~PIN_2MASK(pin);
	gpio->reg_level |= (value << pin);
	gpio->reg->reg_level = gpio->reg_level;
	return 0;
}

u8 gpio_get_value(struct gpio *gpio, u32 pin)
{
	return (gpio->reg->reg_level >> pin) & 0x01;
}
