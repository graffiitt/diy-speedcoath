#include "FreeRTOS.h"
#include "task.h"

#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "hardware/watchdog.h"

#include "display_task/display_task.h"
#include "battery.h"

// Priorities of our threads - higher numbers are higher priority
#define DISPLAY_TASK_PRIORITY (tskIDLE_PRIORITY + 4UL)

// Stack sizes of our threads in words (4 bytes)
#define DISPLAY_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

TaskHandle_t taskDisplay;
void xStartTasks()
{
  xTaskCreate(display_task, "displayTask", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &taskDisplay);
}

int main()
{
    timer_hw->dbgpause = 0x2;

  st7567_Init();
  if (watchdog_caused_reboot())
  {
    st7567_WriteString(0, 20, "WATCHDOG", FontStyle_veranda_18);
    st7567_WriteString(0, 36, "REBOOT", FontStyle_veranda_18);
  }
  else
  {
    st7567_WriteString(0, 20, "TUMBA", FontStyle_veranda_18);
    st7567_WriteString(0, 36, "by RTOS", FontStyle_veranda_9);

  }
  st7567_UpdateScreen();
  st7567_Clear();
  buttonHandlerInit();

  start_adc();

  xStartTasks();
  vTaskStartScheduler();
  return 0;
}
