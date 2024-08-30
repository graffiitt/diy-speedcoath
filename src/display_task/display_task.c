#include "display_task/display_task.h"
#include "display_task/menu_display.h"

#include "FreeRTOS.h"
#include "task.h"

void (*drawDiplay)();
uint8_t selectRow = 0;
extern TaskHandle_t taskDisplay;

void display_task(__unused void *params)
{
    menuSetup();
    while (true)
    {
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(2000));
        if (drawDiplay)
            drawDiplay();
        st7567_UpdateScreen();
        st7567_Clear();
    }
}
