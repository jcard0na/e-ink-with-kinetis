
#include "Display_EPD_W21_spi.h"

void SPI_Delay(unsigned char xrate)
{
	unsigned char i;
	while(xrate)
	{
		for(i=0;i<EPD_W21_SPI_SPEED;i++);
		xrate--;
	}
}


void SPI_Write(unsigned char value)
{
#if USE_BITBANG_SPI
    unsigned char i;


	//SPI_Delay(1);
    for(i=0; i<8; i++)
    {
        EPD_W21_CLK_0;
		//SPI_Delay(1);
        if(value & 0x80)
        	EPD_W21_MOSI_1;
        else
        	EPD_W21_MOSI_0;
        value = (value << 1);
		//SPI_Delay(1);
		//driver_delay_us(1);
        EPD_W21_CLK_1;
        //SPI_Delay(1);
    }
#else
    SPI_WriteBlocking(SPI0, &value, 1);
#endif
}

void EPD_W21_WriteCMD(unsigned char command)
{
    //SPI_Delay(1);
    EPD_W21_CS_0;
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_CS_1;
}
void EPD_W21_WriteDATA(unsigned char command)
{
    //SPI_Delay(1);
    EPD_W21_CS_0;
	EPD_W21_DC_1;		// command write
	SPI_Write(command);
	EPD_W21_CS_1;
}

void EPD_W21_WriteCMD_p1(unsigned char command,unsigned char para)
{
	while(isEPD_W21_BUSY == 1);	// wait

    EPD_W21_CS_0;
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_DC_1;		// command write
	SPI_Write(para);
	EPD_W21_CS_1;
}

void EPD_W21_WriteCMD_p2(unsigned char command,unsigned char para1,unsigned char para2)
{
	while(isEPD_W21_BUSY == 1);	// wait

    EPD_W21_CS_0;
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_DC_1;		// command write
	SPI_Write(para1);
	SPI_Write(para2);
	EPD_W21_CS_1;
}
void EPD_W21_Write(unsigned char *value, unsigned char datalen)
{
	unsigned char i = 0;
	unsigned char *ptemp;

	ptemp = value;
	//DebugInfo("write data or command\n");

    EPD_W21_CS_0;
	EPD_W21_DC_0;		// command write

	SPI_Write(*ptemp);
	ptemp++;

	EPD_W21_DC_1;		// data write

	for(i= 0;i<datalen-1;i++)	// sub the command
	{
		SPI_Write(*ptemp);
		ptemp++;
	}

	EPD_W21_CS_1;

}
void EPD_W21_WriteDispRam(unsigned char XSize,unsigned int YSize,
							unsigned char *Dispbuff)
{
	int i = 0,j = 0;

	if(XSize%8 != 0)
	{
		XSize = XSize+(8-XSize%8);
	}
	XSize = XSize/8;

	while(isEPD_W21_BUSY == 1);	// wait

    EPD_W21_CS_0;
	EPD_W21_DC_0;		// command write
	SPI_Write(0x24);

	EPD_W21_DC_1;		// data write
#if USE_BITBANG_SPI
	for(i=0;i<YSize;i++)
	{
		for(j=0;j<XSize;j++)
		{
			SPI_Write(~(*Dispbuff));
			Dispbuff++;
		}
	}
#else
	// todo fix polarity
	SPI_WriteBlocking(SPI0, Dispbuff, XSize * YSize);
#endif

	EPD_W21_CS_1;
}

void EPD_W21_WriteDispRamMono(unsigned char XSize,unsigned int YSize,
							unsigned char dispdata)
{
	int i = 0,j = 0;

	if(XSize%8 != 0)
	{
		XSize = XSize+(8-XSize%8);
	}
	XSize = XSize/8;
	while(isEPD_W21_BUSY == 1);	// wait

    EPD_W21_CS_0;
	EPD_W21_DC_0;		// command write
	SPI_Write(0x24);

	EPD_W21_DC_1;		// data write
#if USE_BITBANG_SPI
	for(i=0;i<YSize;i++)
	{
		for(j=0;j<XSize;j++)
		{
		 SPI_Write(dispdata);
		}
	}
#else
	for(i=0;i<YSize * XSize;i++)
		SPI_WriteBlocking(SPI0, &dispdata, 1);
#endif

	EPD_W21_CS_1;
}

/***********************************************************
						end file
***********************************************************/
