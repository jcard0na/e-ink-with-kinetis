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


void EPD_W21_Init(void)
{
	EPD_W21_BS_0;		// 4 wire spi mode selected

	EPD_W21_RST_0;		// Module reset
	driver_delay_xms(10000);
	EPD_W21_RST_1;
	driver_delay_xms(10000);
	
//	EPD_W21_DispInit();		// pannel configure

//	EPD_W21_WirteLUT(LUTDefault);	// update wavefrom

}

/***********************************************************
						end file
***********************************************************/

