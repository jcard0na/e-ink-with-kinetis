/***********************************************************
Copyright(C), Yk Technology
FileName	:
Author		: Yukewantong, Zhongxiaodong
Date		  	: 2011/12/22
Description	:
Version		: V1.0
History		:
--------------------------------
2011/12/22: created
2012/02/29: update the pvi's waveform(20C) as default
2012/03/18: update the EPD initinal step, add function-->
			void EPD_W21_PowerOnInit(void)
***********************************************************/
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"



unsigned char RamAreaX[3];	// X start and end
unsigned char RamAreaY[5]; 	// Y start and end
unsigned char RamPointerX[2];	// default (0,0)
unsigned char RamPointerY[3];

unsigned char DispBuff[32];	// Display buffer
unsigned char LUT[30];	// LUT buffer

unsigned char GDOControl[]={0x01,0xC7,0x00,0x00}; //for 1.54inch
unsigned char softstart[]={0x0c,0xd7,0xd6,0x9d};
unsigned char Rambypass[] = {0x21,0x8f};		// Display update
unsigned char MAsequency[] = {0x22,0xf0};		// clock
unsigned char GDVol[] = {0x03,0x00};	// Gate voltage +15V/-15V
unsigned char SDVol[] = {0x04,0x0a};	// Source voltage +15V/-15V
unsigned char VCOMVol[] = {0x2c,0xa8};	// VCOM 7c
unsigned char BOOSTERFB[] = {0xf0,0x1f};	// Source voltage +15V/-15V
unsigned char DummyLine[] = {0x3a,0x1a};	// 4 dummy line per gate
unsigned char Gatetime[] = {0x3b,0x08};	// 2us per line
unsigned char BorderWavefrom[] = {0x3c,0x33};	// Border
unsigned char RamDataEntryMode[] = {0x11,0x03};	// Ram data entry mode



void part_display(unsigned char RAM_XST,unsigned char RAM_XEND,unsigned char RAM_YST,unsigned char RAM_YST1,unsigned char RAM_YEND,unsigned char RAM_YEND1)
 {    EPD_W21_SetRamArea(RAM_XST,RAM_XEND,RAM_YST,RAM_YST1,RAM_YEND,RAM_YEND1);  	/*set w h*/
      EPD_W21_SetRamPointer (RAM_XST,RAM_YST,RAM_YST1);		 /*set orginal*/
 }
void EPD_W21_UpdataDisplay(unsigned char *imgbuff,unsigned char xram,unsigned char yram)
{
	EPD_W21_WriteDispRam(xram, yram, imgbuff);
//	EPD_W21_Update();
}
void EPD_W21_SetRamArea(unsigned char Xstart,unsigned char Xend,
						unsigned char Ystart,unsigned char Ystart1,unsigned char Yend,unsigned char Yend1)
{
	RamAreaX[0] = 0x44;	// command
	RamAreaX[1] = Xstart;
	RamAreaX[2] = Xend;
	RamAreaY[0] = 0x45;	// command
	RamAreaY[1] = Ystart;
	RamAreaY[2] = Ystart1;
	RamAreaY[3] = Yend;
    RamAreaY[4] = Yend1;
	EPD_W21_Write(RamAreaX, sizeof(RamAreaX));
	EPD_W21_Write(RamAreaY, sizeof(RamAreaY));
}
void EPD_W21_SetRamPointer(unsigned char addrX,unsigned char addrY,unsigned char addrY1)
{
	RamPointerX[0] = 0x4e;
	RamPointerX[1] = addrX;
	RamPointerY[0] = 0x4f;
	RamPointerY[1] = addrY;
	RamPointerY[2] = addrY1;

	EPD_W21_Write(RamPointerX, sizeof(RamPointerX));
	EPD_W21_Write(RamPointerY, sizeof(RamPointerY));
}
//=========================functions============================

//-------------------------------------------------------
//Func	: void EPD_W21_DispInit(void)
//Desc	: display parameters initinal
//Input	: none
//Output: none
//Return: none
//Author:
//Date	: 2011/12/24
//-------------------------------------------------------
void EPD_W21_DispInit(void)
{
	EPD_W21_Write(GDOControl, sizeof(GDOControl));	// Pannel configuration, Gate selection
    EPD_W21_Write(softstart, sizeof(softstart));	// voodoo
	EPD_W21_Write(VCOMVol, sizeof(VCOMVol));		// VCOM setting
	EPD_W21_Write(DummyLine, sizeof(DummyLine));	// dummy line per gate
	EPD_W21_Write(Gatetime, sizeof(Gatetime));		// Gage time setting
	EPD_W21_Write(RamDataEntryMode, sizeof(RamDataEntryMode));	// X increase, Y increase
	EPD_W21_SetRamArea(0x00,0x18,0x00,0x00,0xc7,0x00);	// X-source area,Y-gage area
    EPD_W21_SetRamPointer(0x00,0x00,0x00);	// set ram
}

