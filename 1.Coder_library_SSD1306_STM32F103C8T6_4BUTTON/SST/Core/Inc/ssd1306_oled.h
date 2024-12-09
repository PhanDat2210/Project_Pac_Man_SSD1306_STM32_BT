#ifndef INC_SSD1306_OLED_H_
#define INC_SSD1306_OLED_H_

#include "fonts.h"

#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR	0x78
//#define SSD1306_I2C_ADDR	0x7A
#endif

#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH		128
#endif

#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT   	64
#endif

typedef enum {
	BLACK = 0x00,
	WHITE = 0x01,
	PATTERN_RED   = 0xAA,
	PATTERN_GREEN = 0X55,
	PATTERN_BLUE = 0X33,
	PATTERN_YELLOW =  0xFF,
	YELLOW = 0xFFFF,
	 BLUE = 0x001F

} SSD1306_COLOR_t;


#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E
#define SSD1306_ACTIVATE_SCROLL                      0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3

#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7


uint8_t SSD1306_Init(void);
void SSD1306_UpdateScreen(void);
void SSD1306_ToggleInvert(void);
void SSD1306_Fill(SSD1306_COLOR_t Color);
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
void SSD1306_GotoXY(uint16_t x, uint16_t y);
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);
char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);
void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c);
void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);
void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);
void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);
void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);
void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);

#ifndef ssd1306_I2C_TIMEOUT
#define ssd1306_I2C_TIMEOUT		20000
#endif

void SSD1306_I2C_Init(void);
void SSD1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data);
void SSD1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);
void SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);
void SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row);
void SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row);
void SSD1306_Scrolldiagright(uint8_t start_row, uint8_t end_row);
void SSD1306_Scrolldiagleft(uint8_t start_row, uint8_t end_row);
void SSD1306_Stopscroll(void);
void SSD1306_InvertDisplay(int i);
void SSD1306_Clear(void);
void SSD1306_FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color);
void updateGameState(uint8_t start, uint8_t pause, uint8_t jump, uint8_t over);
void SSD1306_FillCircle(int x, int y, int r, uint8_t color);
void DrawPacMan(void);
void HandleButtonPress(void);

void DrawRoundedRectangle(int x, int y, int width, int height, int radius, SSD1306_COLOR_t color);
void SSD1306_DrawCirclePart(int x0, int y0, int radius, int start_angle, int end_angle, SSD1306_COLOR_t color) ;
void DrawMap(void);
void draw_map1(void);
void draw_pacman(int x, int y);


#endif
