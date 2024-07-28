#include "uart_config.h"
#include "gps.h"

uint8_t rx_buffer[160];

PIO pio;
uint sm;
int8_t pio_irq;
queue_t fifo;
uint offset;

static async_context_threadsafe_background_t async_context;
static async_when_pending_worker_t worker = {.do_work = async_worker_func};

void uart_configure()
{
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    queue_init(&fifo, 1, FIFO_SIZE);
    if (!async_context_threadsafe_background_init_with_defaults(&async_context))
    {
      //  printf("failed to setup context\n");
    }
    async_context_add_when_pending_worker(&async_context.core, &worker);
    if (!init_pio(&uart_rx_program, &pio, &sm, &offset))
    {
      //  printf("failed to setup pio\n");
    }
    uart_rx_program_init(pio, sm, offset, UART_RX_PIN, BAUD_RATE);

    static_assert(PIO0_IRQ_1 == PIO0_IRQ_0 + 1 && PIO1_IRQ_1 == PIO1_IRQ_0 + 1, "");
    pio_irq = (pio == pio0) ? PIO0_IRQ_0 : PIO1_IRQ_0;
    if (irq_get_exclusive_handler(pio_irq))
    {
        pio_irq++;
        if (irq_get_exclusive_handler(pio_irq))
        {
        //    printf("All IRQs are in use\n");
        }
    }
    irq_add_shared_handler(pio_irq, pio_irq_func, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY); // Add a shared IRQ handler
    irq_set_enabled(pio_irq, true);                                                                // Enable the IRQ
    const uint irq_index = pio_irq - ((pio == pio0) ? PIO0_IRQ_0 : PIO1_IRQ_0);                    // Get index of the IRQ
    pio_set_irqn_source_enabled(pio, irq_index, pis_sm0_rx_fifo_not_empty, true);                  // Set pio to tell us when the FIFO is NOT empty
}

void pio_irq_func(void)
{
    while (!pio_sm_is_rx_fifo_empty(pio, sm))
    {
        char c = uart_rx_program_getc(pio, sm);
        if (!queue_try_add(&fifo, &c))
        {
          //  printf("fifo full\n");
        }
    }
    async_context_set_work_pending(&async_context.core, &worker);
}

void async_worker_func(async_context_t *async_context, async_when_pending_worker_t *worker)
{
    while (!queue_is_empty(&fifo))
    {
        static uint8_t counter = 0;
        char ch;
        if (!queue_try_remove(&fifo, &ch))
        {
         //   printf("fifo empty\n");
        }
        rx_buffer[counter] = ch;
        counter++;

        if (ch == '\n')
        {
          //  printf("len: %d \n", counter);
            counter = 0;
            nmea_parcer(rx_buffer);
            memset(rx_buffer, '\0', 160);
        }
    }
}

bool init_pio(const pio_program_t *program, PIO *pio_hw, uint *sm, uint *offset)
{
    // Find a free pio
    *pio_hw = pio1;
    if (!pio_can_add_program(*pio_hw, program))
    {
        *pio_hw = pio0;
        if (!pio_can_add_program(*pio_hw, program))
        {
            *offset = -1;
            return false;
        }
    }
    *offset = pio_add_program(*pio_hw, program);
    // Find a state machine
    *sm = (int8_t)pio_claim_unused_sm(*pio_hw, false);
    if (*sm < 0)
    {
        return false;
    }
    return true;
}
