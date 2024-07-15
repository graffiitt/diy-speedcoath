/*
 * st7567.c
 *
 *  Created on: Jun 23, 2024
 *      Author: vlad
 */

#include "st7567.h"

uint8_t lcd_buffer[LCDWIDTH * LCDHEIGHT / LCDNUMPAGES];

void st7567_Init()
{
	spiInit();

	gpio_init(ST7567A_RST_Pin);
	gpio_set_dir(ST7567A_RST_Pin, GPIO_OUT);
	gpio_init(ST7567A_DC_Pin);
	gpio_set_dir(ST7567A_DC_Pin, GPIO_OUT);
	gpio_init(ST7567_LED_Pin);
	gpio_set_dir(ST7567_LED_Pin, GPIO_OUT);
	gpio_put(ST7567_LED_Pin, 0);
	gpio_put(ST7567A_RST_Pin, 0);
	sleep_ms(50);
	gpio_put(ST7567A_RST_Pin, 1);
	sleep_ms(50);

	sendCommand(CMD_SET_BIAS_7);
	sendCommand(CMD_SET_ADC_NORMAL);
	sendCommand(CMD_SET_COM_REVERSE);
	sendCommand(CMD_SET_VOLUME_FIRST);
	sendCommand(34);
	sendCommand(CMD_SET_POWER_CONTROL | 0x4);
	sleep_ms(50);
	sendCommand(CMD_SET_POWER_CONTROL | 0x6);
	sleep_ms(50);
	sendCommand(CMD_SET_POWER_CONTROL | 0x7);
	sleep_ms(50);

	sendCommand(CMD_SET_RESISTOR_RATIO | 0x2);

	sendCommand(0xA4);
	sendCommand(0xAF);

	st7567_Clear();
	st7567_UpdateScreen();
}

void st7567_Test()
{
	st7567_WriteString(0, 0, "123%", Font_7x10);
	st7567_WriteString(0, 40, "123", Font_16x26); // 3 ячейка
	st7567_WriteString(0, 16, "12", Font_11x18);

	// отрисовка линий раскладки на 4 ячейки
	st7567_DrawHLine(10, BLACK);
	st7567_DrawLine(63, 10, 63, 63, BLACK);
	st7567_DrawHLine(37, BLACK);
	st7567_DrawHLine(63, BLACK);

	st7567_UpdateScreen();
	// st7567_WriteString(0, 26 + 18, "12:34.6", Font_7x10);
	// st7567_DrawWLine(120, BLACK);
	// st7567_DrawHLine(60, BLACK);
	// st7567_DrawLine(10, 58, 50, 50, BLACK);
}

void st7567_DrawHLine(uint8_t y, uint8_t color)
{
	for (uint8_t x = 0; x < LCDWIDTH; x++)
	{
		if (color)
			lcd_buffer[x + (y / 8) * 128] |= (1 << (y % 8));
		else
			lcd_buffer[x + (y / 8) * 128] &= ~(1 << (y % 8));
	}
}

void st7567_DrawWLine(uint8_t x, uint8_t color)
{
	for (uint8_t y = 0; y < LCDNUMPAGES; y++)
	{
		if (color)
		{
			lcd_buffer[x + y * 128] = 255;
		}
		else
		{
			lcd_buffer[x + y * 128] = 0;
		}
	}
}

void st7567_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
	const int8_t deltaX = abs(x2 - x1);
	const int8_t deltaY = abs(y2 - y1);
	const int8_t signX = x1 < x2 ? 1 : -1;
	const int8_t signY = y1 < y2 ? 1 : -1;

	int8_t error = deltaX - deltaY;

	st7567_SetPixelBuffer(x2, y2, color);

	while (x1 != x2 || y1 != y2)
	{
		st7567_SetPixelBuffer(x1, y1, color);
		const int8_t error2 = error * 2;

		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
	// st7567_UpdateScreen();
}

void st7567_WriteString(uint8_t x, uint8_t y, const char *str, FontDef font)
{
	while (*str)
	{
		if (x + font.width >= LCDWIDTH)
		{
			x = 0;
			y += font.height;
			if (*str == ' ')
			{
				// skip spaces in the beginning of the new line
				str++;
				continue;
			}
		}
		st7567_WriteCharBuf(x, y, *str, font);
		x += font.width;
		str++;
	}
	// st7567_UpdateScreen();
}

void st7567_WriteChar(uint8_t x, uint8_t y, char ch, FontDef font)
{
	st7567_WriteCharBuf(x, y, ch, font);
	// st7567_UpdateScreen();
}

void st7567_PrintWithDelimetr(uint8_t num1, uint8_t num2, uint8_t widthDelim)
{
}

void st7567_Clear()
{
	memset(lcd_buffer, 0, 1024);
	// st7567_UpdateScreen();
}

void st7567_SetPixelBuffer(uint8_t x, uint8_t y, uint8_t color)
{
	if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
		return;

	// x is which column
	if (color)
		lcd_buffer[x + (y / 8) * 128] |= (1 << (y % 8));
	else
		lcd_buffer[x + (y / 8) * 128] &= ~(1 << (y % 8));
}

void st7567_WriteCharBuf(uint8_t x, uint8_t y, char ch, FontDef font)
{
	uint32_t b;
	for (uint8_t i = 0; i < font.height; i++)
	{
		b = font.data[(ch - 32) * font.height + i];
		for (uint8_t j = 0; j < font.width; j++)
		{
			if ((b << j) & 0x8000)
			{
				st7567_SetPixelBuffer(x + j, y + i, BLACK);
			}
			else
			{
				st7567_SetPixelBuffer(x + j, y + i, WHITE);
			}
		}
	}
}

void st7567_UpdateScreen()
{
	sendCommand(CMD_SET_DISP_START_LINE);
	for (uint8_t bank = 0; bank < LCDNUMPAGES; bank++)
	{
		sendCommand(CMD_SET_PAGE + bank);
		sendCommand(CMD_SET_COLUMN_UPPER);
		sendCommand(CMD_SET_COLUMN_LOWER);
		for (uint8_t i = 0; i < 128; i++)
		{
			sendDataSingle(lcd_buffer[128 * bank + i]);
		}
	}
}

void sendData(uint8_t *data, uint16_t size)
{
	gpio_put(ST7567A_CS_Pin, 0);
	gpio_put(ST7567A_DC_Pin, 1);
	spiWriteArr(data, size);
}

void sendDataSingle(uint8_t data)
{
	gpio_put(ST7567A_CS_Pin, 0);
	gpio_put(ST7567A_DC_Pin, 1);
	spiWriteChr(data);
}

void sendCommand(uint8_t cmd)
{
	gpio_put(ST7567A_CS_Pin, 0);
	gpio_put(ST7567A_DC_Pin, 0);
	spiWriteChr(cmd);
}
