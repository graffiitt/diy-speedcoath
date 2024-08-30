#include "battery.h"

uint8_t chargeValue = 0;

void start_adc()
{
    adc_init();
    gpio_deinit(27);
    gpio_set_function(27, GPIO_FUNC_SIO);
    gpio_disable_pulls(27);
    gpio_set_input_enabled(27, false);
    adc_select_input(1);
}

void updateCharge()
{
    uint16_t result = adc_read();
    const float voltage = 3.3f / (1 << 12) * result / 0.575f; // 0.6 -- divider volttage 750kom 500kom

    chargeValue = (uint8_t)(((voltage - MIN_VOLTAGE) / DELTA_VOLTAGE) * 100);
}
