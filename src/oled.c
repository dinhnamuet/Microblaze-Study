#include "oled.h"
#include <stdlib.h>
#include "sleep.h"

const int max_X = OLED_WIDTH / FONT_X;
const int max_Y = OLED_HEIGHT / 8;
const int frame_size = FONT_X * max_X * max_Y;

int oled_init(struct oled *oled)
{
	struct gpio *gpio = oled->gpio;
	oled->frame_buffer = calloc(frame_size, sizeof(u8));
	if (!oled->frame_buffer)
		return -ENOMEM;
	gpio_direction_output(gpio, oled->vbat);
	gpio_direction_output(gpio, oled->vdd);
	gpio_direction_output(gpio, oled->rst);
	gpio_direction_output(gpio, oled->dc);
	gpio_set_value(gpio, oled->vbat, 0);
	gpio_set_value(gpio, oled->vdd, 0);
	gpio_set_value(gpio, oled->dc, 0);
	gpio_set_value(gpio, oled->rst, 0);
	return 0;
}

void oled_reset(struct oled *oled)
{
	gpio_set_value(oled->gpio, oled->rst, 0);
	usleep(10000);
	gpio_set_value(oled->gpio, oled->rst, 1);
	usleep(10000);
}

int oled_powerup(struct oled *oled)
{
	return 0;
}

int oled_powerdown(struct oled *oled)
{
	return 0;
}

void oled_write(struct oled *oled, const u8 data, oled_mode_t mode)
{
	gpio_set_value(oled->gpio, oled->dc, (u8)mode);
	spi_master_xfer(oled->spi, (u8 *)&data, NULL, 1);
}

void oled_burst_write(struct oled *oled, const u8 *data, size_t size, oled_mode_t mode)
{
	gpio_set_value(oled->gpio, oled->dc, (u8)mode);
	spi_master_xfer(oled->spi, (u8 *)data, NULL, size);
}

int oled_clear(struct oled *oled)
{
	return 0;
}

int oled_goto_xy(struct oled *oled, u8 x, u8 y)
{
	return 0;
}

int oled_send_char(struct oled *oled, const char c)
{
	return 0;
}

int oled_send_string(struct oled *oled, const char *str, size_t size)
{
	return 0;
}

int oled_next_line(struct oled *oled)
{
	return 0;
}

int oled_sync(struct oled *oled)
{
	return 0;
}
