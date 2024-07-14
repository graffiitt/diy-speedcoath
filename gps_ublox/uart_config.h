#ifndef UART_CONFIG_H
#define UART_CONFIG_H

#include <string.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 16
#define UART_RX_PIN 17

void uart_configure();
void uart_handle();

#endif