static void EPD_SPI_Init()
{
#if !USE_BITBANG_SPI
#error
  	uint32_t sourceClock;
	spi_master_config_t masterConfig = {0};
	// Configure SPI Hardware
	SPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.outputMode = kSPI_SlaveSelectAsGpio;
	sourceClock = CLOCK_GetFreq(kCLOCK_BusClk);
	SPI_MasterInit(SPI0, &masterConfig, sourceClock);
	SPI_EnableFIFO(SPI0, false);
#endif
}

void EPD_W21_Init(void)
{

	EPD_W21_BS_0;		// 4 wire spi mode selected

	EPD_SPI_Init();

	//driver_delay_xms(100);
	EPD_W21_RST_1;
	//driver_delay_xms(100);

	EPD_W21_DispInit();		// pannel configure
}



//-------------------------------------------------------
//Func	: EPD_W21_EnableChargepump
//Desc	:
//Input	:
//Output:
//Return:
//Author:
//Date	: 2011/12/24
//-------------------------------------------------------
void EPD_W21_EnableChargepump(void)
{
	EPD_W21_WriteCMD_p1(0xf0,0x8f);
	EPD_W21_WriteCMD_p1(0x22,0xc0);
	EPD_W21_WriteCMD(0x20);
	EPD_W21_WriteCMD(0xff);
}

//-------------------------------------------------------
//Func	: EPD_W21_DisableChargepump
//Desc	:
//Input	:
//Output:
//Return:
//Author:
//Date	: 2011/12/24
//-------------------------------------------------------
void EPD_W21_DisableChargepump(void)
{
	EPD_W21_WriteCMD_p1(0x22,0xf0);
	EPD_W21_WriteCMD(0x20);
	EPD_W21_WriteCMD(0xff);
}

//-------------------------------------------------------
//Func	: EPD_W21_Update
//Desc	:
//Input	:
//Output:
//Return:
//Author:
//Date	: 2011/12/24
//-------------------------------------------------------
void EPD_W21_Update(void)
{
	EPD_W21_WriteCMD_p1(0x22,0xc7);
	EPD_W21_WriteCMD(0x20);
	EPD_W21_WriteCMD(0xff);
}

 void EPD_W21_Update1(void)
{
	EPD_W21_WriteCMD_p1(0x22,0x04);
	//EPD_W21_WriteCMD_p1(0x22,0x08);
	EPD_W21_WriteCMD(0x20);
	EPD_W21_WriteCMD(0xff);
}

void EPD_W21_WriteRAM(void)
{
	EPD_W21_WriteCMD(0x24);
}




//-------------------------------------------------------
//Func	: EPD_W21_WirteLUT(unsigned char *LUTvalue)
//Desc	: write the waveform to the dirver's ram
//Input	: *LUTvalue, the wavefrom tabe address
//Output: none
//Return: none
//Author:
//Date	: 2011/12/24
//-------------------------------------------------------
void EPD_W21_WirteLUT(unsigned char *LUTvalue)
{
	EPD_W21_Write(LUTvalue, 31);
}



//-------------------------------------------------------
//Func	: EPD_W21_UpdataDisplay
//Desc	: updata the display
//Input	:
//Output:
//Return:
//Author:
//Date	: 2012/3/13
//-------------------------------------------------------
/*
void EPD_W21_UpdataDisplay(unsigned char *imgbuff)
{
	EPD_W21_WriteDispRam(128, 296, imgbuff);
	//EPD_W21_WriteRAM();
	EPD_W21_Update();
}
*/
 void EPD_W21_POWERON(void)
{
	EPD_W21_WriteCMD_p1(0x22,0xc0);
	EPD_W21_WriteCMD(0x20);
	//EPD_W21_WriteCMD(0xff);
}
void EPD_W21_POWEROFF(void)
{  	EPD_W21_WriteCMD_p1(0x22,0xc3);
	EPD_W21_WriteCMD(0x20);
//	EPD_W21_WriteCMD(0xff);
}
/***********************************************************
						end file
***********************************************************/

