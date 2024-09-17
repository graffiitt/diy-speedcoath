#ifndef SETTINGS_DISPLAY_H_
#define SETTINGS_DISPLAY_H_

#include <stdio.h>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#include "display_functions.h"
#include "gps.h"
#include "../bluetooth/ble_client.h"

#define MINIMUM_SPEED 0.01

struct DataDisplay
{
    double distance;
    uint8_t *statusGPS;
};

void dataDisplaySetup();
void initDataDisp();

#endif