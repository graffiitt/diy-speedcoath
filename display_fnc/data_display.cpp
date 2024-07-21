#include "data_display.h"
#include "menu_display.h"
#include "gps_ublox/gps.h"

struct ItemObjectData dataItems[NUM_DATA_ITEMS];
int8_t dataItemsSelect[4] = {0, 1, 2, 3};
extern struct Time time;
extern struct Position pos;

void dataDisplayInit()
{
    dataItems[0].text = "pulse";
    dataItems[0].drawItem = &drawPulseItem;

    dataItems[1].text = "stroke rate";
    dataItems[1].drawItem = &drawStroceRate;

    dataItems[2].text = "stroke count";
    dataItems[2].drawItem = &drawCounterStroke;

    dataItems[3].text = "split";
    dataItems[3].drawItem = &drawSplit;
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
    if (pos.status == 1) st7567_WriteString(0, 0, "*", FontStyle_veranda_9);
    sprintf(str, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
    st7567_WriteString(35, 0, str, FontStyle_veranda_9);

    dataItems[dataItemsSelect[0]].drawItem(0, 11);
    dataItems[dataItemsSelect[1]].drawItem(64, 11);
    dataItems[dataItemsSelect[2]].drawItem(0, 37);
    dataItems[dataItemsSelect[3]].drawItem(64, 37);

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

    st7567_WriteString(x, y + 4, "123", FontStyle_veranda_26);
    st7567_WriteString(x + 51, y + 15, "hr", FontStyle_veranda_9);
}

void drawStroceRate(const int x, const int y)
{
    int stroke = 0;
    char str[2];
    sprintf(str, "%02d", stroke);
    st7567_WriteString(x + 3, y + 4, str, FontStyle_veranda_26);
    st7567_WriteString(x + 41, y + 15, "s/m", FontStyle_veranda_9);
}

void drawCounterStroke(const int x, const int y)
{
    char str[5];
    sprintf(str, "%05d", 1);
    st7567_WriteString(x + 4, y + 7, str, FontStyle_veranda_18);
}

void drawSplit(const int x, const int y)
{
    char str[5];
     double min, sec;
    if(pos.speed != 0)
    sec = modf(30/pos.speed, &min);
    else 
    sec=min=0;
    sprintf(str, "%02d:%02d", (int)min, (int)(sec * 60));
    st7567_WriteString(x + 4, y + 7, str, FontStyle_veranda_18);
}