#include "data_display.h"
#include "menu_display.h"

struct ItemObjectData dataItems[NUM_DATA_ITEMS];
int8_t dataItemsSelect[4] = {0, 1, 0, 0};

void dataDisplayInit()
{
    dataItems[0].text = "pulse";
    dataItems[0].drawItem = &drawPulseItem;

    dataItems[1].text = "stroce rate";
    dataItems[1].drawItem = &drawStroceRate;
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
    st7567_WriteString(0, 0, "disp", Font_7x10);

    dataItems[dataItemsSelect[0]].drawItem(0, 11);
    dataItems[dataItemsSelect[1]].drawItem(63, 11);
    // dataItems[dataItemsSelect[0]].drawItem(0, 11);
    // dataItems[dataItemsSelect[0]].drawItem(0, 11);

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
}

void drawStroceRate(const int x, const int y)
{
    st7567_WriteString(x, y + 2, "11", Font_16x26);
}