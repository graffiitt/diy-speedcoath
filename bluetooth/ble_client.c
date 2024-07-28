#include "ble_client.h"

// std::vector<BLE_Item> bleItems;
cvector(struct BLE_Item) bleItems = NULL;

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
    gap_set_scan_parameters(1, 48, 48);
    gap_start_scan();

    hci_event_callback_registration.callback = &pactet_handler_scanner;
    hci_add_event_handler(&hci_event_callback_registration);
}

void ble_scan_stop()
{
    gap_stop_scan();
    hci_event_callback_registration.callback = &pactet_handler_scanner;
    hci_remove_event_handler(&hci_event_callback_registration);
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
        length = gap_event_advertising_report_get_data_length(packet);
        const uint8_t *data = gap_event_advertising_report_get_data(packet);
        printf("Advertisement (legacy) event: addr %s \n", bd_addr_to_str(address));
        dump_advertisement_data(data, length, address);
        break;
    }
}

// get name device and add to vector
void dump_advertisement_data(const uint8_t *adv_data, uint8_t adv_size, uint8_t *address)
{
    ad_context_t context;
    for (ad_iterator_init(&context, adv_size, (uint8_t *)adv_data); ad_iterator_has_more(&context); ad_iterator_next(&context))
    {
        uint8_t data_type = ad_iterator_get_data_type(&context);
        uint8_t size = ad_iterator_get_data_len(&context);
        const uint8_t *data = ad_iterator_get_data(&context);
        if ((data_type == BLUETOOTH_DATA_TYPE_SHORTENED_LOCAL_NAME) || (data_type == BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME))
        {
            device_add(address, (char *)data, size);
            return;
        }
    }
}

void device_add(uint8_t *address, char *name, int size_name)
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

    bd_addr_copy(it.address, address);
    cvector_push_back(bleItems, it);
}
