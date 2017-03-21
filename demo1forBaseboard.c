//本程序时钟采用内部RC振荡器。     DCO：8MHz,供CPU时钟;  SMCLK：1MHz,供定时器时钟
#include <msp430g2553.h>
#include <tm1638.h>  //与TM1638有关的变量及函数定义均在该H文件中

//////////////////////////////
//         常量定义         //
//////////////////////////////

// 0.1s软件定时器溢出值，5个20ms
#define V_T100ms	5
// 0.5s软件定时器溢出值，25个20ms
#define V_T500ms	25

//////////////////////////////
//       变量定义           //
//////////////////////////////

// 软件定时器计数
unsigned char clock100ms=0;
unsigned char clock500ms=0;
// 软件定时器溢出标志
unsigned char clock100ms_flag=0;
unsigned char clock500ms_flag=0;
// 测试用计数器
unsigned int test_counter=0;
// 8位数码管显示的数字或字母符号
// 注：板上数码位从左到右序号排列为4、5、6、7、0、1、2、3
unsigned char digit[8]={' ',' ',' ',' ','.',' ',' ',' '};
// 8位小数点 1亮  0灭
// 注：板上数码位小数点从左到右序号排列为4、5、6、7、0、1、2、3
unsigned char pnt=0x04;
// 8个LED指示灯状态，每个灯4种颜色状态，0灭，1绿，2红，3橙（红+绿）
// 注：板上指示灯从左到右序号排列为7、6、5、4、3、2、1、0
//     对应元件LED8、LED7、LED6、LED5、LED4、LED3、LED2、LED1
unsigned char led[]={0,0,1,1,2,2,3,3};
// 当前按键值
unsigned char key_code=0;
bool upgraded=false;
unsigned short level=1;

//////////////////////////////
//       系统初始化         //
//////////////////////////////

//  I/O端口和引脚初始化
void Init_Ports(void)
{
	P2SEL &= ~(BIT7+BIT6);       //P2.6、P2.7 设置为通用I/O端口
	  //因两者默认连接外晶振，故需此修改

	P2DIR |= BIT7 + BIT6 + BIT5; //P2.5、P2.6、P2.7 设置为输出
	  //本电路板中三者用于连接显示和键盘管理器TM1638，工作原理详见其DATASHEET
 }

//  定时器TIMER0初始化，循环定时20ms
void Init_Timer0(void)
{
	TA0CTL = TASSEL_2 + MC_1 ;      // Source: SMCLK=1MHz, UP mode,
	TA0CCR0 = 20000;                // 1MHz时钟,计满20000次为 20ms
	TA0CCTL0 = CCIE;                // TA0CCR0 interrupt enabled
}

//  MCU器件初始化，注：会调用上述函数
void Init_Devices(void)
{
	WDTCTL = WDTPW + WDTHOLD;     // Stop watchdog timer，停用看门狗
	if (CALBC1_8MHZ ==0xFF || CALDCO_8MHZ == 0xFF)
	{
		while(1);            // If calibration constants erased, trap CPU!!
	}

    //设置时钟，内部RC振荡器。     DCO：8MHz,供CPU时钟;  SMCLK：1MHz,供定时器时钟
	BCSCTL1 = CALBC1_8MHZ; 	 // Set range
	DCOCTL = CALDCO_8MHZ;    // Set DCO step + modulation
	BCSCTL3 |= LFXT1S_2;     // LFXT1 = VLO
	IFG1 &= ~OFIFG;          // Clear OSCFault flag
	BCSCTL2 |= DIVS_3;       //  SMCLK = DCO/8

    Init_Ports();           //调用函数，初始化I/O口
    Init_Timer0();          //调用函数，初始化定时器0
    _BIS_SR(GIE);           //开全局中断
   //all peripherals are now initialized
}

//////////////////////////////
//      中断服务程序        //
//////////////////////////////

// Timer0_A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
	// 0.1秒钟软定时器计数
	if (++clock100ms>=V_T100ms)
	{
		clock100ms_flag = 1; //当0.1秒到时，溢出标志置1
		clock100ms = 0;
	}
 	// 0.5秒钟软定时器计数
	if (++clock500ms>=V_T500ms)
	{
		clock500ms_flag = 1; //当0.5秒到时，溢出标志置1
		clock500ms = 0;
	}
	// 刷新全部数码管和LED指示灯
	//TM1638_RefreshDIGIandLED(digit,pnt,led);
	TM1638_RefreshDIGIandLED(digit,pnt,led);

	// 检查当前键盘输入，0代表无键操作，1-16表示有对应按键
	//   键号显示在两位数码管上
	key_code=TM1638_Readkeyboard();
	//digit[6]=key_code%10;
	//digit[5]=key_code/10;

}

//////////////////////////////
//         主程序           //
//////////////////////////////

int main(void)
{
	unsigned char i=0,temp;
	Init_Devices();
	while (clock100ms<3);   // 延时60ms等待TM1638上电完成
	init_TM1638();	    //初始化TM1638

	while(1)
	{
		if (clock100ms_flag==1)   // 检查0.1秒定时是否到
		{
			clock100ms_flag=0;
		   	// 每0.1秒累加计时值在数码管上以十进制显示，有键按下时暂停计时
            if (++test_counter>=10000) test_counter=0;
            //更新增益等级
            if(!upgraded){
                if(key_code==1){
                    upgraded=true;
                    --level;
                    if(level==0)
                        level=15;
                } else if(key_code==2){
                    upgraded=true;
                    ++level;
                    if(level==16)
                        level=1;
                } else{
                    upgraded=false;
                }
            } else{
                if(key_code!=1 && key_code!=2)
                    upgraded=false;
            }
            //
            digit[1] = level%10;
            digit[0] = level/10;
		}

		if (clock500ms_flag==1)   // 检查0.5秒定时是否到
		{
			clock500ms_flag=0;
			// 8个指示灯以走马灯方式，每0.5秒向右（循环）移动一格
			temp=led[0];
			for (i=0;i<7;i++) led[i]=led[i+1];
			led[7]=temp;
		}
	}
}
