#ifndef __PINMAPS__
#define __PINMAPS__

#include <libopencm3/stm32/rcc.h>
#include <stdint.h>

typedef struct
{
    uint32_t port;
    uint32_t pins;
} port_n_pins_t;

#define PORT_TO_RCC(_port_)   (RCC_GPIOA + ((_port_ - GPIO_PORT_A_BASE) / 0x400))

#define LED_PORT   GPIOA
#define LED_PIN    GPIO5

#define ADCS_PORT_N_PINS                            \
{                                                   \
    {GPIOA, GPIO4},      /* ADC 0  = Channel 4  */  \
    {GPIOA, GPIO6},      /* ADC 1  = Channel 6  */  \
    {GPIOA, GPIO7},      /* ADC 2  = Channel 7  */  \
    {GPIOB, GPIO0},      /* ADC 3  = Channel 8  */  \
    {GPIOB, GPIO1},      /* ADC 4  = Channel 9  */  \
    {GPIOC, GPIO0},      /* ADC 5  = Channel 10 */  \
    {GPIOC, GPIO1},      /* ADC 6  = Channel 11 */  \
    {GPIOC, GPIO3},      /* ADC 7  = Channel 13 */  \
    {GPIOC, GPIO4},      /* ADC 8  = Channel 14 */  \
    {GPIOC, GPIO5},      /* ADC 9  = Channel 15 */  \
}

#define ADC_CHANNELS  {4,6,7,8,9,10,11,13,14,15}

#define ADC_COUNT 10

#define PPS_PORT_N_PINS             \
{                                   \
    {GPIOB, GPIO3},     /* PPS 0 */ \
    {GPIOC, GPIO7},     /* PPS 1 */ \
}


#define PPS_EXTI      \
{                     \
    {TIM1, EXTI3},    \
    {TIM14, EXTI7},   \
}

#define PPS_INIT                     \
{                                    \
    {RCC_TIM1, NVIC_EXTI2_3_IRQ },   \
    {RCC_TIM14, NVIC_EXTI4_15_IRQ},  \
}

#define PPS0_EXTI_ISR        exti2_3_isr
#define PPS1_EXTI_ISR        exti4_15_isr

/*subset of usb_cdc_line_coding_bParityType*/
typedef enum
{
    uart_parity_none = 0,
    uart_parity_odd  = 1,
    uart_parity_even = 2,
} uart_parity_t;

/*matches usb_cdc_line_coding_bCharFormat*/
typedef enum
{
    uart_stop_bits_1   = 0,
    uart_stop_bits_1_5 = 1,
    uart_stop_bits_2   = 2,
} uart_stop_bits_t;

typedef struct
{
    uint32_t              usart;
    enum rcc_periph_clken uart_clk;
    uint32_t              baud;
    uint8_t               databits:4;
    uint8_t               parity:2 /*uart_parity_t*/;
    uint8_t               stop:2 /*uart_stop_bits_t*/;
    uint32_t              gpioport;
    uint16_t              pins;
    uint8_t               alt_func_num;
    uint8_t               irqn;
    uint32_t              dma_addr;
    uint8_t               dma_irqn;
    uint8_t               dma_channel;
    uint8_t               priority;
    uint8_t               enabled;
} uart_channel_t;

#define UART_CHANNELS                                                                                                   \
{                                                                                                                       \
    { USART2, RCC_USART2, UART_2_SPEED, UART_2_DATABITS, UART_2_PARITY, UART_2_STOP, GPIOA, GPIO2  | GPIO3,  GPIO_AF1, NVIC_USART2_IRQ,   (uint32_t)&USART2_TDR, NVIC_DMA1_CHANNEL4_7_DMA2_CHANNEL3_5_IRQ, DMA_CHANNEL4, UART2_PRIORITY, 0 }, /* UART 0 */ \
    { USART3, RCC_USART3, UART_3_SPEED, UART_3_DATABITS, UART_3_PARITY, UART_3_STOP, GPIOC, GPIO10 | GPIO11, GPIO_AF1, NVIC_USART3_4_IRQ, (uint32_t)&USART3_TDR, NVIC_DMA1_CHANNEL2_3_DMA2_CHANNEL1_2_IRQ, DMA_CHANNEL2, UART3_PRIORITY, 0 }, /* UART 1 */ \
    { USART4, RCC_USART4, UART_4_SPEED, UART_4_DATABITS, UART_4_PARITY, UART_4_STOP, GPIOA, GPIO1  | GPIO0,  GPIO_AF4, NVIC_USART3_4_IRQ, (uint32_t)&USART4_TDR, NVIC_DMA1_CHANNEL4_7_DMA2_CHANNEL3_5_IRQ, DMA_CHANNEL7, UART4_PRIORITY, 0 }, /* UART 2 */ \
}

#define UART_CHANNELS_COUNT 3
#define UART_DEBUG USART2

/*
 * NOTE: A13 and A14 will cause SWD problems.
 */
