/***********************************************************************
**  Copyright (c)  SDU SmartCar 12
**----------------------------------------------------------------------
**	Module Name: Serve_Functions.c
**	Module Date: October 14, 2012                         
**	Module Auth: Guo Wenjun
**  Description: ���ļ��Ǹ��ֹ��ܺ����ļ����ܹ���ɸ��ֻ�������Ĺ���Ҫ��
**  ����Ҫʹ�ñ��ļ��еĺ���ֻ��Ҫ���þͿɡ�
**----------------------------------------------------------------------
**  Revision History:
**  Date:		month/day/year    
**  Notes:
***********************************************************************/


/*--------------- I N C L U D E S ------------------------------------*/

#include  "Include_Def.h"

/*======================================================================
��������AD_Collect()
����  ���Ե���źŽ��вɼ������Բɼ��������źŽ����˲���
��ע  ��ÿ�βɼ�ʱ��ֻҪ��ATD0CTL5������д�Ϳɡ�ԭ��ʼ�������Ѷ�16��ͨ����
******* ��ͨ����Ҫʹ���ĸ�AD����Ĵ�����ֵֻҪ��ȡAD����Ĵ����Ϳɡ�
========================================================================*/
//int AD_Value_temp[8];
int AD_ADD_Temp[50];             
int AD_ave_Temp[8];
int AD_Value[5];
float AD_Pro[5]={1,1.28,1.21,1.24,1};     //����AD�ɼ�ֵ��ʹ֮��һ��Ϊ�����ֵ
/************************************************************************/
void AD_Collect(void)
{
   int i,a,b;
   for(i=0;i<8;i++)
   {
     ATD0CTL5 = 0x10;   
     while(!(ATD0STAT0&0x80));                            //�ȴ�����ת������
     AD_ADD_Temp[i]     = (int)(AD_Pro[0]*ATD0DR0);       //��ȡ��Ӧ����Ĵ���ֵ����Ӧת�����ͨ����־ATD0STAT1
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
 ʱ�䣺2014��10��30��
 ��������
 ���ã��ɼ��ܵ��ź�
 ��ע����һ�ν����µ��ܵ�ʱ���ɼ��ܵ���Ϣ����ȡ�����ֵ���ɼ������ź����ֵ

**********************************************************************/
int AD_Value_his[5];
void Collect_RoalMas(void) 
{     
    int i;
    AD_Collect();
    for(i=0;i<5;i++)                        //��5·�źţ�ѭ��5�ηֱ�ȡ��·�ź����ֵ
    {
      if(AD_Value[i]<AD_Value_his[i]) 
      {
        AD_Value[i] = AD_Value_his[i];      //��αȽ�ȡ���ֵ����AD_Value������
      }
    }
    for(i=0;i<5;i++)                        //��֤AD_Value_his���鱣�������һ���жϳ������ֵ����
    {
      AD_Value_his[i] = AD_Value[i];
    }    
} 

/************************************************************************
ʱ�䣺2014��11��4��
��������
���ã� ����AD�ź�
��ע����AD�ɼ��źųɱ��ı䣬�滮Ϊ�Լ�����ʹ�õĳ�ֵ,�˺������ڳ��������
      �ڼ䣬��AD����ȡ�����ֵ�����ʹ��
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
��������Servo_Con()
����  ����AD�ɼ����źŽ��д������������ɶ������������
��ע  ��ÿһ���������㷽��������ͬ��������������Ҫ����ÿ�����ľ��������
******* ��ȷ����������õķ����������ĸ���еĴ������ܹ���˼��ǳ��򵥣���
******* ʹ�ñ�����ʱҪ��Ծ�������ڸ�д��
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
  AD_Value_err_His_z = AD_Value_err_z;             //1��3�ŵ�У�ֱ������ʷֵ
  AD_Value_err_His = AD_Value_err;                 //0��4�ŵ�в�ֵ
  Servo_Out_His = Servo_Out;
 
  AD_Value_err_z = AD_Value[1] - AD_Value[3];            //���м��������������ֵ
  AD_Value_dif_z = AD_Value_err_z - AD_Value_err_His_z;      //���β�ֵ����ʷ��ֵ�Ĳ� 
 
  AD_Value_err = AD_Value[0] - AD_Value[4];            //������������������ֵ
  AD_Value_dif = AD_Value_err - AD_Value_err_His;      //���β�ֵ����ʷ��ֵ�Ĳ� 
 
  AD_Value_sum = AD_Value[1] + AD_Value[3];
              //���м������ֵ֮�ͣ��ж�ֱ��
  
  if((AD_Value_sum > SERVO_TURN_HOLD)||( AD_Value[2]>150))          //�ж�����
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
  
  if(Servo_Out < SERVO_RIGHT)                               //�Զ�����ֵ�޷�������������޷�����
  {                                                    //��ֹ���������
    Servo_Out = SERVO_RIGHT;
  }
  if(Servo_Out > SERVO_LEFT)
  {
    Servo_Out = SERVO_LEFT;
  }
 
  PWMDTY01 = Servo_Out;
}
/**************************************
������ Speed_Choose()
���� �ٶ�ѡ��
��ע
*************************************************/
 void Speed_Choose(void)
 {
  
 }
  /********************************************************************
������ Check_Start
����   ���������
��ע
*********************************************************************/
int K=0;
int T;
void Check_Start(void) 
{        

}
/*=========================================================================
��������Motor_Con()
����  �����Ƶ�������PWMֵ
��ע  ������Ե�����û���κβ��ԣ�Ϊ�ᳵ���������Ӧ���ڱ��������е��ٳ���
******* ����ֻ��Ϊ�˸����д��ģ������û�н��е��٣��������١����Ҫ���е���
****** �����ȶ�סPWMDTY23��PWMDTY45�е�һ�����˵��������һ����λΪ0��ֻ��
******* ��һ����ֵ��
===========================================================================*/
void Motor_Con(void)
{
   //�˴���PWMDTY��ֵ��ע������������ȸ�ֵͬ���������ȸ�0
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
������  Delay()
����    ������ʱ
��ע
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
