#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include "st7565/st7567.h"
#include "button_handler/button.h"
#include <string>
#include <vector>

#define NUM_DISPLAY_ROWS 4

extern void updateDisp();
extern uint8_t selectRow;
extern void (*drawDiplay)();

struct ItemObjectList
{
    std::string text;
    void (*setupDisplay)(void);
};

struct ItemObjectData
{
    std::string text;
    void (*drawItem)(const int, const int);
};

void drawList(const std::vector<ItemObjectList> *items);

void buttonUpList();
void buttonDownList();

#endif