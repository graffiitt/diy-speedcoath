#include "hardware/timer.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"

#include "display_fnc/menu_display.h"
#include "st7565/st7567.h"
#include "button_handler/button.h"

void (*drawDiplay)();
uint8_t selectRow = 0;
repeating_timer_t _timerDisplay;

bool drawDisplayIrq(repeating_timer_t *rt)
{
  if (drawDiplay)
    drawDiplay();
  st7567_DrawHLine(10, BLACK);
  st7567_UpdateScreen();
  st7567_Clear();
  printf("updateDisp\n");
  return true;
}

int main()
{
  stdio_init_all();
  st7567_Init();
  buttonHandlerInit();
  // menuSetup();

  add_repeating_timer_us(-1000000, drawDisplayIrq, NULL, &_timerDisplay);

  printf("setup\n");
  while (1)
  {
  }
  return 0;
}