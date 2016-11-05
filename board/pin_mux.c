/*
 vim: ts=4 sw=4 et:
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

gpio_pin_config_t pin_as_output_low = { kGPIO_DigitalOutput, 0 };
gpio_pin_config_t pin_as_output_high = { kGPIO_DigitalOutput, 1 };
gpio_pin_config_t pin_as_input = { kGPIO_DigitalInput, 0 };

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortE);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);

    /* e-ink reset output, active low */
    PORT_SetPinMux(PORTE, 0u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOE, 0u, &pin_as_output_high);
    /* D/~C */
    PORT_SetPinMux(PORTE, 1u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOE, 1u, &pin_as_output_high);
    /* BUSY */
    PORT_SetPinMux(PORTE, 18u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOE, 18u, &pin_as_input);

    /* e-ink e-ink slave select */
    PORT_SetPinMux(PORTE, 16u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOE, 16u, &pin_as_output_high);
#if USE_BITBANG_SPI
    /* e-ink sclk */
    PORT_SetPinMux(PORTE, 17u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOE, 17u, &pin_as_output_high);
    /* e-ink mosi */
    PORT_SetPinMux(PORTE, 19u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOE, 19u, &pin_as_output_high);
#else
    /* e-ink spi0_clk */
    PORT_SetPinMux(PORTE, 17u, kPORT_MuxAlt2);
    /* e-ink spi0_mosi, Alt 5 (not Alt 2) */
    PORT_SetPinMux(PORTE, 19u, kPORT_MuxAlt5);
#endif

    /* Pin header */
    PORT_SetPinMux(PORTD, 7u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOD, 7u, &pin_as_output_high);
    PORT_SetPinMux(PORTD, 6u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOD, 6u, &pin_as_output_low);
    PORT_SetPinMux(PORTD, 5u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOD, 5u, &pin_as_output_high);
    PORT_SetPinMux(PORTD, 4u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOD, 4u, &pin_as_output_low);
    PORT_SetPinMux(PORTC, 7u, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIOC, 7u, &pin_as_output_high);
}
