#ifndef  _COMMON_DEF_H                       //if no define
#define  _COMMON_DEF_H                       //define  如果没有定义则定义，防止其它文件多次调用此头文件时而重复定义
/*-------------------关于舵机宏定义---------------------*/
#define  SERVO_MID         3162        //舵机中间值宏定义
#define  SERVO_LEFT        3580         //舵机左偏最大值    //原来 left 2220 right 3260
#define  SERVO_RIGHT       2750        //舵机右偏最大值
#define  SERVO_TURN_HOLD   240       //舵机转向阈值
#define  SERVO_TURN_P      15           //舵机转向比例值
#define  SERVO_TURN_D      10



         //舵机转向微分值        //28 10
/*-------------------关于电机宏定义---------------------*/
#endif