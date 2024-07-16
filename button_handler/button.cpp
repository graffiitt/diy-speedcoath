#include "button.h"

struct Button button[4];
repeating_timer_t _timerButton;

void settingButton(const Button *bt)
{
    gpio_init(bt->numberPin);
    gpio_set_dir(bt->numberPin, GPIO_IN);
    gpio_pull_up(bt->numberPin);
}

void handlerButton(bool state, Button *bt)
{
    if (!state && bt->lastState) // fix pressed
    {
        bt->flag = 1;
        printf("pressed %d %d \n", bt->numberPin, time_us_32());
        bt->timePress = time_us_32();
    }

    uint32_t deltaTime = time_us_32() - bt->timePress;

    if (!(state && bt->lastState) &&
        (deltaTime > LONG_PRESS_TIME) && bt->flag)
    {
        bt->flag = 0;
        printf("pressed long %d %d \n", bt->numberPin, deltaTime);
        if (bt->handlerLongPress)
            bt->handlerLongPress();
    }
    if ((state && !bt->lastState) && (deltaTime < LONG_PRESS_TIME))
    {
        printf("pressed short %d %d \n", bt->numberPin, deltaTime);
        if (bt->handlerShortPress)
            bt->handlerShortPress();
    }
    bt->lastState = state;
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

bool alarmIrq(repeating_timer_t *rt)
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

    add_repeating_timer_us(-1000000 / 5, alarmIrq, NULL, &_timerButton);
}
