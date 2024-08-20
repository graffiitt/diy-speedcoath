#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "hardware/watchdog.h"

#include "display_task/menu_display.h"
#include "display_task/data_display.h"

#include "bluetooth/ble_client.h"
#include "st7567.h"
#include "button_handler/button.h"
#include "gps_ublox/gps.h"
#include "battery.h"

void (*drawDiplay)();
uint8_t selectRow = 0;

// paint display function
void updateDisp()
{
  updateCharge();
  if (drawDiplay)
    drawDiplay();
  st7567_DrawHLine(10, BLACK);
  st7567_UpdateScreen();
  st7567_Clear();
}
void powerHandlerON()
{
  gps_on(true);
  st7567_WriteString(35, 20, "GRAFFIT", FontStyle_veranda_18);
  st7567_UpdateScreen();
  st7567_Clear();
  st7565_backlight(true);

  busy_wait_ms(1000);
  menuSetup();
  updateDisp();

}

void powerHandlerOFF()
{
  st7567_WriteString(35, 20, "OFF", FontStyle_veranda_18);
  st7567_UpdateScreen();
  st7567_Clear();

  setButtonHandlerShort(0, 0);
  setButtonHandlerLong(0, 0);
  setButtonHandlerShort(1, 0);
  setButtonHandlerLong(1, 0);
  setButtonHandlerShort(2, 0);
  setButtonHandlerLong(2, 0);
  setButtonHandlerShort(3, 0);
  setButtonHandlerLong(3, powerHandlerON);
  busy_wait_ms(1000);

  st7565_backlight(false);
  gps_on(false);
}

int main()
{
  timer_hw->dbgpause = 0x2;
  // stdio_init_all();
  start_adc();
  st7567_Init();
  buttonHandlerInit();
  if (watchdog_caused_reboot())
  {
    st7567_WriteString(0, 20, "WATCHDOG", FontStyle_veranda_18);
    st7567_WriteString(0, 36, "REBOOT", FontStyle_veranda_18);

    st7567_UpdateScreen();
  }
  else
  {
    st7567_WriteString(35, 20, "GRAFFIT", FontStyle_veranda_18);
  }
  st7567_UpdateScreen();
  st7567_Clear();

  // on gps power

  gps_init();
  gps_on(true);
  ble_init();
  rtc_init();

  sleep_ms(1000);

  menuSetup();
  updateDisp();
  initDataDisp();

  watchdog_enable(8000, 1);

  while (1)
  {
    uart_async();
    watchdog_update();
  }
  return 0;
}
