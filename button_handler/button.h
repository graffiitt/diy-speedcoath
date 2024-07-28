#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <string.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

#define BUTTON_1 6
#define BUTTON_2 7
#define BUTTON_3 8
#define BUTTON_4 9

 #define REQUEST_HZ 150000           // 250 ms
#define COUNTER_LONG_PRESS 20

struct Button
{
    void (*handlerShortPress)(void);
    void (*handlerLongPress)(void);
    bool flag;
    int numberPin;
    bool lastState;
    uint32_t counterPress;
    uint32_t timePress; // timestamp when button was pressed
};

void settingButton(const struct Button *bt);
void handlerButton(const bool state, struct Button *bt);
void setButtonHandlerShort(uint8_t numButton, void (*fncHandler)());
void setButtonHandlerLong(uint8_t numButton, void (*fncHandler)());
bool buttonIrq(repeating_timer_t *rt);
void buttonHandlerInit();

#endif