#ifndef MENU_DISPLAY_H
#define MENU_DISPLAY_H

#include "display_functions.h"

extern void (*drawDiplay)();

void menuSetup();

void menuSelectButton();
void menuButtonHandler();
void menuDisplayDraw();

#endif