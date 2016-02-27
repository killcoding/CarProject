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
   FBE_Init();                                //����ʱ�ӳ�ʼ��
   IO_Init();                                 //���ֶ˿ڳ�ʼ��
   //SCI_Init();                              //SCI��ʼ��
   Check_boma();
   Servo_Init();                              //���ģ���ʼ��
   Forward_Init();                            //�������ģ���ʼ��
   ATD_Init();                                //ADģ���ʼ��
   RTI_Init();                               //ʵʱʱ���ж�
  // DFlash_Init();                           //DFlash��ʼ��   
  // DFlash_Erase(data_Address);               //ȷ���Ȳ�����д��
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
   CRGFLG_RTIF=1;        //���жϱ�־λ
   AD_Collect();                             //AD��������
   Servo_Con();                              //������ƺ���
   Motor_Con();                              //������ƺ���
      
} 


#pragma CODE_SEG DEFAULT
