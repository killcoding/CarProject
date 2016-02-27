/***********************************************************************
**  Copyright (c)  SDU SmartCar 12
**----------------------------------------------------------------------
**	Module Name: Serve_Functions.c
**	Module Date: October 14, 2012                         
**	Module Auth: Guo Wenjun
**  Description: 此文件是各种功能函数文件，能够完成各种基本需求的功能要求
**  ，若要使用本文件中的函数只需要调用就可。
**----------------------------------------------------------------------
**  Revision History:
**  Date:		month/day/year    
**  Notes:
***********************************************************************/


/*--------------- I N C L U D E S ------------------------------------*/

#include  "Include_Def.h"

/*======================================================================
函数名：AD_Collect()
作用  ：对电感信号进行采集，并对采集而来的信号进行滤波。
备注  ：每次采集时，只要对ATD0CTL5进行重写就可。原初始化程序已对16个通道都
******* 开通，想要使用哪个AD结果寄存器的值只要读取AD结果寄存器就可。
========================================================================*/
//int AD_Value_temp[8];
int AD_ADD_Temp[50];             
int AD_ave_Temp[8];
int AD_Value[5];
float AD_Pro[5]={1,1.28,1.21,1.24,1};     //倍乘AD采集值，使之归一化为所需的值
/************************************************************************/
void AD_Collect(void)
{
   int i,a,b;
   for(i=0;i<8;i++)
   {
     ATD0CTL5 = 0x10;   
     while(!(ATD0STAT0&0x80));                            //等待队列转换结束
     AD_ADD_Temp[i]     = (int)(AD_Pro[0]*ATD0DR0);       //读取相应结果寄存器值清相应转换完成通道标志ATD0STAT1
     AD_ADD_Temp[8+i]   = (int)(AD_Pro[1]*ATD0DR1);
     AD_ADD_Temp[8*2+i] = (int)(AD_Pro[2]*ATD0DR2);
     AD_ADD_Temp[8*3+i] = (int)(AD_Pro[3]*ATD0DR3);			   
     AD_ADD_Temp[8*4+i] = (int)(AD_Pro[4]*ATD0DR4);
     
     if(i==7) 
     {  
        for(a=0;a<5;a++) 
        {
          for(b=0;b<8;b++) 
          {
             AD_ave_Temp[a] =  AD_ave_Temp[a]+ AD_ADD_Temp[8*a+b]; 
          }
          AD_ave_Temp[a] = AD_ave_Temp[a]/8;
          AD_Value[a] = AD_ave_Temp[a];
        }
     }
   }
}

/**********************************************************************
 时间：2014年10月30日
 函数名：
 作用：采集跑道信号
 备注：第一次进入新的跑道时，采集跑道信息，采取出电感值所采集到的信号最大值

**********************************************************************/
int AD_Value_his[5];
void Collect_RoalMas(void) 
{     
    int i;
    AD_Collect();
    for(i=0;i<5;i++)                        //共5路信号，循环5次分别取各路信号最大值
    {
      if(AD_Value[i]<AD_Value_his[i]) 
      {
        AD_Value[i] = AD_Value_his[i];      //逐次比较取最大值放入AD_Value数组中
      }
    }
    for(i=0;i<5;i++)                        //保证AD_Value_his数组保存的是上一次判断出的最大值数组
    {
      AD_Value_his[i] = AD_Value[i];
    }    
} 

/************************************************************************
时间：2014年11月4日
函数名：
作用： 倍放AD信号
备注：将AD采集信号成倍改变，规划为自己打算使用的初值,此函数用于程序输出化
      期间，在AD采样取出最大值数组后使用
************************************************************************/
void Pro_AD(void) 
{
    int i;
    for(i=1;i<4;i++)
    {
      AD_Pro[i] = 180/AD_Value[i];
    }
} 
/*=======================================================================
函数名：Servo_Con()
作用  ：对AD采集的信号进行处理，并运算生成舵机的输出结果。
备注  ：每一辆车的运算方法不尽相同，具体的运算规则要根据每个车的具体情况进
******* 行确定，这里采用的方法仅适用四个电感的传感器架构，思想非常简单，在
******* 使用本工程时要针对具体情况在改写。
========================================================================*/
int AD_Value_err_z;
int AD_Value_err ;
int AD_Value_err_His_z;
int AD_Value_err_His;
int Servo_Out_His;
int Servo_Out;
int AD_Value_dif_z ;
int AD_Value_dif;
int AD_Value_sum;

