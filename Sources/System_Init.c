/***********************************************************************
**  Copyright (c)  SDU SmartCar 12
**----------------------------------------------------------------------
**	Module Name: System_Init.c
**	Module Date: October 14, 2012                         
**	Module Auth: Guo Wenjun
**  Description: 此文件是系统初始化文件，不同模块函数针对基本需求对各模块
**							 进行初始化操作
**----------------------------------------------------------------------
**  Revision History:
**  Date:		month/day/year    
**  Notes:
***********************************************************************/


/*--------------- I N C L U D E S ------------------------------------*/

#include  "Include_Def.h"
/*======================================================================
函数名：FBE_Init                          
作用  ：设置总线时钟                                                   
备注  ：晶振频率(OSC):16MHz，总线频率(BUS):40MHz，单位时钟:0.025us                                              
=======================================================================*/

void FBE_Init(void)           //函数作用:设置总线时钟
{
   CLKSEL = 0;                //先关闭时钟
                                                           
   SYNR_VCOFRQ = 1;           //VCO频率(80MHZ)在48MHz～80MHz               2* 16*    10    /     4     
   SYNR_SYNDIV = 9;           //设置SYNDIV(9)                        VCO = 2*OSC*(SYNDIV+1)/(REFDIV+1) = 80MHz
    
                                                                 //         16  /     4
   REFDV_REFFRQ = 1;          //REF频率(4MHz)在2MHz～6MHz            REF = OSC / (REFDIV+1) = 4MHz
   REFDV_REFDIV = 3;          //设置REFDIV(3)
                                                                 //         80  /     1       
   POSTDIV = 0;               //设置PLL(锁相环)分频系数(不分频)      PLL = VCO / (2*POSTDIV) = 80MHz
    
   COPCTL_CR = 0;             //关闭看门狗
    
   while(1!=CRGFLG_LOCK);     //锁定VCO频率，锁定后VCO与PLL频率为120MHz
    
                                                                 //          80 / 2 
   CLKSEL_PLLSEL = 1;         //设置总线频率，为PLL的一半            BUS = PLL / 2 = 40MHz
    
}

/*================================================================================================
函数名：RTI_Init
作用  ：初始化中断，此处设置为5ms，可根据需要自己更改
=================================================================================================*/ 
 void RTI_Init(void)
{
    
   RTICTL = 0xb7;              //中断时间：5ms    1101 1001  实时中断的溢出时钟周期=（RTR[3:0]+1) x (2的（RTR[6:4] + 9）次方)/OSCCLK                               // 0x97~1ms中断~16*1000分频;0xbf~4ms中断  0x59~10ms中断  0x54~5ms中断   0x9F~2ms中断   0x97~1ms中断
   CRGINT = 0x80;              //使能RTI中断
}

/*=================================================================================================
函数名：Servo_Init
作用  ：初始化PWM01，PWM01用来控制舵机，舵机周期为20ms 
******* 当高电平时间为1.5ms时 舵机位于物理中值，舵机转动的范围请大家自己填写参数来试验(更改PWMDTY01)
==================================================================================================*/ 

 void Servo_Init(void)       
 {
   PWMCTL_CON01 = 1;          //使用PWM0、PWM1端口组成16位PWM
  
   PWME_PWME1 = 0;	          //PWM1为输出端口  
  								 
   PWMPRCLK_PCKA = 2;         //clockA 为总线时钟4分频(不分频)，10MHz
                             
   PWMSCLA = 2;               //clockSA为clockA 4分频，2.5MHz
  
   PWMCLK_PCLK1 = 1;          //选择clockSA为PWMA的时钟，单位时钟4us
                      
   PWMPOL_PPOL1 = 1;					//电平选择	

   PWMCNT01 = 0;              //计数器初值
  
   PWMDTY01 = SERVO_MID;     //占空比3750/50000(舵机PWM的高电平为1.5ms，根据安装方式不同再具体更改)  
                   
   PWMPER01 = 50000;          //周期50000个单位时钟(即20ms,50Hz)
   
   PWME_PWME1 = 1;            //输出端口为PWM1   
  }
  
