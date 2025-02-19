#include "oled.h"
#include <stdlib.h>
#include "sleep.h"

const u8 oled_font[][FONT_X] = {
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space
		{0x00, 0x00, 0x2f, 0x00, 0x00, 0x00}, // !
		{0x00, 0x07, 0x00, 0x07, 0x00, 0x00}, // "
		{0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00}, // #
		{0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00}, // $
		{0x23, 0x13, 0x08, 0x64, 0x62, 0x00}, // %
		{0x36, 0x49, 0x55, 0x22, 0x50, 0x00}, // &
		{0x00, 0x05, 0x03, 0x00, 0x00, 0x00}, // '
		{0x00, 0x1c, 0x22, 0x41, 0x00, 0x00}, // (
		{0x00, 0x41, 0x22, 0x1c, 0x00, 0x00}, // )
		{0x14, 0x08, 0x3E, 0x08, 0x14, 0x00}, // *
		{0x08, 0x08, 0x3E, 0x08, 0x08, 0x00}, // +
		{0x00, 0x00, 0xA0, 0x60, 0x00, 0x00}, // ,
		{0x08, 0x08, 0x08, 0x08, 0x08, 0x00}, // -
		{0x00, 0x60, 0x60, 0x00, 0x00, 0x00}, // .
		{0x20, 0x10, 0x08, 0x04, 0x02, 0x00}, // /
		{0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00}, // 0
		{0x00, 0x42, 0x7F, 0x40, 0x00, 0x00}, // 1
		{0x42, 0x61, 0x51, 0x49, 0x46, 0x00}, // 2
		{0x21, 0x41, 0x45, 0x4B, 0x31, 0x00}, // 3
		{0x18, 0x14, 0x12, 0x7F, 0x10, 0x00}, // 4
		{0x27, 0x45, 0x45, 0x45, 0x39, 0x00}, // 5
		{0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00}, // 6
		{0x01, 0x71, 0x09, 0x05, 0x03, 0x00}, // 7
		{0x36, 0x49, 0x49, 0x49, 0x36, 0x00}, // 8
		{0x06, 0x49, 0x49, 0x29, 0x1E, 0x00}, // 9
		{0x00, 0x36, 0x36, 0x00, 0x00, 0x00}, // :
		{0x00, 0x56, 0x36, 0x00, 0x00, 0x00}, // ;
		{0x08, 0x14, 0x22, 0x41, 0x00, 0x00}, // <
		{0x14, 0x14, 0x14, 0x14, 0x14, 0x00}, // =
		{0x00, 0x41, 0x22, 0x14, 0x08, 0x00}, // >
		{0x02, 0x01, 0x51, 0x09, 0x06, 0x00}, // ?
		{0x32, 0x49, 0x59, 0x51, 0x3E, 0x00}, // @
		{0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00}, // A
		{0x7F, 0x49, 0x49, 0x49, 0x36, 0x00}, // B
		{0x3E, 0x41, 0x41, 0x41, 0x22, 0x00}, // C
		{0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00}, // D
		{0x7F, 0x49, 0x49, 0x49, 0x41, 0x00}, // E
		{0x7F, 0x09, 0x09, 0x09, 0x01, 0x00}, // F
		{0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00}, // G
		{0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00}, // H
		{0x00, 0x41, 0x7F, 0x41, 0x00, 0x00}, // I
		{0x20, 0x40, 0x41, 0x3F, 0x01, 0x00}, // J
		{0x7F, 0x08, 0x14, 0x22, 0x41, 0x00}, // K
		{0x7F, 0x40, 0x40, 0x40, 0x40, 0x00}, // L
		{0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00}, // M
		{0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00}, // N
		{0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00}, // O
		{0x7F, 0x09, 0x09, 0x09, 0x06, 0x00}, // P
		{0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00}, // Q
		{0x7F, 0x09, 0x19, 0x29, 0x46, 0x00}, // R
		{0x46, 0x49, 0x49, 0x49, 0x31, 0x00}, // S
		{0x01, 0x01, 0x7F, 0x01, 0x01, 0x00}, // T
		{0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00}, // U
		{0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00}, // V
		{0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00}, // W
		{0x63, 0x14, 0x08, 0x14, 0x63, 0x00}, // X
		{0x07, 0x08, 0x70, 0x08, 0x07, 0x00}, // Y
		{0x61, 0x51, 0x49, 0x45, 0x43, 0x00}, // Z
		{0x00, 0x7F, 0x41, 0x41, 0x00, 0x00}, // [
		{0x55, 0xAA, 0x55, 0xAA, 0x55, 0x00}, // Backslash (Checker pattern)
		{0x00, 0x41, 0x41, 0x7F, 0x00, 0x00}, // ]
		{0x04, 0x02, 0x01, 0x02, 0x04, 0x00}, // ^
		{0x40, 0x40, 0x40, 0x40, 0x40, 0x00}, // _
		{0x00, 0x03, 0x05, 0x00, 0x00, 0x00}, // `
		{0x20, 0x54, 0x54, 0x54, 0x78, 0x00}, // a
		{0x7F, 0x48, 0x44, 0x44, 0x38, 0x00}, // b
		{0x38, 0x44, 0x44, 0x44, 0x20, 0x00}, // c
		{0x38, 0x44, 0x44, 0x48, 0x7F, 0x00}, // d
		{0x38, 0x54, 0x54, 0x54, 0x18, 0x00}, // e
		{0x08, 0x7E, 0x09, 0x01, 0x02, 0x00}, // f
		{0x18, 0xA4, 0xA4, 0xA4, 0x7C, 0x00}, // g
		{0x7F, 0x08, 0x04, 0x04, 0x78, 0x00}, // h
		{0x00, 0x44, 0x7D, 0x40, 0x00, 0x00}, // i
		{0x40, 0x80, 0x84, 0x7D, 0x00, 0x00}, // j
		{0x7F, 0x10, 0x28, 0x44, 0x00, 0x00}, // k
		{0x00, 0x41, 0x7F, 0x40, 0x00, 0x00}, // l
		{0x7C, 0x04, 0x18, 0x04, 0x78, 0x00}, // m
		{0x7C, 0x08, 0x04, 0x04, 0x78, 0x00}, // n
		{0x38, 0x44, 0x44, 0x44, 0x38, 0x00}, // o
		{0xFC, 0x24, 0x24, 0x24, 0x18, 0x00}, // p
		{0x18, 0x24, 0x24, 0x18, 0xFC, 0x00}, // q
		{0x7C, 0x08, 0x04, 0x04, 0x08, 0x00}, // r
		{0x48, 0x54, 0x54, 0x54, 0x20, 0x00}, // s
		{0x04, 0x3F, 0x44, 0x40, 0x20, 0x00}, // t
		{0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00}, // u
		{0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00}, // v
		{0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00}, // w
		{0x44, 0x28, 0x10, 0x28, 0x44, 0x00}, // x
		{0x1C, 0xA0, 0xA0, 0xA0, 0x7C, 0x00}, // y
		{0x44, 0x64, 0x54, 0x4C, 0x44, 0x00}, // z
		{0x00, 0x10, 0x7C, 0x82, 0x00, 0x00}, // {
		{0x00, 0x00, 0xFF, 0x00, 0x00, 0x00}, // |
		{0x00, 0x82, 0x7C, 0x10, 0x00, 0x00}, // }
		{0x00, 0x06, 0x09, 0x09, 0x06, 0x00}	// ~ (Degrees)
};

