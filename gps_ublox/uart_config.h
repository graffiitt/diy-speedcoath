#ifndef UART_CONFIG_H
#define UART_CONFIG_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "pico/async_context_threadsafe_background.h"

#include "hardware/pio.h"
#include "hardware/uart.h"
#include "uart_rx.pio.h"

#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 16
#define UART_RX_PIN 17

#define FIFO_SIZE 80

void uart_configure();

void async_worker_func(async_context_t *async_context, async_when_pending_worker_t *worker);
void pio_irq_func(void);
void async_worker_func(async_context_t *async_context, async_when_pending_worker_t *worker);
bool init_pio(const pio_program_t *program, PIO *pio_hw, uint *sm, uint *offset);

#endif