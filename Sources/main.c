/***********************************************************************
**  Copyright (c)  SDU SmartCar 12
**----------------------------------------------------------------------
**	Module Name: main.c
**	Module Date: October 21, 2012                         
**	Module Auth: Guo Wenjun
**  Description: main.c 
**							 
**----------------------------------------------------------------------
**  Revision History:
**  Date:		month/day/year    
**  Notes:
***********************************************************************/


/*--------------- I N C L U D E S ------------------------------------*/

#include  "Include_def.h"


//int SCI_rev_valuenum;
int AD_check_time=0;
int CheckAD_Ring=0;
void main(void) 
{
   FBE_Init();                                //总线时钟初始化
   IO_Init();                                 //部分端口初始化
   //SCI_Init();                              //SCI初始化
   Check_boma();
   Servo_Init();                              //舵机模块初始化
   Forward_Init();                            //电机驱动模块初始化
   ATD_Init();                                //AD模块初始化
   RTI_Init();                               //实时时钟中断
  // DFlash_Init();                           //DFlash初始化   
  // DFlash_Erase(data_Address);               //确保先擦除后写入
  // DFlash_Write(data_Address);
   Ring();
   EnableInterrupts;
  

   for(;;) 
   {

     // Check_Start();
     
   }
}  

#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 7 RTI_int()
{ 
   CRGFLG_RTIF=1;        //清中断标志位
   AD_Collect();                             //AD采样函数
   Servo_Con();                              //舵机控制函数
   Motor_Con();                              //电机控制函数
      
} 


#pragma CODE_SEG DEFAULT
