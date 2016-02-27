/***********************************************************************
 时间：2014年11月4号
 文件作用：拨码开关选择函数
 备注：硬件连接3路拨码，分别连接电路的T口的5、6、7，低电平导通
************************************************************************/
/*--------------- I N C L U D E S ------------------------------------*/

#include  "Include_Def.h"
int Speed;
void Check_boma(void) 
{
   if(PTT_PTT5==1&&PTT_PTT6==1&&PTT_PTT7==1)
      Speed = 0;                //已加上拉电阻，开始时置零，此处以拨码拨至on为置一
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