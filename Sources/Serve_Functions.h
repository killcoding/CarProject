#ifndef  _SERVE_FUNCTIONS_H                  //if no define
#define  _SERVE_FUNCTIONS_H                  //define  ���û�ж������壬��ֹ�����ļ���ε��ô�ͷ�ļ�ʱ���ظ�����

extern void AD_Collect(void);                //AD�źŲɼ�����
extern void Servo_Con(void);                 //������ƺ���
extern void Motor_Con(void);                 //��ʱ����
extern void Delay(int j);                    //������ƺ���
extern void Speed_Choose(void);              //�ٶȵ�λѡ��
extern void Check_Start(void);                //�����߼��
extern void Collect_RoalMas(void);
extern void Pro_AD(void); 
extern void Ring(void);

#define RING_ON (PORTB_PB3 = 0)
#define RING_OFF (PORTB_PB3 = 1)
#endif                                        