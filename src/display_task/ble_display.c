#include "display_task/ble_diplay.h"

repeating_timer_t _bleTimer;
extern cvector(struct BLE_Item) bleItems;
extern gc_state_t stateBLE;
extern void (*handlerConnection)(void);

void bleSettingsSetup()
{
    // handlerConnection = &updateDisp;
    bleSettingsButtonHandler();
    drawDiplay = &bleSettingsDisplayDraw;
}

void bleSettingsButtonHandler()
{
    setButtonHandlerShort(0, bleSettingsUpdate);
    setButtonHandlerLong(0, bleSettingsConnectButton);
    setButtonHandlerShort(1, buttonUpList);
    setButtonHandlerLong(1, 0);
    setButtonHandlerShort(2, buttonDownList);
    setButtonHandlerLong(2, 0);
    setButtonHandlerShort(3, bleSettingsBackButton);
}

void bleSettingsDisplayDraw()
{
    if (stateBLE == TC_W4_SCAN_RESULT)
        st7567_WriteString(0, 0, "bluetooth search dev", FontStyle_veranda_9);
    else if (stateBLE == TC_CONNECTED)
        st7567_WriteString(0, 0, "bluetooth connected", FontStyle_veranda_9);
    else
        st7567_WriteString(0, 0, "bluetooth disconnected", FontStyle_veranda_9);

    if (stateBLE == TC_CONNECTED)
        st7567_WriteString(7, 16, "device connected", FontStyle_veranda_9);
    else
        drawListBLE(&bleItems);
}

void drawListBLE(cvector(struct BLE_Item) * items)
{
    uint8_t x = 7;
    uint8_t y = 16;

    if (cvector_size(*items) == 0)
        return;

    if (selectRow > cvector_size(*items) - 1)
    {
        selectRow = cvector_size(*items) - 1;
    }

    for (uint8_t i = 0; i < NUM_DISPLAY_ROWS; i++)
    {
        int item = selectRow > (NUM_DISPLAY_ROWS - 1) ? (selectRow - NUM_DISPLAY_ROWS + 1) + i : i;
        if (item < cvector_size(*items))
        {
            st7567_WriteString(x, y, cvector_at(*items, i)->name, FontStyle_veranda_9);
            y += 12;
        }
    }
    y = 16 + 12 * (selectRow > (NUM_DISPLAY_ROWS - 1) ? (NUM_DISPLAY_ROWS - 1) : selectRow);
    st7567_WriteChar(0, y, '>', FontStyle_veranda_9);
}

void bleSettingsConnectButton()
{
    if (stateBLE == TC_OFF)
    {
        connectDevice(&cvector_at(bleItems, selectRow)->address, cvector_at(bleItems, selectRow)->addr_type);
    }
    else if (stateBLE == TC_CONNECTED)
    {
        printf("discon button\n");
        disconnectDevice();
    }
}

// update list devices
void bleSettingsUpdate()
{
    if (stateBLE == TC_CONNECTED)
        return;
    if (stateBLE != TC_W4_SCAN_RESULT)
    {
        ble_clearDevices();
        ble_scan_start();
        add_repeating_timer_us(-5000000, bleScanTimer, NULL, &_bleTimer);
    }
    // updateDisp();
}

bool bleScanTimer(repeating_timer_t *rt)
{
    ble_scan_stop();
    // updateDisp();
    cancel_repeating_timer(&_bleTimer);
    return true;
}

extern void settingsSetup();
void bleSettingsBackButton()
{
    if (stateBLE == TC_W4_SCAN_RESULT)
    {
        ble_scan_stop();
        cancel_repeating_timer(&_bleTimer);
    }
    ble_clearDevices();
    settingsSetup();
    selectRow = 0;
    // updateDisp();
}
