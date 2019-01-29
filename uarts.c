#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>

#include "cmd.h"
#include "log.h"
#include "usb_uarts.h"
#include "pinmap.h"
#include "ring.h"
#include "uart_rings.h"


typedef struct
{
    uint32_t              usart;
    enum rcc_periph_clken uart_clk;
    uint32_t              gpioport;
    uint16_t              pins;
    uint8_t               alt_func_num;
    uint8_t               irqn;
    uint32_t              baud;
    uint8_t               priority;
} uart_channel_t;

static const uart_channel_t uart_channels[] = UART_CHANNELS;


#define UART_1_ISR  usart1_isr
#define UART_2_ISR  usart2_isr


static void uart_setup(const uart_channel_t * channel)
{
    rcc_periph_clock_enable(PORT_TO_RCC(channel->gpioport));
    rcc_periph_clock_enable(channel->uart_clk);

    gpio_mode_setup( channel->gpioport, GPIO_MODE_AF, GPIO_PUPD_NONE, channel->pins );
    gpio_set_af( channel->gpioport, channel->alt_func_num, channel->pins );

    usart_set_baudrate( channel->usart, channel->baud );
    usart_set_databits( channel->usart, 8 );
    usart_set_stopbits( channel->usart, USART_CR2_STOPBITS_1 );
    usart_set_mode( channel->usart, USART_MODE_TX_RX );
    usart_set_parity( channel->usart, USART_PARITY_NONE );
    usart_set_flow_control( channel->usart, USART_FLOWCONTROL_NONE );

    nvic_enable_irq(channel->irqn);
    nvic_set_priority(channel->irqn, channel->priority);
    usart_enable(channel->usart);
    usart_enable_rx_interrupt(channel->usart);
}



static bool uart_getc(uint32_t uart, char* c)
{
    uint32_t flags = USART_ISR(uart);

    if (!(flags & USART_ISR_RXNE))
    {
        USART_ICR(uart) = flags;
        return false;
    }

    usart_wait_recv_ready(uart);

    *c = usart_recv(uart);

    return ((*c) != 0);
}



static void process_serial(unsigned uart)
{
    if (uart > 3)
        return;

    char c;

    if (!uart_getc(uart_channels[uart].usart, &c))
    {
        return;
    }

    uart_ring_in(uart, &c, 1);
}


void UART_2_ISR(void)
{
    process_serial(0);
}


void UART_1_ISR(void) { process_serial(1); }

#ifdef STM32F0
void usart3_4_isr(void)
{
    process_serial(2);
    process_serial(3);
}
#else
#error Requires handling for UART 3 and 4.
#endif

void uarts_setup(void)
{
    for(unsigned n = 0; n < 4; n++)
        uart_setup(&uart_channels[n]);
}


void uart_out(unsigned uart, const char* s, unsigned len)
{
    if (uart > 3)
        return;

    uart = uart_channels[uart].usart;

    for(unsigned i = 0; i < len; i++)
        usart_send_blocking(uart, s[i]);
}


bool uart_out_async(unsigned uart, char c)
{
    if (uart > 3)
        return false;

    uart = uart_channels[uart].usart;

    if (!(USART_ISR(uart) & USART_ISR_TXE))
        return false;

    usart_send(uart, c);
    return true;
}
