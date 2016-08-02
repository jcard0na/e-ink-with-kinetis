
/*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "Ap_29demo.h"	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ErrorStatus HSEStartUpStatus;


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);


//unsigned int IC_SWITCH;
//unsigned char vcom;
void lcd_chkstatus(void);
void lut_red(void);
void lut_bw(void);
void Ultrachip_red(void);
void Ultrachip(void);
void master_uc8154_init(void);
void Ultrachip_red1(void);
void Ultrachip1(void);
void pic_display(void);
unsigned char  EPD_W21_ReadDATA(void);
void driver_delay_us(unsigned int xus)
{
	for(;xus>1;xus--);
}

void driver_delay_xms(unsigned long xms)	
{	
    unsigned long i = 0 , j=0;

    for(j=0;j<xms;j++)
	{
        for(i=0; i<256; i++);
    }
}


	
/*void master_uc8154_init(void)
{
	IC_SWITCH=0Xff;	 
    EPD_W21_WriteCMD(0x10);	       //开始传输黑白图像
	Ultrachip();
	IC_SWITCH=0Xff;	
    EPD_W21_WriteCMD(0x13);		   //开始传输红色图像
	Ultrachip_red();
}*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
#define NVIC_VectTab_FLASH  misc.h
*******************************************************************************/
int main(void)
{
  

#ifdef DEBUG
  debug();
#endif
  // 系统时钟设置
   RCC_Configuration();
  //GPIO参数设置
   GPIO_Configuration();
  
   EPD_W21_Init();
//	k=0;															
	while(1)	   
	{
		EPD_W21_Init();
		//driver_delay_xms(80000);	  //wait for fresh reset
		EPD_W21_WriteCMD(0x01); 
		EPD_W21_WriteDATA(0x07);	    //设置高低电压
        EPD_W21_WriteDATA(0x00);
		EPD_W21_WriteDATA(0x08);
		EPD_W21_WriteDATA(0x00);
 	    EPD_W21_WriteCMD(0x06);         //boost设定
        EPD_W21_WriteDATA(0x07);
        EPD_W21_WriteDATA(0x07);
        EPD_W21_WriteDATA(0x07);
	    EPD_W21_WriteCMD(0x04);         //上电 
        lcd_chkstatus();                //查看芯片状态
		driver_delay_xms(300000);  
		EPD_W21_WriteCMD(0X00);
		EPD_W21_WriteDATA(0xcf);		//选择最大分辨率
	    EPD_W21_WriteCMD(0X50);
		EPD_W21_WriteDATA(0x37);
        EPD_W21_WriteCMD(0x30);			//PLL设定
        EPD_W21_WriteDATA(0x39);		      
        EPD_W21_WriteCMD(0x61);			//像素设定
        EPD_W21_WriteDATA(0xC8);		//200像素	 
        EPD_W21_WriteDATA(0x00);		//200像素 
        EPD_W21_WriteDATA(0xC8);
        EPD_W21_WriteCMD(0x82);			//vcom设定
        EPD_W21_WriteDATA(0x0E);				   
        lut_bw();
		lut_red();
	    pic_display();
        EPD_W21_WriteCMD(0x12);              
        lcd_chkstatus();
		driver_delay_xms(300000);	  //wait for fresh display
		EPD_W21_WriteCMD(0X50);
		EPD_W21_WriteDATA(0x17);		//BD floating
		EPD_W21_WriteCMD(0x82); 		//to solve Vcom drop    
        EPD_W21_WriteDATA(0x00);		
		EPD_W21_WriteCMD(0x01);			//power setting      
        EPD_W21_WriteDATA(0x02);		//gate switch to external
        EPD_W21_WriteDATA(0x00);
        EPD_W21_WriteDATA(0x00); 
        EPD_W21_WriteDATA(0x00); 
		driver_delay_xms(1500);			//delay 1.5S
		EPD_W21_WriteCMD(0X02);			//power off
		driver_delay_xms(40000);
		while(1);
	}
  
}

void pic_display(void)
{
	EPD_W21_WriteCMD(0x10);			//开始传输黑白图像
	Ultrachip(); 
	EPD_W21_WriteCMD(0x13);			//开始传输红图像
	Ultrachip_red();
}

