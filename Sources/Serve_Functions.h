#ifndef  _SERVE_FUNCTIONS_H                  //if no define
#define  _SERVE_FUNCTIONS_H                  //define  如果没有定义则定义，防止其它文件多次调用此头文件时而重复定义

extern void AD_Collect(void);                //AD信号采集函数
extern void Servo_Con(void);                 //舵机控制函数
extern void Motor_Con(void);                 //延时函数
extern void Delay(int j);                    //电机控制函数
extern void Speed_Choose(void);              //速度档位选择
extern void Check_Start(void);                //起跑线检测
extern void Collect_RoalMas(void);
extern void Pro_AD(void); 
extern void Ring(void);

#define RING_ON (PORTB_PB3 = 0)
#define RING_OFF (PORTB_PB3 = 1)
#endif                                        