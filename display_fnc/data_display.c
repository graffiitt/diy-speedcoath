#include "data_display.h"

void dataDisplaySetup()
{
    dataDisplayButtonHandler();
    drawDiplay = &dataDisplayDisplayDraw;
}

void dataDisplayButtonHandler()
{
    setButtonHandlerShort(0, 0);
    setButtonHandlerLong(0, 0);
    setButtonHandlerShort(1, 0);
    setButtonHandlerLong(1, 0);
    setButtonHandlerShort(2, 0);
    setButtonHandlerLong(2, 0);
    setButtonHandlerShort(3, dataDisplayBackButton);
}

extern struct Position pos;
void dataDisplayDisplayDraw()
{
    char str[10];
    sprintf(str, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
    st7567_WriteString(36, 0, str, FontStyle_veranda_9);
    if (pos.status == 1)
        st7567_WriteString(0, 0, "*", FontStyle_veranda_9);

    // draw lines
    st7567_DrawLine(63, 10, 63, 63, BLACK);
    st7567_DrawHLine(37, BLACK);
    st7567_DrawHLine(63, BLACK);
}

extern void menuSetup();
void dataDisplayBackButton()
{
    menuSetup();
    updateDisp();
}
