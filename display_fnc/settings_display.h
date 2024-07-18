#ifndef SETTINGS_DISPLAY_H_
#define SETTINGS_DISPLAY_H_

#include "display_functions.h"
#include "setup_display_data.h"

void settingsSetup();
void settingsDrawDisplay();
void settingsClearVector();

// button handlers
void settingsButtonsSetup();
void settingsBackButton();
void settingsSelectButton();

#endif