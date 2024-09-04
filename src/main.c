#include "FreeRTOS.h"
#include "task.h"

#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "hardware/watchdog.h"

#include "main_task/main_task.h"
#include "display_task/display_task.h"
#include "display_task/ble_diplay.h"
#include "battery.h"

// Priorities of our threads - higher numbers are higher priority
#define DISPLAY_TASK_PRIORITY (tskIDLE_PRIORITY + 4UL)
#define MAIN_TASK_TASK_PRIORITY (tskIDLE_PRIORITY + 2UL)

// Stack sizes of our threads in words (4 bytes)
#define DISPLAY_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define MAIN_TASK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

TaskHandle_t taskDisplay;
void xStartTasks()
{
  buttonHandlerInit();
  xTaskCreate(display_task, "displayTask", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &taskDisplay);

  xTaskCreate(main_task, "bleTask", MAIN_TASK_TASK_STACK_SIZE, NULL, MAIN_TASK_TASK_PRIORITY, NULL);
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
  sleep_ms(2000);

  start_adc();

  xStartTasks();
  vTaskStartScheduler();
  return 0;
}
