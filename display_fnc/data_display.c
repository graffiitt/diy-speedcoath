#include "data_display.h"

extern struct Time timeGPS;
extern struct Position pos;
extern uint8_t chargeValue;
extern void powerHandlerOFF();

//     "pulse",
//     "stroke count",
//     "stroke rate"
//     "split 500m",
//     "distance"
//      "chrono"
char *dataItemsText[] = {
    "pulse",
    "stroke count",
    "stroke rate",
    "split 500m",
    "distance",
    "chrono"};

uint8_t currentDataItem[] = {0, 1, 4, 3};
struct ItemObjectData dataItems[NUM_DATA_ITEMS];
repeating_timer_t _timerDisplay;

datetime_t timeStruct = {
    .year = 2020,
    .month = 06,
    .day = 05,
    .dotw = 5, // 0 is Sunday, so 5 is Friday
    .hour = 0,
    .min = 0,
    .sec = 0};

struct DataDisplayValue dataDisp = {.battery = &chargeValue,
                                    .distance = 0.00f,
                                    .ble = &bleData};

bool drawDisplayIrq(repeating_timer_t *rt)
{
    updateDisp();
    return true;
}

void dataDisplaySetup()
{
    dataDisplayButtonHandler();
    drawDiplay = &dataDisplayDisplayDraw;
    add_repeating_timer_us(5000000, drawDisplayIrq, NULL, &_timerDisplay);
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
    setButtonHandlerLong(3, dataDisplayButtonOff);
}

void dataDisplayButtonOff()
{
    cancel_repeating_timer(&_timerDisplay);
    powerHandlerOFF();
}

void dataDisplayDisplayDraw()
{
    char str[10];
    sprintf(str, "%02d:%02d:%02d", timeGPS.hours, timeGPS.minutes, timeGPS.seconds);
    st7567_WriteString(36, 0, str, FontStyle_veranda_9);
    sprintf(str, "%d%%", *dataDisp.battery);
    st7567_WriteStringBack(124, 0, str, FontStyle_veranda_9);

    if (pos.status == 1)
        st7567_WriteString(0, 0, "*", FontStyle_veranda_9);

    dataItems[currentDataItem[0]].drawItem(0, 11);
    dataItems[currentDataItem[1]].drawItem(64, 11);
    dataItems[currentDataItem[2]].drawItem(0, 37);
    dataItems[currentDataItem[3]].drawItem(64, 37);

    // draw lines
    st7567_DrawLine(63, 10, 63, 63, BLACK);
    st7567_DrawHLine(37, BLACK);
    st7567_DrawHLine(63, BLACK);
}

extern void menuSetup();
void dataDisplayBackButton()
{
    setButtonHandlerLong(3, powerHandlerOFF);
    cancel_repeating_timer(&_timerDisplay);
    menuSetup();
    updateDisp();
}

void initDataDisp()
{
    dataItems[0].text = dataItemsText[0];
    dataItems[0].drawItem = &drawPulseItem;
    dataItems[1].text = dataItemsText[1];
    dataItems[1].drawItem = &drawStrokeCountItem;
    dataItems[2].text = dataItemsText[2];
    dataItems[2].drawItem = &drawStroceRateItem;
    dataItems[3].text = dataItemsText[3];
    dataItems[3].drawItem = &drawSplitItem;
    dataItems[4].text = dataItemsText[4];
    dataItems[4].drawItem = &drawDistanse;
    dataItems[5].text = dataItemsText[5];
    dataItems[5].drawItem = &drawChrono;

    rtc_set_datetime(&timeStruct);
}

void drawPulseItem(const int x, const int y)
{

    if (dataDisp.ble->sensor_contact == 3)
    {
        char str[5];
        sprintf(str, "%03d", (int)dataDisp.ble->heart_rate);
        st7567_WriteString(x, y + 4, str, FontStyle_veranda_26);
    }
    else
        st7567_WriteString(x, y + 4, "---", FontStyle_veranda_18);

    st7567_WriteString(x + 51, y + 15, "hr", FontStyle_veranda_9);
}

void drawStrokeCountItem(const int x, const int y)
{
    char str[6];
    sprintf(str, "%d", 1);
    st7567_WriteStringBack(x + 62, y + 7, str, FontStyle_veranda_18);
}