void Ultrachip(void)
{
	unsigned int i;
	for(i=0;i<10000;i++)	     
	{
		EPD_W21_WriteDATA(G_Ultrachip[i]);  
	}  
	driver_delay_xms(2);	              
} 

void Ultrachip_red(void)
{
    unsigned int i;
    for(i=0;i<5000;i++)	     
	{
		EPD_W21_WriteDATA(G_Ultrachip_red[i]);  
	}  
	driver_delay_xms(2);     	     
}


void lut_bw(void)
{
	unsigned int count;
	EPD_W21_WriteCMD(0x20);
	for(count=0;count<15;count++)	     
		{EPD_W21_WriteDATA(lut_vcom0[count]);}
	
	EPD_W21_WriteCMD(0x21);
	for(count=0;count<15;count++)	     
		{EPD_W21_WriteDATA(lut_w[count]);}   
	
	EPD_W21_WriteCMD(0x22);
	for(count=0;count<15;count++)	     
		{EPD_W21_WriteDATA(lut_b[count]);}    
	
	EPD_W21_WriteCMD(0x23);
	for(count=0;count<15;count++)	     
		{EPD_W21_WriteDATA(lut_g1[count]);}    
	
	EPD_W21_WriteCMD(0x24);
	for(count=0;count<15;count++)	     
		{EPD_W21_WriteDATA(lut_g2[count]);}          
}

void lut_red(void)
{
	unsigned int count;
	EPD_W21_WriteCMD(0x25);
	for(count=0;count<15;count++)	     
		{EPD_W21_WriteDATA(lut_vcom1[count]);}
	
	EPD_W21_WriteCMD(0x26);
	for(count=0;count<15;count++)	     
		{EPD_W21_WriteDATA(lut_red0[count]); }  
	
	EPD_W21_WriteCMD(0x27);
	for(count=0;count<15;count++)	     
		{EPD_W21_WriteDATA(lut_red1[count]); }   
}

void lcd_chkstatus(void)
{
	unsigned char busy;
	while(1)
  	{	 
     if(isEPD_W21_BUSY==0) break;
  	}       
   driver_delay_xms(2000);                       
}
/***********************************************************
						end file
***********************************************************/

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
 
  // 复位RCC时钟配置(用于调试模式) 
  RCC_DeInit();
 
  // 使能外部晶振 
  RCC_HSEConfig(RCC_HSE_ON);
  
  // 等待外部晶振稳定 
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    // 设置锁相环频率PLLCLK = 8MHz * 9 = 72 MHz 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
  }
  else {
    // 使能内部晶振 
    RCC_HSICmd(ENABLE);
    // 等待内置振荡器稳定 
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

    // 设置锁相环频率PLLCLK = 8MHz/2 * 16 = 64 MHz 
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16);
  }

    // 使能FLASH的预取缓冲 
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

  //设置代码延时值,FLASH_Latency_2为两个延时周期
  FLASH_SetLatency(FLASH_Latency_2);
	
  //设置系统总线时钟
  RCC_HCLKConfig(RCC_SYSCLK_Div1); 

  //设置高速设备总线时钟，RCC_HCLK_Div1为系统时钟除以1
  RCC_PCLK2Config(RCC_HCLK_Div1); 

  //设置低速设备总线时钟，RCC_HCLK_Div2为系统时钟除以2
  RCC_PCLK1Config(RCC_HCLK_Div2);
  
  //使能锁相环倍频
  RCC_PLLCmd(ENABLE);
  
  // 等待锁相环倍频后的频率稳定 
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  
  // 选择锁相环时钟为系统时钟源 
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  // 等待设置完成 
  while(RCC_GetSYSCLKSource() != 0x08);
    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
            RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,
            ENABLE);

}

/*******************************************************************************
* 函数名   : GPIO_Configuration
* 描述         : 设置设置串口所用GPIO引脚的参数
* 输入         : None
* 输出         : None
* 返回         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10;    //abcd OE ST LT0输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14 |GPIO_Pin_15;    //abcd OE ST LT0输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;    //abcd OE ST LT0输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{ 
  //NVIC_InitTypeDef NVIC_InitStructure;
  ;
}


#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif






