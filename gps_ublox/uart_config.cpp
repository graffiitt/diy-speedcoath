#include "uart_config.h"
#include "gps.h"

uint8_t rx_buffer[80];

void uart_configure()
{
    uart_init(UART_ID, 9600);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);
    uart_set_hw_flow(UART_ID, false, false);

    uart_set_fifo_enabled(UART_ID, false);

    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, uart_handle);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
}

void uart_handle()
{
    static uint8_t count = 0;
    while (uart_is_readable(UART_ID))
    {
        uint8_t ch = uart_getc(UART_ID);
        if (ch == '$')
        {
            count = 0;
            nmea_parcer(rx_buffer);
            memset(rx_buffer, '\0', 80);
        }
        rx_buffer[count] = ch;
        count++;
    }
}