void oled_init(struct oled *oled, u8 width, u8 height)
{
	struct gpio *gpio = oled->gpio;
	oled->max_x = width / FONT_X;
	oled->max_y = height / 8;
	gpio_direction_output(gpio, oled->vbat);
	gpio_direction_output(gpio, oled->vdd);
	gpio_direction_output(gpio, oled->rst);
	gpio_direction_output(gpio, oled->dc);
	gpio_set_value(gpio, oled->vbat, 0);
	gpio_set_value(gpio, oled->vdd, 0);
	gpio_set_value(gpio, oled->dc, 0);
	gpio_set_value(gpio, oled->rst, 0);
	oled->width = width;
	oled->heigh = height;
	oled->current_x = 0;
	oled->current_y = 0;
}

void oled_reset(struct oled *oled)
{
	gpio_set_value(oled->gpio, oled->rst, 0);
	usleep(10000);
	gpio_set_value(oled->gpio, oled->rst, 1);
	usleep(10000);
}

void oled_powerup(struct oled *oled)
{
	gpio_set_value(oled->gpio, oled->vdd, 1); /* Power up VDD */
	oled_reset(oled);
	usleep(100000);
	oled_write(oled, OLED_DISPLAY_OFF, OLED_COMMAND); /* Display Off */
	/* set Osc Frequency */
	oled_write(oled, OLED_SET_DISPLAY_CLK_DIVIDE, OLED_COMMAND); /* Set Display Clock Divide Ratio/Oscillator Frequency */
	oled_write(oled, 0x80, OLED_COMMAND);
	/* set MUX Ratio */
	oled_write(oled, OLED_SET_MUX_RATIO, OLED_COMMAND); /* Set MUX ratio to N+1 MUX */
	oled_write(oled, 0x1F, OLED_COMMAND); /* 32 MUX */
	/* set display offset */
	oled_write(oled, OLED_SET_DISPLAY_OFFSET, OLED_COMMAND); /* Set Display Offset */
	oled_write(oled, 0x00, OLED_COMMAND);
	/* set display start line */
	oled_write(oled, 0x40, OLED_COMMAND); /* Set RAM display start line */
	/* Enable charge pump regulator */
	oled_write(oled, 0x8D, OLED_COMMAND);
	oled_write(oled, 0x14, OLED_COMMAND);
	/* Set memory addressing mode */
	oled_write(oled, OLED_SET_MEM_ADDR_MODE, OLED_COMMAND); /* Set Memory Addressing Mode */
	oled_write(oled, OLED_HORIZONTAL_ADDRS_MODE, OLED_COMMAND); /* Horizontal Addressing Mode */
	/* Set segment remap with column address 0 mapped to segment 0 */
	oled_write(oled, OLED_SET_SEG0_COL0, OLED_COMMAND); /* column address 0 is mapped to SEG0 */
	oled_write(oled, OLED_SET_COM_NORMAL_MODE, OLED_COMMAND); /* normal mode Scan from COM0 to COM[N �1] */
	/* set COM Pin hardware configuration */
	oled_write(oled, OLED_SET_COM_PIN_HW_CFG, OLED_COMMAND); /* Set COM Pins Hardware Configuration */
	oled_write(oled, 0x02, OLED_COMMAND); /* Sequential COM pin configuration */
	/* set contrast control */
	oled_write(oled, OLED_SET_CONTRAST_CTRL, OLED_COMMAND);
	oled_write(oled, 0x7F, OLED_COMMAND);
	/* Set pre-charge period */
	oled_write(oled, OLED_SET_PRECHARGE_PERIOD, OLED_COMMAND); /* Set Pre-charge Period */
	oled_write(oled, 0xF1, OLED_COMMAND);
	/* Set Vcomh deselect level */
	oled_write(oled, OLED_SET_VCOMH_DESELECT_LV, OLED_COMMAND);
	oled_write(oled, 0x40, OLED_COMMAND); /* ~ 0.77 x VCC */
	/* set normal display */
	oled_write(oled, OLED_SET_NORMAL_DISPLAY, OLED_COMMAND);
	/* Set GDDRAM Page Start Address 0 */
	oled_write(oled, OLED_SET_PAGE_START0, OLED_COMMAND);
	/* deactive scroll */
	oled_write(oled, OLED_DEACTIVE_SCROLL, OLED_COMMAND);
	/* ... */
	oled_write(oled, 0x00, OLED_COMMAND); /* set low column address */
	oled_write(oled, 0x10, OLED_COMMAND); /* set high column address */
	oled_write(oled, OLED_ENTIRE_DISPLAY_RAM, OLED_COMMAND); /* Resume to RAM content display */
	gpio_set_value(oled->gpio, oled->vbat, 1);
	usleep(100000);
	oled_clear(oled);
	oled_goto_xy(oled, 0, 0);
	oled_write(oled, OLED_DISPLAY_ON, OLED_COMMAND); /* Display ON */
}

