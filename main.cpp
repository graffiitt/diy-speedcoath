/**
 * Copyright (c) 2023 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/timer.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
 


#define ALARM_NUM_DRAW 1
#define ALARM_IRQ_DRAW TIMER_IRQ_0
#define TIMER_DELAY_DRAW 1000000

void (*drawDiplay)();

void drawDisplayIrq()
{
    hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM_DRAW);
    timer_hw->alarm[ALARM_NUM_DRAW] = timer_hw->timerawl + TIMER_DELAY_DRAW;

    if (drawDiplay)
        drawDiplay();
}

int main()
{
    stdio_init_all();
  //  st7567_Init();

 
 
    hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM_DRAW);
    irq_set_exclusive_handler(ALARM_IRQ_DRAW, drawDisplayIrq);
    irq_set_enabled(ALARM_IRQ_DRAW, true);
    timer_hw->alarm[ALARM_NUM_DRAW] = timer_hw->timerawl + TIMER_DELAY_DRAW;

    while (1)
    {
    }
    return 0;
}
