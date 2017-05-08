/*
 * controller.h
 *
 *  Created on: 2017��3��29��
 *      Author: ShieLian
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define V_T100ms    5
#define V_T500ms    25

extern unsigned char clock100ms;
extern unsigned char clock500ms;
extern unsigned char clock100ms_flag;
extern unsigned char clock500ms_flag;
extern unsigned char digit[8];
extern unsigned char key_code;
extern unsigned short level;
enum PIN {P1,P2};
// 8��LEDָʾ��״̬��ÿ����4����ɫ״̬��0��1�̣�2�죬3�ȣ���+�̣�
// ע������ָʾ�ƴ������������Ϊ7��6��5��4��3��2��1��0
//     ��ӦԪ��LED8��LED7��LED6��LED5��LED4��LED3��LED2��LED1
extern unsigned char led[];

#endif /* CONTROLLER_H_ */
