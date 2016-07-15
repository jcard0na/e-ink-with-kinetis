
#ifndef _DISPLAY_EPD_W21_H_
#define _DISPLAY_EPD_W21_H_

#define EPD_W21_WRITE_DATA 1
#define EPD_W21_WRITE_CMD  0
void driver_delay_us(unsigned int xus);
void driver_delay_xms(unsigned long xms);

void EPD_W21_WriteCMD(unsigned char command);
void EPD_W21_POWERON(void);
void EPD_W21_Update(void);
void EPD_W21_EnableChargepump(void);
void EPD_W21_DisableChargepump(void);
void EPD_W21_WirteLUT(unsigned char *LUTvalue);
void EPD_W21_SetRamPointer(unsigned char addrX,unsigned char addrY,unsigned char addrY1);
void EPD_W21_SetRamArea(unsigned char Xstart,unsigned char Xend,
						unsigned char Ystart,unsigned char Ystart1,unsigned char Yend,unsigned char Yend1);
void EPD_W21_Write(unsigned char *value, unsigned char datalen);

void EPD_W21_Update1(void);

void EPD_W21_Init(void);
void EPD_W21_Test_Graphics(unsigned char PicNum);
void EPD_W21_UpdataDisplay(unsigned char *imgbuff,unsigned char xram,unsigned char yram);
void part_display(unsigned char RAM_XST,unsigned char RAM_XEND,unsigned char RAM_YST,unsigned char RAM_YST1,unsigned char RAM_YEND,unsigned char RAM_YEND1);

#endif
/***********************************************************
						end file
***********************************************************/


