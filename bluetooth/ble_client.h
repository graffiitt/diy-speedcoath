#ifndef BLE_CLIENT_H_
#define BLE_CLIENT_H_

#include <string.h>
#include <stdio.h>
#include "../cvector.h"
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#define MAX_LENGHT_NAME 14
struct BLE_Item
{
    char name[MAX_LENGHT_NAME];
    bd_addr_t address;
};

typedef enum
{
    TC_OFF,
    TC_IDLE,
    TC_W4_SCAN_RESULT,
    TC_W4_CONNECT,
    TC_W4_SERVICE_RESULT,
    TC_W4_HEART_RATE_MEASUREMENT_CHARACTERISTIC,
    TC_W4_ENABLE_NOTIFICATIONS_COMPLETE,
    TC_W4_SENSOR_LOCATION_CHARACTERISTIC,
    TC_W4_SENSOR_LOCATION,
    TC_CONNECTED
} gc_state_t;
static gc_state_t state = TC_OFF;

static btstack_packet_callback_registration_t hci_event_callback_registration;

void ble_init();
void ble_scan_start();
void ble_scan_stop();

// for scanning
void pactet_handler_scanner(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);

// private fnc
void dump_advertisement_data(const uint8_t *adv_data, uint8_t adv_size, uint8_t *address);
void device_add(uint8_t *address, char *name, int size_name);

#endif