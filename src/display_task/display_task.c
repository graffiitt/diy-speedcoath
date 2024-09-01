#include "display_task/display_task.h"
#include "display_task/data_display.h"

void (*drawDiplay)();
uint8_t selectRow = 0;
SemaphoreHandle_t dispSem;
extern TaskHandle_t taskDisplay;

void display_task(__unused void *params)
{
    dispSem = xSemaphoreCreateMutex();
    xSemaphoreGive(dispSem);
    menuSetup();
    initDataDisp();
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
