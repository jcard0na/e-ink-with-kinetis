/*
 vim: ts=4 sw=4 et:
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

gpio_pin_config_t pin_as_output_low = { kGPIO_DigitalOutput, 0 };
gpio_pin_config_t pin_as_output_high = { kGPIO_DigitalOutput, 1 };
gpio_pin_config_t pin_as_input = { kGPIO_DigitalInput, 0 };

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    /* Initialize LPUART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Affects PORTA_PCR1 register */
    PORT_SetPinMux(PORTA, 1u, kPORT_MuxAlt2);
    /* Affects PORTA_PCR2 register */
    PORT_SetPinMux(PORTA, 2u, kPORT_MuxAlt2);
    /* e-ink reset output, active low */
    PORT_SetPinMux(PORTA, 13u, kPORT_MuxAsGpio);

    /* Initialize pins used for e-ink below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* e-ink D/C output */
    PORT_SetPinMux(PORTD, 2u, kPORT_MuxAsGpio);

/* The example code provided by the vendor has a software spi implementation.
   Let's try that first.  Down the road, we'd probably want to switch to
   the hardware assisted API.  I'm choosing the same pins to make that easy.
 */
#define USE_BITBANG_SPI 1
#if USE_BITBANG_SPI
    /* e-ink spi1_ss */
    PORT_SetPinMux(PORTD, 4u, kPORT_MuxAsGpio);
    /* e-ink spi1_clk */
    PORT_SetPinMux(PORTD, 5u, kPORT_MuxAsGpio);
    /* e-ink spi1_mosi */
    PORT_SetPinMux(PORTD, 6u, kPORT_MuxAsGpio);

    GPIO_PinInit(GPIOD, 4u, &pin_as_output_high);
    GPIO_PinInit(GPIOD, 5u, &pin_as_output_high);
    GPIO_PinInit(GPIOD, 6u, &pin_as_output_high);
#else
    /* e-ink spi1_ss */
    PORT_SetPinMux(PORTD, 4u, kPORT_MuxAlt2);
    /* e-ink spi1_clk */
    PORT_SetPinMux(PORTD, 5u, kPORT_MuxAlt2);
    /* e-ink spi1_mosi */
    PORT_SetPinMux(PORTD, 6u, kPORT_MuxAlt2);
#endif

    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortE);
    /* e-ink busy input */
    PORT_SetPinMux(PORTE, 0u, kPORT_MuxAsGpio);

    GPIO_PinInit(GPIOA, 13u, &pin_as_output_high);
    GPIO_PinInit(GPIOD, 2u, &pin_as_output_low);
    GPIO_PinInit(GPIOE, 0u, &pin_as_input);
}
