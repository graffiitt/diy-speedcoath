#include "display_task/display_functions.h"
#include "display_task/data_display.h"

extern uint8_t currentDataItem[];
extern struct ItemObjectData dataItems[];

// display for settings display items
static void displayDraw();
static void backButton();
static void applyButton();
static void upButton();
static void changeUpButton();
static void downButton();
static void changeDownButton();

static void buttonHandler()
{
    setButtonHandlerShort(0, applyButton);
    setButtonHandlerShort(1, upButton);
    setButtonHandlerLong(1, changeUpButton);
    setButtonHandlerShort(2, downButton);
    setButtonHandlerLong(2, changeDownButton);
    setButtonHandlerShort(3, backButton);
}

uint8_t bufState[] = {0, 0, 0, 0};
void setupDispItemsSetup()
{
    memcpy(bufState, currentDataItem, 4);
    buttonHandler();
    drawDiplay = &displayDraw;
}

void displayDraw()
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

void settingsSetup();
void backButton()
{
    startChangeDisplay();
    settingsSetup();
    selectRow = 1;
    endChangeDisplay();
}

void applyButton()
{
    memcpy(currentDataItem, bufState, 4);
    st7567_WriteString(110, 0, "wr", FontStyle_veranda_9);
    xTaskNotifyGive(taskDisplay);
}

void upButton()
{
    if (selectRow < 3)
        selectRow++;
    else
        selectRow = 0;
    xTaskNotifyGive(taskDisplay);
}

void changeUpButton()
{
    if (bufState[selectRow] < NUM_DATA_ITEMS - 1)
        bufState[selectRow]++;
    else
        bufState[selectRow] = 0;
    xTaskNotifyGive(taskDisplay);
}

void downButton()
{
    if (selectRow > 0)
        selectRow--;
    else
        selectRow = 3;
    xTaskNotifyGive(taskDisplay);
}

void changeDownButton()
{
    if (bufState[selectRow] > 0)
        bufState[selectRow]--;
    else
        bufState[selectRow] = NUM_DATA_ITEMS - 1;
    xTaskNotifyGive(taskDisplay);
}
