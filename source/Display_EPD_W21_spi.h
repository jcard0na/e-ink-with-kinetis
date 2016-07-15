#ifndef _MCU_SPI_H_
#define _MCU_SPI_H_

#include "fsl_gpio.h"

#define EPD_W21_SPI_SPEED 0x02

//#define EPD_W21_VPP_0	P4 &= ~0x80
//#define EPD_W21_VPP_1	P4 |= 0x80

#define EPD_W21_MOSI_0	GPIO_WritePinOutput(GPIOD, 6u, 0)
#define EPD_W21_MOSI_1	GPIO_WritePinOutput(GPIOD, 6u, 1)

#define EPD_W21_CLK_0	GPIO_WritePinOutput(GPIOD, 5u, 0)
#define EPD_W21_CLK_1	GPIO_WritePinOutput(GPIOD, 5u, 1)

#define EPD_W21_CS_0	GPIO_WritePinOutput(GPIOD, 4u, 0)
#define EPD_W21_CS_1	GPIO_WritePinOutput(GPIOD, 4u, 1)

#define EPD_W21_DC_0	GPIO_WritePinOutput(GPIOD, 2u, 0)
#define EPD_W21_DC_1	GPIO_WritePinOutput(GPIOE, 2u, 1)

#define EPD_W21_RST_0	GPIO_WritePinOutput(GPIOA, 13u, 0)
#define EPD_W21_RST_1	GPIO_WritePinOutput(GPIOA, 13u, 1)

/* we have this set to ground in hardware */
#define EPD_W21_BS_0 	{}
//#define EPD_W21_BS_1	P2 |= 0x01

#define isEPD_W21_BUSY GPIO_ReadPinInput(GPIOE, 0)


void driver_delay_us(unsigned int xus);
void driver_delay_xms(unsigned long xms);

void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char command);
void EPD_W21_WriteCMD(unsigned char command);
void EPD_W21_WriteCMD_p1(unsigned char command,unsigned char para);
void EPD_W21_WriteCMD_p2(unsigned char command,unsigned char para1,unsigned char para2);
void EPD_W21_Write(unsigned char *value, unsigned char datalen);

void EPD_W21_WriteDispRam(unsigned char XSize,unsigned int YSize,
							unsigned char *Dispbuff);

void EPD_W21_WriteDispRamMono(unsigned char XSize,unsigned int YSize,
							unsigned char dispdata);
#endif  //#ifndef _MCU_SPI_H_

/***********************************************************
						end file
***********************************************************/
