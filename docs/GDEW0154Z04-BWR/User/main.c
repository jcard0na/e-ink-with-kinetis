
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
    EPD_W21_WriteCMD(0x10);	       //��ʼ����ڰ�ͼ��
	Ultrachip();
	IC_SWITCH=0Xff;	
    EPD_W21_WriteCMD(0x13);		   //��ʼ�����ɫͼ��
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
  // ϵͳʱ������
   RCC_Configuration();
  //GPIO��������
   GPIO_Configuration();
  
   EPD_W21_Init();
//	k=0;															
	while(1)	   
	{
		EPD_W21_Init();
		//driver_delay_xms(80000);	  //wait for fresh reset
		EPD_W21_WriteCMD(0x01); 
		EPD_W21_WriteDATA(0x07);	    //���øߵ͵�ѹ
        EPD_W21_WriteDATA(0x00);
		EPD_W21_WriteDATA(0x08);
		EPD_W21_WriteDATA(0x00);
 	    EPD_W21_WriteCMD(0x06);         //boost�趨
        EPD_W21_WriteDATA(0x07);
        EPD_W21_WriteDATA(0x07);
        EPD_W21_WriteDATA(0x07);
	    EPD_W21_WriteCMD(0x04);         //�ϵ� 
        lcd_chkstatus();                //�鿴оƬ״̬
		driver_delay_xms(300000);  
		EPD_W21_WriteCMD(0X00);
		EPD_W21_WriteDATA(0xcf);		//ѡ�����ֱ���
	    EPD_W21_WriteCMD(0X50);
		EPD_W21_WriteDATA(0x37);
        EPD_W21_WriteCMD(0x30);			//PLL�趨
        EPD_W21_WriteDATA(0x39);		      
        EPD_W21_WriteCMD(0x61);			//�����趨
        EPD_W21_WriteDATA(0xC8);		//200����	 
        EPD_W21_WriteDATA(0x00);		//200���� 
        EPD_W21_WriteDATA(0xC8);
        EPD_W21_WriteCMD(0x82);			//vcom�趨
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
	EPD_W21_WriteCMD(0x10);			//��ʼ����ڰ�ͼ��
	Ultrachip(); 
	EPD_W21_WriteCMD(0x13);			//��ʼ�����ͼ��
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
 
  // ��λRCCʱ������(���ڵ���ģʽ) 
  RCC_DeInit();
 
  // ʹ���ⲿ���� 
  RCC_HSEConfig(RCC_HSE_ON);
  
  // �ȴ��ⲿ�����ȶ� 
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    // �������໷Ƶ��PLLCLK = 8MHz * 9 = 72 MHz 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
  }
  else {
    // ʹ���ڲ����� 
    RCC_HSICmd(ENABLE);
    // �ȴ����������ȶ� 
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

    // �������໷Ƶ��PLLCLK = 8MHz/2 * 16 = 64 MHz 
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16);
  }

    // ʹ��FLASH��Ԥȡ���� 
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

  //���ô�����ʱֵ,FLASH_Latency_2Ϊ������ʱ����
  FLASH_SetLatency(FLASH_Latency_2);
	
  //����ϵͳ����ʱ��
  RCC_HCLKConfig(RCC_SYSCLK_Div1); 

  //���ø����豸����ʱ�ӣ�RCC_HCLK_Div1Ϊϵͳʱ�ӳ���1
  RCC_PCLK2Config(RCC_HCLK_Div1); 

  //���õ����豸����ʱ�ӣ�RCC_HCLK_Div2Ϊϵͳʱ�ӳ���2
  RCC_PCLK1Config(RCC_HCLK_Div2);
  
  //ʹ�����໷��Ƶ
  RCC_PLLCmd(ENABLE);
  
  // �ȴ����໷��Ƶ���Ƶ���ȶ� 
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  
  // ѡ�����໷ʱ��Ϊϵͳʱ��Դ 
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  // �ȴ�������� 
  while(RCC_GetSYSCLKSource() != 0x08);
    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
            RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,
            ENABLE);

}

/*******************************************************************************
* ������   : GPIO_Configuration
* ����         : �������ô�������GPIO���ŵĲ���
* ����         : None
* ���         : None
* ����         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10;    //abcd OE ST LT0���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14 |GPIO_Pin_15;    //abcd OE ST LT0���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;    //abcd OE ST LT0���
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






