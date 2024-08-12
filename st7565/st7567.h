/*
 * st7567.h
 *
 *  Created on: Jun 23, 2024
 *      Author: vlad
 */

#ifndef ST7567_H_
#define ST7567_H_

#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "spi.h"
#include "fontlibrary.h"

#define ST7567A_RST_Pin 21
#define ST7567A_CS_Pin 1
#define ST7567A_DC_Pin 22
#define ST7567_LED_Pin 20

#define BLACK 1
#define WHITE 0

#define LCDWIDTH 128
#define LCDHEIGHT 64
#define LCDNUMPAGES 8

#define CMD_DISPLAY_OFF 0xAE
#define CMD_DISPLAY_ON 0xAF

#define CMD_SET_DISP_START_LINE 0x40
#define CMD_SET_PAGE 0xB0

#define CMD_SET_COLUMN_UPPER 0x10
#define CMD_SET_COLUMN_LOWER 0x00

#define CMD_SET_ADC_NORMAL 0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON 0xA5
#define CMD_SET_BIAS_9 0xA2
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW 0xE0       // Read-modify-Write (column address increment: Read:+0, Write:+1)
#define CMD_RMW_CLEAR 0xEE // END Read-modify-Write
#define CMD_INTERNAL_RESET 0xE2
#define CMD_SET_COM_NORMAL 0xC0
#define CMD_SET_COM_REVERSE 0xC8
#define CMD_SET_POWER_CONTROL 0x28
#define CMD_SET_RESISTOR_RATIO 0x20 // regulation ratio RR[2:0]

#define CMD_SET_VOLUME_FIRST 0x81
#define CMD_SET_VOLUME_SECOND 0 // EV[5:0]

#define CMD_SET_STATIC_OFF 0xAC
#define CMD_SET_STATIC_ON 0xAD
#define CMD_SET_STATIC_REG 0x0

#define CMD_SET_BOOSTER_FIRST 0xF8
#define CMD_SET_BOOSTER_234 0
#define CMD_SET_BOOSTER_5 1
#define CMD_SET_BOOSTER_6 3

#define CMD_NOP 0xE3
#define CMD_TEST 0xF0

void st7567_Init();
void st7567_Test();
void st7565_backlight(bool state);

void st7567_DrawHLine(uint8_t y, uint8_t color);
void st7567_DrawWLine(uint8_t x, uint8_t color);
void st7567_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void st7567_WriteString(uint8_t x, uint8_t y, const char *str, fontStyle_t font);
void st7567_WriteStringBack(uint8_t x, uint8_t y, const char *str, fontStyle_t font);
void st7567_WriteChar(uint8_t x, uint8_t y, char ch, fontStyle_t font);
 
void st7567_Clear();

void st7567_SetPixelBuffer(uint8_t x, uint8_t y, uint8_t color);
void st7567_UpdateScreen();
void sendData(uint8_t *data, uint16_t size);
void sendDataSingle(uint8_t data);
void sendCommand(uint8_t cmd);

#endif /* ST7567_H_ */
