/***********************************************************************
 ʱ�䣺2014��11��4��
 �ļ����ã����뿪��ѡ����
 ��ע��Ӳ������3·���룬�ֱ����ӵ�·��T�ڵ�5��6��7���͵�ƽ��ͨ
************************************************************************/
/*--------------- I N C L U D E S ------------------------------------*/

#include  "Include_Def.h"
int Speed;
void Check_boma(void) 
{
   if(PTT_PTT5==1&&PTT_PTT6==1&&PTT_PTT7==1)
      Speed = 0;                //�Ѽ��������裬��ʼʱ���㣬�˴��Բ��벦��onΪ��һ
   if(PTT_PTT5==0&&PTT_PTT6==1&&PTT_PTT7==1)
      Speed = 1; 
   if(PTT_PTT5==1&&PTT_PTT6==0&&PTT_PTT7==1)
      Speed = 2;        
   if(PTT_PTT5==0&&PTT_PTT6==0&&PTT_PTT7==1)
      Speed = 3;
   if(PTT_PTT5==1&&PTT_PTT6==1&&PTT_PTT7==0)
      Speed = 4;
   if(PTT_PTT5==0&&PTT_PTT6==1&&PTT_PTT7==0)
      Speed = 5;
   if(PTT_PTT5==1&&PTT_PTT6==0&&PTT_PTT7==0)
      Speed = 6;
   if(PTT_PTT5==0&&PTT_PTT6==0&&PTT_PTT7==0)
      Speed = 7;
}