void drawStroceRateItem(const int x, const int y)
{
    int stroke = 0;
    char str[2];
    sprintf(str, "%02d", stroke);
    st7567_WriteString(x + 3, y + 4, str, FontStyle_veranda_26);
    st7567_WriteString(x + 41, y + 15, "s/m", FontStyle_veranda_9);
}

void drawSplitItem(const int x, const int y)
{
    char str[5];
    double min, sec;
    if (pos.speed > MINIMUM_SPEED)
        sec = modf(30 / pos.speed, &min);
    else
        sec = min = 0;
    sprintf(str, "%02d:%02d", (int)min, (int)(sec * 60));
    st7567_WriteString(x + 4, y + 7, str, FontStyle_veranda_18);
}

void drawDistanse(const int x, const int y)
{
    char str[6];
    if (pos.speed > MINIMUM_SPEED)
        dataDisp.distance += calc_distance();
    //  dataDisp.distance+=0.1;
    sprintf(str, "%0.2f", dataDisp.distance);
    st7567_WriteStringBack(x + 60, y + 7, str, FontStyle_veranda_18);
}

void drawChrono(const int x, const int y)
{
    char str[10];
    rtc_get_datetime(&timeStruct);

    sprintf(str, "%01d:%02d:%02d", timeStruct.hour, timeStruct.min, timeStruct.sec);
    st7567_WriteString(x + 3, y + 9, str, FontStyle_veranda_9);
}

uint8_t bufState[] = {0, 0, 0, 0};
void setupDispItemsSetup()
{
    memcpy(bufState, currentDataItem, 4);
    setupDispItemsButtonHandler();
    drawDiplay = &setupDispItemsDraw;
}

void setupDispItemsDraw()
{
    char str[20];
    st7567_WriteString(0, 0, "display items", FontStyle_veranda_9);

    sprintf(str, "1: %s", dataItems[bufState[0]].text);
    st7567_WriteString(7, 16, str, FontStyle_veranda_9);
    sprintf(str, "2: %s", dataItems[bufState[1]].text);
    st7567_WriteString(7, 16 + 12, str, FontStyle_veranda_9);
    sprintf(str, "3: %s", dataItems[bufState[2]].text);
    st7567_WriteString(7, 16 + 24, str, FontStyle_veranda_9);
    sprintf(str, "4: %s", dataItems[bufState[3]].text);
    st7567_WriteString(7, 16 + 36, str, FontStyle_veranda_9);

    st7567_WriteChar(0, 16 + 12 * selectRow, '>', FontStyle_veranda_9);
}

extern void powerHandlerOFF();
void setupDispItemsButtonHandler()
{
    setButtonHandlerShort(0, setupDispItemsApplyButton);
    setButtonHandlerLong(0, 0);
    setButtonHandlerShort(1, setupDispItemsUpButton);
    setButtonHandlerLong(1, setupDispItemsChangeUpButton);
    setButtonHandlerShort(2, setupDispItemsDownButton);
    setButtonHandlerLong(2, setupDispItemsChangeDownButton);
    setButtonHandlerShort(3, setupDispItemsBackButton);
    setButtonHandlerLong(3, powerHandlerOFF);
}

extern void settingsSetup();
void setupDispItemsBackButton()
{
    settingsSetup();
    selectRow = 1;
    updateDisp();
}

void setupDispItemsApplyButton()
{
    memcpy(currentDataItem, bufState, 4);
    st7567_WriteString(110, 0, "wr", FontStyle_veranda_9);
    updateDisp();
}

void setupDispItemsUpButton()
{
    if (selectRow < 3)
        selectRow++;
    else
        selectRow = 0;
    updateDisp();
}

void setupDispItemsChangeUpButton()
{
    if (bufState[selectRow] < NUM_DATA_ITEMS - 1)
        bufState[selectRow]++;
    else
        bufState[selectRow] = 0;
    updateDisp();
}

void setupDispItemsDownButton()
{
    if (selectRow > 0)
        selectRow--;
    else
        selectRow = 3;
    updateDisp();
}

void setupDispItemsChangeDownButton()
{
    if (bufState[selectRow] > 0)
        bufState[selectRow]--;
    else
        bufState[selectRow] = NUM_DATA_ITEMS - 1;
    updateDisp();
}
