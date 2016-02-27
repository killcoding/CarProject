#ifndef  _SYSTEM_INIT_H                   //if no define
#define  _SYSTEM_INIT_H                   //define  如果没有定义则定义，防止其它文件多次调用此头文件时而重复定义
 
extern void FBE_Init(void);               //总线时钟初始化
extern void Servo_Init(void);             //舵机模块初始化
extern void Forward_Init(void);           //电机模块初始化
extern void ATD_Init(void);               //AD模块初始化
extern void RTI_Init(void);               //实时时钟设置
extern void SCI_Init(void);                   //串口设置函数
void IO_Init(void);                       //端口初始化
#endif 