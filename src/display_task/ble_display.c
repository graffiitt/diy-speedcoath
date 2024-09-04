#include "display_task/ble_diplay.h"

#include "FreeRTOS.h"
#include "task.h"

static int32_t timeScanBegin;

extern cvector(struct BLE_Item) bleItems;
extern gc_state_t stateBLE;
extern void (*handlerConnection)(void);

static void displayDraw();
static void drawListBLE(cvector(struct BLE_Item) * items);
// button handlers
static void connectButton();
static void updateDevices();
static void backButton();

static void handlerBle()
{
    xTaskNotifyGive(taskDisplay);
}

static void buttonHandler()
{
    setButtonHandlerShort(0, updateDevices);
    setButtonHandlerLong(0, connectButton);
    setButtonHandlerShort(1, buttonUpList);
    setButtonHandlerLong(1, 0);
    setButtonHandlerShort(2, buttonDownList);
    setButtonHandlerLong(2, 0);
    setButtonHandlerShort(3, backButton);
}

void bleSettingsSetup()
{
    handlerConnection = &handlerBle;
    buttonHandler();
    drawDiplay = &displayDraw;
}

void displayDraw()
{
    if (stateBLE == TC_W4_SCAN_RESULT)
    {
        st7567_WriteString(0, 0, "bluetooth search device", FontStyle_veranda_9);
        if ((time_us_32() - timeScanBegin) > 5000000)
            ble_scan_stop();
    }
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
            st7567_WriteString(x, y, cvector_at(*items, item)->name, FontStyle_veranda_9);
            y += 12;
        }
    }
    y = 16 + 12 * (selectRow > (NUM_DISPLAY_ROWS - 1) ? (NUM_DISPLAY_ROWS - 1) : selectRow);
    st7567_WriteChar(0, y, '>', FontStyle_veranda_9);
}

void connectButton()
{
    if (stateBLE == TC_OFF)
    {
        connectDevice(cvector_at(bleItems, selectRow));
    }
    else if (stateBLE == TC_CONNECTED)
    {
        disconnectDevice();
    }
}

// update list devices
void updateDevices()
{
    if (stateBLE == TC_CONNECTED)
        return;

    if (stateBLE == TC_W4_SCAN_RESULT)
    {
        ble_scan_stop();
        ble_clearDevices();
        ble_scan_start();
        timeScanBegin = time_us_32();
        xTaskNotifyGive(taskDisplay);
        return;
    }

    if (stateBLE != TC_W4_SCAN_RESULT)
    {
        ble_clearDevices();
        ble_scan_start();
        timeScanBegin = time_us_32();
        xTaskNotifyGive(taskDisplay);
    }
}

void settingsSetup();
void backButton()
{
    startChangeDisplay();
    if (stateBLE == TC_W4_SCAN_RESULT)
    {
        ble_scan_stop();
    }
    ble_clearDevices();
    settingsSetup();
    selectRow = 0;
    endChangeDisplay();
}
