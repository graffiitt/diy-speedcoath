#ifndef DATA_DISPLAY_H_
#define DATA_DISPLAY_H_

#define NUM_DATA_ITEMS 2

#include "display_functions.h"

void dataDisplayInit();
void dataDisplaySetup();
void dataDisplayButtonSetup();
void dataDisplayDraw();
void dataDisplayBackButton();

void drawPulseItem(const int x, const int y);
void drawStroceRate(const int x, const int y);

#endif