void oled_powerdown(struct oled *oled)
{
	oled_write(oled, OLED_DISPLAY_OFF, OLED_COMMAND); /* Display Off */
	gpio_set_value(oled->gpio, oled->vbat, 0);
	usleep(100000);
	gpio_set_value(oled->gpio, oled->vdd, 0);
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

void oled_clear(struct oled *oled)
{
	oled_goto_xy(oled, 0, 0);
	for (int i = 0; i < oled->width * (oled->heigh / 8); i++)
		oled_write(oled, 0, OLED_DATA);
}

void oled_goto_xy(struct oled *oled, u8 x, u8 y)
{
	oled_write(oled, OLED_SET_COLUMN_ADDR, OLED_COMMAND);
	oled_write(oled, x * FONT_X, OLED_COMMAND);
	oled_write(oled, oled->width - 1, OLED_COMMAND);
	oled_write(oled, OLED_SET_PAGE_ADDR, OLED_COMMAND);
	oled_write(oled, y, OLED_COMMAND);
	oled_write(oled, oled->max_y, OLED_COMMAND);
	oled->current_x = x;
	oled->current_y = y;
}

void oled_send_char(struct oled *oled, const char c)
{
	if (oled->current_x == oled->max_x - 1)
		oled_next_line(oled);
	oled_burst_write(oled, oled_font[c - 32], FONT_X, OLED_DATA);
	oled->current_x++;
}

void oled_send_string(struct oled *oled, const char *str, size_t size)
{
	for (int i = 0; i < size; i ++) {
		if (str[i])
			oled_send_char(oled, str[i]);
	}
}

void oled_next_line(struct oled *oled)
{
	oled->current_y = (oled->current_y == oled->max_y - 1) ? 0 : (oled->current_y + 1);
	oled_goto_xy(oled, 0, oled->current_y);
}
