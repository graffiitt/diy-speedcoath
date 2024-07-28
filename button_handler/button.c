#include "button.h"

struct Button button[4];
repeating_timer_t _timerButton;

void settingButton(const struct Button *bt)
{
    gpio_init(bt->numberPin);
    gpio_set_dir(bt->numberPin, GPIO_IN);
    gpio_pull_up(bt->numberPin);
}

void setButtonHandlerShort(uint8_t numButton, void (*fncHandler)())
{
    if (numButton > 4)
        return;
    button[numButton].handlerShortPress = fncHandler;
}

void setButtonHandlerLong(uint8_t numButton, void (*fncHandler)())
{
    if (numButton > 4)
        return;
    button[numButton].handlerLongPress = fncHandler;
}

bool buttonIrq(repeating_timer_t *rt)
{
    // set timer for next irq
    // hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM);
    // timer_hw->alarm[ALARM_NUM] = timer_hw->timerawl + TIMER_DELAY;

    bool state[] = {
        gpio_get(BUTTON_1),
        gpio_get(BUTTON_2),
        gpio_get(BUTTON_3),
        gpio_get(BUTTON_4),
    };

    if (3 > (state[0] + state[1] + state[2] + state[3]))
        return true;
    for (int i = 0; i < 4; i++)
        handlerButton(state[i], &button[i]);
    return true;
}

void handlerButton(const bool state, struct Button *bt)
{
    if (!state)
        bt->counterPress++;

    if ((!state) && bt->lastState) // fix pressed
    {
        bt->counterPress = 0;
        bt->flag = 1;
    }
    if (!(state && bt->lastState) &&
        (bt->counterPress > COUNTER_LONG_PRESS) && bt->flag)
    {
        bt->flag = 0;
        if (bt->handlerLongPress)
            bt->handlerLongPress();
    }
    if ((state && !bt->lastState) && (bt->counterPress < COUNTER_LONG_PRESS))
    {
        if (bt->handlerShortPress)
            bt->handlerShortPress();
    }
    bt->lastState = state;
}

void buttonHandlerInit()
{
    button[0].numberPin = BUTTON_1;
    button[1].numberPin = BUTTON_2;
    button[2].numberPin = BUTTON_3;
    button[3].numberPin = BUTTON_4;

    for (int i = 0; i < 4; i++)
    {
        settingButton(&button[i]);
    }

    add_repeating_timer_us(-REQUEST_HZ, buttonIrq, NULL, &_timerButton);
}
