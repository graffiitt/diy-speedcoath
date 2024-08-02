#include "hardware/timer.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"

#include "display_fnc/menu_display.h"
#include "display_fnc/data_display.h"

#include "st7565/st7567.h"
#include "button_handler/button.h"
#include "gps_ublox/gps.h"

void (*drawDiplay)();
uint8_t selectRow = 0;

// paint display function
void updateDisp()
{
  if (drawDiplay)
    drawDiplay();
  st7567_DrawHLine(10, BLACK);
  st7567_UpdateScreen();
  st7567_Clear();
}

int main()
{

  timer_hw->dbgpause = 0x2;
  stdio_init_all();

  st7567_Init();
  buttonHandlerInit();

  st7567_WriteString(35, 20, "GRAFFIT", FontStyle_veranda_18);

  st7567_UpdateScreen();
  st7567_Clear();

  gps_init();
  sleep_ms(1000);

  menuSetup();
  updateDisp();
  initDataDisp();

  gpio_deinit(5);
  gpio_init(5);
  gpio_set_dir(5, GPIO_OUT);
  gpio_put(5, 1);

  //add_repeating_timer_us(-1000000, drawDisplayIrq, NULL, &_timerDisplay);

  while (1)
  {
  }
  return 0;
}
