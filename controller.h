/*
 * controller.h
 *
 *  Created on: 2017年3月29日
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
// 8个LED指示灯状态，每个灯4种颜色状态，0灭，1绿，2红，3橙（红+绿）
// 注：板上指示灯从左到右序号排列为7、6、5、4、3、2、1、0
//     对应元件LED8、LED7、LED6、LED5、LED4、LED3、LED2、LED1
extern unsigned char led[];

#endif /* CONTROLLER_H_ */
