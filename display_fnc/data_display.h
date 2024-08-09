#ifndef SETTINGS_DISPLAY_H_
#define SETTINGS_DISPLAY_H_

#include "display_functions.h"
#include "../gps_ublox/gps.h"
#include "../bluetooth/ble_client.h"

#define MINIMUM_SPEED 0.01

extern struct BLE_data bleData;
struct DataDisplayValue
{
    double distance;
    struct BLE_data *ble;
};

bool drawDisplayIrq(repeating_timer_t *rt);

void dataDisplaySetup();
void dataDisplayButtonHandler();
void dataDisplayDisplayDraw();

// button handlers
void dataDisplayBackButton();

// draw data items function
void initDataDisp();
void drawPulseItem(const int x, const int y);
void drawStrokeCountItem(const int x, const int y);
void drawStroceRateItem(const int x, const int y);
void drawSplitItem(const int x, const int y);
void drawDistanse(const int x, const int y);

// display for settings display items
void setupDispItemsSetup();
void setupDispItemsDraw();
void setupDispItemsButtonHandler();
void setupDispItemsBackButton();
void setupDispItemsApplyButton();
void setupDispItemsUpButton();
void setupDispItemsChangeUpButton();
void setupDispItemsDownButton();
void setupDispItemsChangeDownButton();
#endif