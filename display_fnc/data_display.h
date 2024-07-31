#ifndef SETTINGS_DISPLAY_H_
#define SETTINGS_DISPLAY_H_

#include "display_functions.h"
#include "../gps_ublox/gps.h"

extern struct Time time;

void dataDisplaySetup();
void dataDisplayButtonHandler();
void dataDisplayDisplayDraw();

// button handlers
void dataDisplayBackButton();

#endif