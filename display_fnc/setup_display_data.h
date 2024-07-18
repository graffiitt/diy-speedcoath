#ifndef SETUP_DATA_DISP_H_
#define SETUP_DATA_DISP_H_

#include "display_functions.h"

extern struct ItemObjectData dataItems[];
extern int8_t dataItemsSelect[];

// setup
void setupDataDispSetup();
void setupDataDispDrawDisplay();
void setupDataDispButtonsSetup();

// button handlers
void setupDataDispBackButton();
void setupDataDispUpButton();
void setupDataDispDownButton();
void setupDataDispChangeUpButton();
void setupDataDispChangeDownButton();
void setupDataDispApplyBotton();

#endif