#include "setup_display_data.h"
#include "settings_display.h"
#include "data_display.h"

uint8_t bufState[] = {0, 0, 0, 0};

void setupDataDispSetup()
{
    memcpy(bufState, dataItemsSelect, 4);

    setupDataDispButtonsSetup();
    drawDiplay = &setupDataDispDrawDisplay;
}

void setupDataDispDrawDisplay()
{
    std::string str;
    st7567_WriteString(0, 0, "data display", Font_7x10);
    str = "1: " + dataItems[bufState[0]].text;
    st7567_WriteString(7, 16, str.c_str(), Font_7x10);
    str = "2: " + dataItems[bufState[1]].text;
    st7567_WriteString(7, 16 + 12, str.c_str(), Font_7x10);
    str = "3: " + dataItems[bufState[2]].text;
    st7567_WriteString(7, 16 + 12*2, str.c_str(), Font_7x10);

    st7567_WriteCharBuf(0, 16 + 12 * selectRow, '>', Font_7x10);
}

void setupDataDispButtonsSetup()
{
    setButtonHandlerShort(0, setupDataDispApplyBotton);
    setButtonHandlerLong(0, nullptr);
    setButtonHandlerShort(1, setupDataDispUpButton);
    setButtonHandlerLong(1, setupDataDispChangeUpButton);
    setButtonHandlerShort(2, setupDataDispDownButton);
    setButtonHandlerLong(2, setupDataDispChangeDownButton);
    setButtonHandlerShort(3, setupDataDispBackButton);
}

void setupDataDispBackButton()
{
    settingsSetup();
    selectRow = 1;
    updateDisp();
}

void setupDataDispUpButton()
{
    if (selectRow < NUM_DATA_ITEMS - 1)
        selectRow++;
    else
        selectRow = 0;

    updateDisp();
}

void setupDataDispDownButton()
{
    if (selectRow > 0)
        selectRow--;
    else
        selectRow = NUM_DATA_ITEMS - 1;
    updateDisp();
}

void setupDataDispChangeUpButton()
{
    if (bufState[selectRow] < NUM_DATA_ITEMS - 1)
        bufState[selectRow]++;
    else
        bufState[selectRow] = 0;
    updateDisp();
}

void setupDataDispChangeDownButton()
{
    if (bufState[selectRow] > 0)
        bufState[selectRow]--;
    else
        bufState[selectRow] = NUM_DATA_ITEMS - 1;
    updateDisp();
}

void setupDataDispApplyBotton()
{
    memcpy(dataItemsSelect, bufState, 4);
    st7567_WriteString(110, 0, "wr", Font_7x10);
    updateDisp();
}