#define IOS_PORT_N_PINS            \
{                                  \
    {GPIOC, GPIO12},   /* IO 0  */ \
    {GPIOA, GPIO15},   /* IO 1  */ \
    {GPIOB, GPIO7},    /* IO 2  */ \
    {GPIOA, GPIO8},    /* IO 3  */ \
    {GPIOB, GPIO10},   /* IO 4  */ \
    {GPIOB, GPIO4},    /* IO 5  */ \
    {GPIOD, GPIO2},    /* IO 6  */ \
    {GPIOC, GPIO2},    /* IO 7  */ \
    {GPIOB, GPIO13},   /* IO 8  */ \
    {GPIOC, GPIO9},    /* IO 9  */ \
    {GPIOC, GPIO8},    /* IO 10 */ \
    {GPIOC, GPIO6},    /* IO 11 */ \
    {GPIOB, GPIO12},   /* IO 12 */ \
    {GPIOB, GPIO11},   /* IO 13 */ \
    {GPIOB, GPIO2},    /* IO 14 */ \
    {GPIOB, GPIO6},    /* IO 15 */ \
    {GPIOB, GPIO14},   /* IO 16 */ \
    {GPIOB, GPIO15},   /* IO 17 */ \
    {GPIOA, GPIO9},    /* IO 18 */ \
    {GPIOA, GPIO10},   /* IO 19 */ \
    {GPIOA, GPIO13},   /* IO 20 */ \
    {GPIOA, GPIO14},   /* IO 21 */ \
    {GPIOC, GPIO13},   /* IO 22 */ \
    {GPIOB, GPIO5},    /* IO 23 */ \
    {GPIOB, GPIO8},    /* IO 24 */ \
    {GPIOB, GPIO9},    /* IO 25 */ \
    {GPIOB, GPIO3},    /* IO 26 - PPS 0 */ \
    {GPIOC, GPIO7},    /* IO 27 - PPS 1 */ \
    {GPIOC, GPIO10},   /* IO 28 - UART_0_RX */ \
    {GPIOC, GPIO11},   /* IO 29 - UART_0_TX */ \
    {GPIOA, GPIO0},    /* IO 30 - UART_1_RX  */ \
    {GPIOA, GPIO1},    /* IO 31 - UART_1_TX */ \
}


#define IO_AS_INPUT     0x0100
#define IO_DIR_LOCKED   0x0200
#define IO_SPECIAL_EN   0x0400
#define IO_UART_TX      0x0800
#define IO_RELAY        0x1000
#define IO_HIGHSIDE     0x2000
#define IO_PPS0         0x3000
#define IO_PPS1         0x4000
#define IO_UART0        0x5000
#define IO_UART1        0x6000
#define IO_TYPE_MASK    0xF000
#define IO_PULL_MASK    0x0003

#define IOS_STATE                                                            \
{                                                                            \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 0   */         \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 1   */         \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 2   */         \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 3   */         \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 4   */         \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 5   */         \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 6   */         \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 7   */         \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 8   */         \
    IO_AS_INPUT | GPIO_PUPD_PULLUP,                   /* GPIO 9   */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_RELAY,    /* GPIO 10  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_RELAY,    /* GPIO 11  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_RELAY,    /* GPIO 12  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_RELAY,    /* GPIO 13  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_RELAY,    /* GPIO 14  */         \
    GPIO_PUPD_PULLUP,                                 /* GPIO 15  */         \
    GPIO_PUPD_PULLUP,                                 /* GPIO 16  */         \
    GPIO_PUPD_PULLUP,                                 /* GPIO 17  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_HIGHSIDE, /* GPIO 18  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_HIGHSIDE, /* GPIO 19  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_HIGHSIDE, /* GPIO 20  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_HIGHSIDE, /* GPIO 21  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_HIGHSIDE, /* GPIO 22  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_HIGHSIDE, /* GPIO 23  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_HIGHSIDE, /* GPIO 24  */         \
    IO_DIR_LOCKED | GPIO_PUPD_PULLDOWN | IO_HIGHSIDE, /* GPIO 25  */         \
    IO_AS_INPUT | IO_PPS0 | IO_SPECIAL_EN,            /* GPIO 26 - PPS 0 */     \
    IO_AS_INPUT | IO_PPS1 | IO_SPECIAL_EN,            /* GPIO 27 - PPS 1 */     \
    IO_UART0 | IO_SPECIAL_EN,                         /* GPIO 28 - UART_0_RX */ \
    IO_UART0 | IO_UART_TX | IO_SPECIAL_EN,            /* GPIO 29 - UART_0_TX */ \
    IO_UART1 | IO_SPECIAL_EN,                         /* GPIO 30 - UART_1_RX */ \
    IO_UART1 | IO_UART_TX | IO_SPECIAL_EN,            /* GPIO 31 - UART_1_TX */ \
}

#define PPS0_IO_NUM          26
#define PPS1_IO_NUM          27

#endif //__PINMAPS__
