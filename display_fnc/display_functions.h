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

void drawList(std::vector<ItemObjectList> *items);
void drawTime(struct Time *tm);
void buttonUpList();
void buttonDownList();

#endif