#ifndef BLE_DISPLAY_H_
#define BLE_DISPLAY_H_

#include "display_functions.h"
#include "../bluetooth/ble_client.h"

void bleSettingsSetup();
void bleSettingsButtonHandler();
void bleSettingsDisplayDraw();
void drawListBLE(cvector(struct BLE_Item) *items);

// button handlers
void bleSettingsConnectButton();
void bleSettingsDisconnectButton();
void bleSettingsUpdate();
bool bleScanTimer(repeating_timer_t *rt);
void bleSettingsBackButton();

#endif