#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#define MIN_VOLTAGE 3.2f
#define MAX_VOLTAGE 4.0f
#define DELTA_VOLTAGE (MAX_VOLTAGE-MIN_VOLTAGE)

void start_adc();
uint8_t getCharge();

#endif