void Servo_Con(void)
{  
  AD_Value_err_His_z = AD_Value_err_z;             //1、3号电感，直道上历史值
  AD_Value_err_His = AD_Value_err;                 //0、4号电感差值
  Servo_Out_His = Servo_Out;
 
  AD_Value_err_z = AD_Value[1] - AD_Value[3];            //求中间两电感数字量差值
  AD_Value_dif_z = AD_Value_err_z - AD_Value_err_His_z;      //本次差值与历史差值的差 
 
  AD_Value_err = AD_Value[0] - AD_Value[4];            //求边沿两电感数字量差值
  AD_Value_dif = AD_Value_err - AD_Value_err_His;      //本次差值与历史差值的差 
 
  AD_Value_sum = AD_Value[1] + AD_Value[3];
              //求中间两电感值之和，判断直道
  
  if((AD_Value_sum > SERVO_TURN_HOLD)||( AD_Value[2]>150))          //判断区域
  {
    Servo_Out = SERVO_MID + 6* AD_Value_err_z + 5* AD_Value_dif_z;
  } 
  else
  {
     Servo_Out = SERVO_MID + ((170-AD_Value[2])/6)* AD_Value_err/2 + SERVO_TURN_D*AD_Value_dif;
  }

  /*if(AD_Value[1]<80&&AD_Value[3]<80&&(AD_Value[1]>AD_Value[2])&&(AD_Value[0]<AD_Value[4]))
   {
     Servo_Out = SERVO_LEFT;
   }
   if(AD_Value[0]<80&&AD_Value[4]<80&&(AD_Value[1]<AD_Value[2])&&(AD_Value[0]>AD_Value[4])) 
   {
     Servo_Out = SERVO_RIGHT;
   }
    */ 
     
 if(AD_Value[1]<60&&AD_Value[2]<60&&AD_Value[2]<70) 
  {
     Servo_Out = Servo_Out_His;
  }
  
  if(Servo_Out < SERVO_RIGHT)                               //对舵机输出值限幅，必须有输出限幅程序
  {                                                    //防止舵机打死损坏
    Servo_Out = SERVO_RIGHT;
  }
  if(Servo_Out > SERVO_LEFT)
  {
    Servo_Out = SERVO_LEFT;
  }
 
  PWMDTY01 = Servo_Out;
}
/**************************************
函数名 Speed_Choose()
作用 速度选择
备注
*************************************************/
 void Speed_Choose(void)
 {
  
 }
  /********************************************************************
函数名 Check_Start
作用   检测起跑线
备注
*********************************************************************/
int K=0;
int T;
void Check_Start(void) 
{        

}
/*=========================================================================
函数名：Motor_Con()
作用  ：控制电机输出的PWM值
备注  ：这里对电机输出没有任何策略，为横车输出，真正应用于比赛必须有调速程序
******* 这里只是为了给大家写个模版所以没有进行调速，而是匀速。如果要进行调速
****** 必须先定住PWMDTY23跟PWMDTY45中的一项，就是说必须先有一个复位为0，只对
******* 另一个赋值。
===========================================================================*/
void Motor_Con(void)
{
   //此处给PWMDTY赋值，注意正电机方向先赋同值，反方向先赋0
   if(Speed==0) 
   { 
     PWMDTY23 = 270;
     PWMDTY67 = 270;
     PWMDTY4  = 0;
     PWMDTY5  = 0;
   }
   if(Speed==1) 
   { 
     PWMDTY23 = 280;
     PWMDTY67 = 280;
     PWMDTY4  = 0;
     PWMDTY5  = 0;
   }
   if(Speed==2) 
   { 
     PWMDTY23 = 290;
     PWMDTY67 = 290;
     PWMDTY4  = 0;
     PWMDTY5  = 0;
   }
   if(Speed==3)   
   { 
     PWMDTY23 = 300;
     PWMDTY67 = 300;
     PWMDTY4  = 0;
     PWMDTY5  = 0;
   }
   if(Speed==4) 
   { 
     PWMDTY23 = 310;
     PWMDTY67 = 310;
     PWMDTY4  = 0;
     PWMDTY5  = 0;
   }
   if(Speed==5) 
   { 
     PWMDTY23 = 330;
     PWMDTY67 = 330;
     PWMDTY4  = 0;
     PWMDTY5  = 0;
   }
   if(Speed==6) 
   { 
     PWMDTY23 = 350;
     PWMDTY67 = 350;
     PWMDTY4  = 0;
     PWMDTY5  = 0;
   }
   if(Speed==7) 
   { 
     PWMDTY23 = 370;
     PWMDTY67 = 370;
     PWMDTY4  = 0;
     PWMDTY5  = 0;
   }
   
}
/************************************************************************
函数名  Delay()
作用    启动延时
备注
*************************************************************************/
void Delay(int j )
{       

  int time,i ;
  for(i=0;i<j;i++)
  for(time=0;time<11000;time++);
}

void Ring(void)
{
 int n;
 
 for (n=0;n!=7;n++)
 {
   RING_ON;
   Delay(100);
   RING_OFF;
   Delay(80);
 }
}
