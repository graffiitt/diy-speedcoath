#include "display_task/display_task.h"
#include "display_task/data_display.h"
#include "../bluetooth/ble_client.h"

void (*drawDiplay)();
uint8_t selectRow = 0;
SemaphoreHandle_t dispSem;
extern TaskHandle_t taskMain;

static void sleepHandle()
{
    static bool state = true;
    if (state) // sleep
    {
        st7567_Backlight(0);
        state = false;
        gps_power(0);
        disconnectDevice();
        ble_off();
        vTaskSuspend(taskDisplay);
        vTaskSuspend(taskMain);
    }
    else
    {
        st7567_Backlight(1);

        st7567_WriteString(0, 20, "TUMBA", FontStyle_veranda_18);
        st7567_WriteString(0, 36, "by RTOS", FontStyle_veranda_9);
        st7567_UpdateScreen();
        st7567_Clear();
        ble_on();
        gps_power(1);
        vTaskDelay(500);
        gps_configure();
        vTaskResume(taskDisplay);
        vTaskResume(taskMain);
        state = true;
    }
}

void display_task(__unused void *params)
{
    dispSem = xSemaphoreCreateMutex();
    xSemaphoreGive(dispSem);
    menuSetup();
    initDataDisp();
    setButtonHandlerLong(3, sleepHandle);
    while (true)
    {
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(2000));
        xSemaphoreTake(dispSem, portMAX_DELAY);
        if (drawDiplay)
            drawDiplay();
        st7567_DrawHLine(10, BLACK);
        st7567_UpdateScreen();
        st7567_Clear();
        xSemaphoreGive(dispSem);
    }
}