/*================================================================================================
函数名：Forward_Init
作用  ：初始化PWM23和PWM45，分别用来控制电机正反转。PWM占空比越高，电机转速越快
********注意PWM23和PWM45不能同时输出！
=================================================================================================*/

 void Forward_Init(void)         
 {
   PWMCTL_CON23 = 1;          
   PWMCTL_CON67 = 1;          //PWMCTL      PWM控制寄存器                   (即:PWMCTL |= 0x20;)使用两个8位端口PWM2、PWM3和PWM4、PWM5组合输出16位PWM波 
  
   PWME_PWME3 = 0;			      
   PWME_PWME7 = 0;            //PWME        PWM使能寄存器                   (即:PWME &= 0xF7;以下略)先停止PWM3、PWM5
   PWME_PWME4 = 0;			      
   PWME_PWME5 = 0;
   			      
   PWMPRCLK_PCKA = 2;         //PWMPRCLK    PWM时钟预分频寄存器              clockA  四分频，clockA = busclk/4 = 10MHz;
   PWMPRCLK_PCKB = 2;         //PWMPRCLK    PWM时钟预分频寄存器              clockB  四分频，clockB = busclk/4 = 10MHz;
                             
   PWMCLK_PCLK3 = 0;          //PWMCLK      PWM时钟选择寄存器                PWM3使用clockB为时钟，单位时钟为0.1us         
   PWMCLK_PCLK7 = 0;          //PWMCLK      PWM时钟选择寄存器                PWM7使用clockA为时钟，单位时钟为0.1us          
   PWMCLK_PCLK4 = 0;
   PWMCLK_PCLK5 = 0;
   
   PWMPOL_PPOL3 = 1;          
   PWMPOL_PPOL7 = 1;          //PWMPOL      PWM电平选择寄存器                PWM3、PWM5初始为高电平，比较匹配时清零
   PWMPOL_PPOL4 = 1;
   PWMPOL_PPOL5 = 1;
   
   PWMCNT23 = 0;              
   PWMCNT67 = 0;              //PWMCNTx     PWM计数寄存器                    PWM23、PWM45计数器清零(递增型计数器)
   PWMCNT4 = 0;
   PWMCNT5 = 0;
   
   PWMDTY23 = 0;           
   PWMDTY67 = 0;              //PWMDTYx     PWM比较寄存器(PWM高电平)         计数器比较匹配后输出相应的电平:PWM高电平时间：0个单位时钟，占空比0/2000 = 0%    
   PWMDTY4 = 0;           
   PWMDTY5 = 0;                   
            
   PWMPER23 = 750;           
   PWMPER67 = 750;            //PWMPERx     PWM预置数寄存器(PWM周期)         PWM周期：750个单位时钟，约为75us(13.3kHz)    标注（考虑是否会对采样进行干扰   万）            
   PWMPER4 = 250;
   PWMPER5 = 250;
   
   PWME_PWME3 = 1;            //PWME        PWM使能寄存器                    使能PWM3，PWM3为输出端口
   PWME_PWME7 = 1;            //PWME        PWM使能寄存器                    使能PWM5，PWM5为输出端口
   PWME_PWME4 = 1;
   PWME_PWME5 = 1;
 }
 
 /*===============================================================================================
函数名：ATD_Init()                          
作用  ：AD转换初始化                                                   
备注  ：AD转换初始化是XS单片机与DG单片机区别最大的地方                                              
==================================================================================================*/

void ATD_Init(void)
{  
 
   ATD0CTL0=0x0f;              //多通道转换，转换16个通道，在AN15完成后回到AN0
    
   ATD0CTL1=0x00;              //8位精度，外部触发源为AN0，采样前不放电
    
   ATD0CTL2=0x40;              //禁止ATD比较中断，ATD队列转换完成中断使能，禁止外部触发，标志位快速清零
    
   ATD0CTL3=0xf8;              //右对齐，每个转换序列的长度为8，非FIFO存储模式
    
   ATD0CTL4=0x09;              //采样时间包含4个ATD时钟周期，AD时钟为BUS_CLK/(2*(9+1))
                                //A/D时钟频率应满足500Hz<=ATD_CLK<=2MHz
      
   ATD0CTL5=0x10;              //特殊转换通道被使能，单独的转换序列，几个通道循环采样，从AN0通道开始采样
}

/*************  SCI设定9600波特  ***************/
void SCI_Init(void)
{
    SCI0BDH=0x01;
    SCI0BDL=0x04;       //9600bps BaudRate=BusClock/(16*SCIBD)
   // SCI0CR1=0x00;       //设置正常码输出、8位数据、无校验
    SCI0CR2=0x2C;       //发送和接收允许，中断方式接受
}
/**************部分IO口初始化****************************************************/
void IO_Init(void) 
{
  DDRB_DDRB3 = 1;   //蜂鸣器端口设置为输出
  PORTB_PB3 = 1;    //先使蜂鸣器不响
  DDRB_DDRB5 = 0;   //以下为拨码端口设置为输入
  DDRB_DDRB6 = 0;
  DDRB_DDRB7 = 0;
}