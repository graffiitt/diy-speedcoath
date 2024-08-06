#include "ble_client.h"

cvector(struct BLE_Item) bleItems = NULL;
gc_state_t stateBLE = TC_OFF;
uint16_t heart_rate = 0;

static hci_con_handle_t connection_handle;
static gatt_client_service_t heart_rate_service;
static gatt_client_characteristic_t body_sensor_location_characteristic;
static gatt_client_characteristic_t heart_rate_measurement_characteristic;

static uint8_t body_sensor_location;

static gatt_client_notification_t notification_listener;
static int listener_registered;

void (*handlerConnection)(void);

void ble_init()
{
    if (cyw43_arch_init())
    {
        printf("failed to initialise cyw43_arch\n");
        return;
    }
    hci_power_control(HCI_POWER_ON);
    l2cap_init();
    gatt_client_init();
    sm_init();
    sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
}

void ble_scan_start()
{
    stateBLE = TC_W4_SCAN_RESULT;
    gap_set_scan_parameters(1, 48, 48);
    gap_start_scan();

    hci_event_callback_registration.callback = &pactet_handler_scanner;
    hci_add_event_handler(&hci_event_callback_registration);
}

void ble_scan_stop()
{
    stateBLE = TC_OFF;
    gap_stop_scan();
    hci_event_callback_registration.callback = &pactet_handler_scanner;
    hci_remove_event_handler(&hci_event_callback_registration);
}

void connectDevice(bd_addr_t *addr, bd_addr_type_t addr_type)
{
    printf("connect to bt\n");
    stateBLE = TC_W4_CONNECT;
    hci_event_callback_registration.callback = &main_hci_event_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    gap_connect(*addr, addr_type);
}

void disconnectDevice()
{
    gap_disconnect(connection_handle);

    // hci_event_callback_registration.callback = &main_hci_event_handler;
    // hci_remove_event_handler(&hci_event_callback_registration);
}

void pactet_handler_scanner(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    UNUSED(channel);
    UNUSED(size);
    if (packet_type != HCI_EVENT_PACKET)
        return;

    uint16_t conn_interval;
    uint8_t event = hci_event_packet_get_type(packet);
    switch (event)
    {
    case GAP_EVENT_ADVERTISING_REPORT:
        bd_addr_t address;
        uint8_t length;

        gap_event_advertising_report_get_address(packet, address);
        bd_addr_type_t le_streamer_addr_type = gap_event_advertising_report_get_address_type(packet);

        length = gap_event_advertising_report_get_data_length(packet);
        const uint8_t *data = gap_event_advertising_report_get_data(packet);
        printf("Advertisement (legacy) event: addr %s \n", bd_addr_to_str(address));
        dump_advertisement_data(data, length, address, le_streamer_addr_type);
        break;
    }
}

void main_hci_event_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    UNUSED(channel);
    UNUSED(size);

    if (packet_type != HCI_EVENT_PACKET)
        return;

    uint16_t conn_interval;
    uint8_t event = hci_event_packet_get_type(packet);
    switch (event)
    {
    case HCI_EVENT_LE_META:
        // wait for connection complete
        if (hci_event_gap_meta_get_subevent_code(packet) != HCI_SUBEVENT_LE_CONNECTION_COMPLETE)
            break;
        if (stateBLE != TC_W4_CONNECT)
            return;
        connection_handle = hci_subevent_le_connection_complete_get_connection_handle(packet);
        // print connection parameters (without using float operations)
        conn_interval = hci_subevent_le_connection_complete_get_conn_interval(packet);
        printf("Connection Interval: %u.%02u ms\n", conn_interval * 125 / 100, 25 * (conn_interval & 3));
        printf("Connection Latency: %u\n", hci_subevent_le_connection_complete_get_conn_latency(packet));
        // initialize gatt client context with handle, and add it to the list of active clients
        // query primary services
        printf("Search for Heart Rate service.\n");
        stateBLE = TC_W4_SERVICE_RESULT;
        gatt_client_discover_primary_services_by_uuid16(handle_gatt_client_event, connection_handle, ORG_BLUETOOTH_SERVICE_HEART_RATE);
        break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
        // unregister listener
        connection_handle = HCI_CON_HANDLE_INVALID;
        if (listener_registered)
        {
            listener_registered = 0;
            gatt_client_stop_listening_for_characteristic_value_updates(&notification_listener);
        }
        stateBLE = TC_OFF;
        if (handlerConnection)
            handlerConnection();
        printf("Disconnected  ble\n");
        if (stateBLE == TC_OFF)
            break;
        // gatt_heart_rate_client_start();
        break;
    default:
        break;
    }
}

