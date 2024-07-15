#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include "st7565/st7567.h"
#include "button_handler/button.h"
#include <string>
#include <vector>

extern uint8_t selectRow;
extern void (*drawDiplay)();

struct ItemObject
{
    std::string text;
    void (*setupDisplay)(void);
 };

void drawList(std::vector<ItemObject> *items);
void drawTime(struct Time *tm);
void buttonUpList();
void buttonDownList();

#endif