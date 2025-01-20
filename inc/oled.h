#ifndef __OLED_H__
#define __OLED_H__

#include "def_config.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define FONT_X	6

#define OLED_SET_CONTRAST_CTRL				0x81
#define OLED_ENTIRE_DISPLAY_RAM				0xA4
#define OLED_ENTIRE_DISPLAY_IGNORE_RAM		0xA5
#define OLED_SET_NORMAL_DISPLAY				0xA6
#define OLED_SET_INVERSE_DISPLAY			0xA7
#define OLED_DISPLAY_OFF					0xAE
#define OLED_DISPLAY_ON						0xAF
#define OLED_SET_MEM_ADDR_MODE				0x20
#define OLED_SET_COLUMN_ADDR				0x21
#define OLED_SET_PAGE_ADDR					0x22
#define OLED_DEACTIVE_SCROLL				0x2E
#define OLED_SET_SEG0_COL0					0xA0
#define OLED_SET_SEG0_COL127				0xA1
#define OLED_SET_MUX_RATIO					0xA8
#define OLED_SET_COM_NORMAL_MODE			0xC0
#define OLED_SET_COM_REMAP_MODE				0xC8
#define OLED_SET_COM_PIN_HW_CFG				0xDA
#define OLED_SET_DISPLAY_OFFSET				0xD3
#define OLED_SET_DISPLAY_CLK_DIVIDE			0xD5
#define OLED_SET_PRECHARGE_PERIOD			0xD9
#define OLED_SET_VCOMH_DESELECT_LV			0xDB
#define OLED_SET_PAGE_START0				0xB0
#define OLED_HORIZONTAL_ADDRS_MODE			0x00
#define OLED_VERTICAL_ADDRS_MODE			0x01
#define OLED_PAGE_ADDRS_MODE				0x02

typedef enum {
	OLED_COMMAND,
	OLED_DATA
} oled_mode_t;

struct __attribute__((packed)) oled {
	struct spidev *spi;
	struct gpio *gpio;
	u32 vbat;
	u32 vdd;
	u32 rst;
	u32 dc;

	u8 width;
	u8 heigh;
	u8 current_x;
	u8 current_y;
	u8 max_x;
	u8 max_y;
};

void oled_init(struct oled *oled, u8 width, u8 height);
void oled_powerup(struct oled *oled);
void oled_powerdown(struct oled *oled);
void oled_reset(struct oled *oled);
void oled_write(struct oled *oled, const u8 data, oled_mode_t mode);
void oled_burst_write(struct oled *oled, const u8 *data, size_t size, oled_mode_t mode);
void oled_clear(struct oled *oled);
void oled_goto_xy(struct oled *oled, u8 x, u8 y);
void oled_send_char(struct oled *oled, const char c);
void oled_send_string(struct oled *oled, const char *str, size_t size);
void oled_next_line(struct oled *oled);

#endif