static void handle_gatt_client_event(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    UNUSED(packet_type);
    UNUSED(channel);
    UNUSED(size);

    uint8_t sensor_contact;
    uint8_t att_status;

    switch (stateBLE)
    {
    case TC_W4_SERVICE_RESULT:
        switch (hci_event_packet_get_type(packet))
        {
        case GATT_EVENT_SERVICE_QUERY_RESULT:
            // store service (we expect only one)
            gatt_event_service_query_result_get_service(packet, &heart_rate_service);
            break;
        case GATT_EVENT_QUERY_COMPLETE:
            att_status = gatt_event_query_complete_get_att_status(packet);
            if (att_status != ATT_ERROR_SUCCESS)
            {
                printf("SERVICE_QUERY_RESULT - ATT Error 0x%02x.\n", att_status);
                gap_disconnect(connection_handle);
                break;
            }
            stateBLE = TC_W4_HEART_RATE_MEASUREMENT_CHARACTERISTIC;
            printf("Search for Heart Rate Measurement characteristic.\n");
            gatt_client_discover_characteristics_for_service_by_uuid16(handle_gatt_client_event, connection_handle, &heart_rate_service, ORG_BLUETOOTH_CHARACTERISTIC_HEART_RATE_MEASUREMENT);
            break;
        default:
            break;
        }
        break;

    case TC_W4_HEART_RATE_MEASUREMENT_CHARACTERISTIC:
        switch (hci_event_packet_get_type(packet))
        {
        case GATT_EVENT_CHARACTERISTIC_QUERY_RESULT:
            gatt_event_characteristic_query_result_get_characteristic(packet, &heart_rate_measurement_characteristic);
            break;
        case GATT_EVENT_QUERY_COMPLETE:
            att_status = gatt_event_query_complete_get_att_status(packet);
            if (att_status != ATT_ERROR_SUCCESS)
            {
                printf("CHARACTERISTIC_QUERY_RESULT - ATT Error 0x%02x.\n", att_status);
                gap_disconnect(connection_handle);
                break;
            }
            // register handler for notifications
            listener_registered = 1;
            gatt_client_listen_for_characteristic_value_updates(&notification_listener, handle_gatt_client_event, connection_handle, &heart_rate_measurement_characteristic);
            // enable notifications
            printf("Enable Notify on Heart Rate Measurements characteristic.\n");
            stateBLE = TC_W4_ENABLE_NOTIFICATIONS_COMPLETE;
            gatt_client_write_client_characteristic_configuration(handle_gatt_client_event, connection_handle,
                                                                  &heart_rate_measurement_characteristic, GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION);
            break;
        default:
            break;
        }
        break;

    case TC_W4_ENABLE_NOTIFICATIONS_COMPLETE:
        switch (hci_event_packet_get_type(packet))
        {
        case GATT_EVENT_QUERY_COMPLETE:
            printf("Notifications enabled, ATT status 0x%02x\n", gatt_event_query_complete_get_att_status(packet));

            stateBLE = TC_W4_SENSOR_LOCATION_CHARACTERISTIC;
            printf("Search for Sensor Location characteristic.\n");
            gatt_client_discover_characteristics_for_service_by_uuid16(handle_gatt_client_event, connection_handle, &heart_rate_service, ORG_BLUETOOTH_CHARACTERISTIC_BODY_SENSOR_LOCATION);
            break;
        default:
            break;
        }
        break;

    case TC_W4_SENSOR_LOCATION_CHARACTERISTIC:
        switch (hci_event_packet_get_type(packet))
        {
        case GATT_EVENT_CHARACTERISTIC_QUERY_RESULT:
            gatt_event_characteristic_query_result_get_characteristic(packet, &body_sensor_location_characteristic);
            break;
        case GATT_EVENT_QUERY_COMPLETE:
            if (gatt_event_query_complete_get_att_status(packet) != ATT_ERROR_SUCCESS)
            {
                printf("CHARACTERISTIC_QUERY_RESULT - ATT Error 0x%02x.\n", packet[4]);
                stateBLE = TC_CONNECTED;
                if (handlerConnection)
                    handlerConnection();
                
                break;
            }
            if (body_sensor_location_characteristic.value_handle == 0)
            {
                printf("Sensor Location characteristic not available.\n");
                stateBLE = TC_CONNECTED;
                if (handlerConnection)
                    handlerConnection();
                
                break;
            }
            stateBLE = TC_W4_HEART_RATE_MEASUREMENT_CHARACTERISTIC;
            printf("Read Body Sensor Location.\n");
            stateBLE = TC_W4_SENSOR_LOCATION;
            gatt_client_read_value_of_characteristic(handle_gatt_client_event, connection_handle, &body_sensor_location_characteristic);
            break;
        default:
            break;
        }
        break;

    case TC_W4_SENSOR_LOCATION:
        switch (hci_event_packet_get_type(packet))
        {
        case GATT_EVENT_CHARACTERISTIC_VALUE_QUERY_RESULT:
            body_sensor_location = gatt_event_characteristic_value_query_result_get_value(packet)[0];
            printf("Sensor Location: %u\n", body_sensor_location);
            break;
        case GATT_EVENT_QUERY_COMPLETE:
            stateBLE = TC_CONNECTED;
            if (handlerConnection)
                handlerConnection();
            
            break;
        default:
            break;
        }
        break;

    case TC_CONNECTED:
        switch (hci_event_packet_get_type(packet))
        {
        case GATT_EVENT_NOTIFICATION:
            if (gatt_event_notification_get_value(packet)[0] & 1)
            {
                heart_rate = little_endian_read_16(gatt_event_notification_get_value(packet), 1);
            }
            else
            {
                heart_rate = gatt_event_notification_get_value(packet)[1];
            }
            break;
        case GATT_EVENT_QUERY_COMPLETE:
            break;
        case GATT_EVENT_CAN_WRITE_WITHOUT_RESPONSE:
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
}

// get name device and add to vector
void dump_advertisement_data(const uint8_t *adv_data, uint8_t adv_size, uint8_t *address, bd_addr_type_t addr_type)
{
    ad_context_t context;
    for (ad_iterator_init(&context, adv_size, (uint8_t *)adv_data); ad_iterator_has_more(&context); ad_iterator_next(&context))
    {
        uint8_t data_type = ad_iterator_get_data_type(&context);
        uint8_t size = ad_iterator_get_data_len(&context);
        const uint8_t *data = ad_iterator_get_data(&context);
        if ((data_type == BLUETOOTH_DATA_TYPE_SHORTENED_LOCAL_NAME) || (data_type == BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME))
        {
            device_add(address, (char *)data, size, addr_type);
            return;
        }
    }
}

void device_add(uint8_t *address, char *name, int size_name, bd_addr_type_t addr_type)
{
    for (int i = 0; i < cvector_size(bleItems); i++)
    {
        if (bd_addr_cmp(cvector_at(bleItems, i)->address, address) == 0)
            return;
    }
    struct BLE_Item it;
    if (size_name > MAX_LENGHT_NAME)
    {
        size_name = MAX_LENGHT_NAME - 1;
    }
    for (uint8_t i = 0; i < size_name; i++)
    {
        it.name[i] = name[i];
    }
    it.name[size_name] = '\0';
    it.addr_type = addr_type;
    bd_addr_copy(it.address, address);
    cvector_push_back(bleItems, it);
}

void ble_clearDevices()
{
    for (int i = cvector_size(bleItems); 0 < i; i--)
    {
        cvector_pop_back(bleItems);
    }
}