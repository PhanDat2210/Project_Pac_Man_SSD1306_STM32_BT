#ifndef INC_FONTS_H_
#define INC_FONTS_H_

#include "main.h"

typedef struct {
	uint8_t FontWidth;
	uint8_t FontHeight;
	const uint16_t *data;
} FontDef_t;

typedef struct {
	uint16_t Length;
	uint16_t Height;
} FONTS_SIZE_t;


typedef struct {
	uint8_t gameStarted;     // Trạng thái bắt đầu trò chơi (0: chưa bắt đầu, 1: đã bắt đầu)
	uint8_t gamePaused;      // Trạng thái tạm dừng trò chơi (0: không tạm dừng, 1: tạm dừng)
	uint8_t playerJumpind;   // Trạng thái nhân vật đang nhảy (0: không nhảy, 1: đang nhảy)
	uint8_t gameOver;        // Trạng thái game over (0: chưa kết thúc, 1: kết thúc)
} GAMESTATE_t;

extern FontDef_t Font_5x7;
extern FontDef_t Font_7x10;
extern FontDef_t Font_11x18;
extern FontDef_t Font_16x26;
extern GAMESTATE_t gameState;  // Khai báo bên ngoài để sử dụng trong các file khác

char* FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);


#endif
