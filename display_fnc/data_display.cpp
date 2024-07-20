#include "data_display.h"
#include "menu_display.h"
#include "gps_ublox/gps.h"

struct ItemObjectData dataItems[NUM_DATA_ITEMS];
int8_t dataItemsSelect[4] = {0, 1, 2, 0};
extern struct Time time;

void dataDisplayInit()
{
    dataItems[0].text = "pulse";
    dataItems[0].drawItem = &drawPulseItem;

    dataItems[1].text = "stroke rate";
    dataItems[1].drawItem = &drawStroceRate;

    dataItems[2].text = "stroke count";
    dataItems[2].drawItem = &drawCounterStroke;
}

void dataDisplaySetup()
{

    dataDisplayButtonSetup();
    drawDiplay = &dataDisplayDraw;
}

void dataDisplayButtonSetup()
{
    setButtonHandlerShort(0, nullptr);
    setButtonHandlerLong(0, nullptr);
    setButtonHandlerShort(1, nullptr);
    setButtonHandlerLong(1, nullptr);
    setButtonHandlerShort(2, nullptr);
    setButtonHandlerLong(2, nullptr);
    setButtonHandlerShort(3, dataDisplayBackButton);
}

void dataDisplayDraw()
{
    char str[10];
    sprintf(str, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
    st7567_WriteString(35, 0, str, Font_7x10);

    dataItems[dataItemsSelect[0]].drawItem(0, 11);
    dataItems[dataItemsSelect[1]].drawItem(64, 11);
    dataItems[dataItemsSelect[2]].drawItem(0, 37);
    // dataItems[dataItemsSelect[3]].drawItem(64, 37);

    // draw lines
    st7567_DrawLine(63, 10, 63, 63, BLACK);
    st7567_DrawHLine(37, BLACK);
    st7567_DrawHLine(63, BLACK);
}

void dataDisplayBackButton()
{
    selectRow = 0;
    menuSetup();
    updateDisp();
}

void drawPulseItem(const int x, const int y)
{

    st7567_WriteString(x, y + 2, "123", Font_16x26);
    st7567_WriteString(x + 48, y + 15, "hr", Font_7x10);
}

void drawStroceRate(const int x, const int y)
{

    st7567_WriteString(x + 3, y + 2, "--", Font_16x26);
    st7567_WriteString(x + 39, y + 15, "s/m", Font_7x10);
}

void drawCounterStroke(const int x, const int y)
{
    char str[5];
    sprintf(str, "%05d", 1);
    st7567_WriteString(x + 4, y + 6, str, Font_11x18);
}