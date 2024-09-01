#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include "st7567.h"
#include "../button_handler/button.h"
#include <string.h>
#include "cvector.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define NUM_DISPLAY_ROWS 4
#define NUM_DATA_ITEMS 6

extern TaskHandle_t taskDisplay;
extern uint8_t selectRow;
extern void (*drawDiplay)();

struct ItemObjectList
{
    const char *text;
    void (*setupDisplay)(void);
};

struct ItemObjectData
{
    const char *text; // for select current data item
    void (*drawItem)(const int, const int);
};

void drawList(cvector(struct ItemObjectList) * items);

void buttonUpList();
void buttonDownList();

void startChangeDisplay();
void endChangeDisplay();

#endif