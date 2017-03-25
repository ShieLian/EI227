//???????????????RC??????     DCO??8MHz,??CPU???;  SMCLK??1MHz,??????????
#include <msp430g2553.h>
#include <tm1638.h>  //??TM1638?й???????????????????H?????

//////////////////////////////
//         ????????         //
//////////////////////////////

// 0.1s???????????????5??20ms
#define V_T100ms	5
// 0.5s???????????????25??20ms
#define V_T500ms	25

//////////////////////////////
//       ????????           //
//////////////////////////////

// ?????????????
unsigned char clock100ms=0;
unsigned char clock500ms=0;
// ???????????????
unsigned char clock100ms_flag=0;
unsigned char clock500ms_flag=0;
// ?????ü?????
unsigned int test_counter=0;
// 8λ??????????????????????
// ???????????λ??????????????4??5??6??7??0??1??2??3
unsigned char digit[8]={' ','-',0x80,'1','G','A','I','N'};
// 8λС???? 1??  0??
// ???????????λС?????????????????4??5??6??7??0??1??2??3
unsigned char pnt=0x04;
// 8??LED?????????????4?????????0??1???2??3?????+???
// ????????????????????????7??6??5??4??3??2??1??0
//     ??????LED8??LED7??LED6??LED5??LED4??LED3??LED2??LED1
unsigned char led[]={0,0,0,0,0,0,0,0};
// ????????
unsigned char key_code=0;
bool upgraded=false;
unsigned short level=1;

enum PIN {P1,P2};
//////////////////////////////
//       ???????         //
//////////////////////////////
void set_input(enum PIN pin,unsigned char port){
    unsigned char pinOUT,pinDIR,pinREN;
    if(pin==P1){
        pinOUT=P1OUT;
        pinDIR=P1DIR;
        pinREN=P1REN;
    } else{
        pinOUT=P2OUT;
        pinDIR=P2DIR;
        pinREN=P2REN;
    }
    pinDIR &= !port;
    pinOUT |= port;
    pinREN |= port;
}

void set_output(enum PIN pin,unsigned char port){
    if(pin==P1){
        P1DIR |= port;
    } else{
        P2DIR |= port;
    }
}
//  I/O????????????
void Init_Ports(void)
{
	P2SEL &= ~(BIT7+BIT6);       //P2.6??P2.7 ????????I/O???
	  //?????????????????????????

	P2DIR |= BIT7 + BIT6 + BIT5; //P2.5??P2.6??P2.7 ????????
	set_output(P1,0xF);
	set_output(P2,0x2);   //将P2.1设置为输出
}

//  ?????TIMER0?????????????20ms
void Init_Timer0(void)
{
	TA0CTL = TASSEL_2 + MC_1 ;      // Source: SMCLK=1MHz, UP mode,
	TA0CCR0 = 20000;                // 1MHz???,????20000??? 20ms
	TA0CCTL0 = CCIE;                // TA0CCR0 interrupt enabled
}

//****************************************************************************
//*********************************增加部分分鸽线*******************************
//****************************************************************************
const unsigned int music_data2[][2] =   //乐谱：全场最佳。
{
     {262,400},{294,400},{330,400},{349,400},{0,400},{392,400},{440,400},{494,400},{523,400},{0,0}
     //{220,800}, {327,200},{440,100},{392,100},{327,400},{220,400},{262,200},{392,100},{327,100},{294,600},{262,100},{247,100},{294,200},{262,100},{247,100},{220,600},{563,100},{494,100},{440,400},{440,400},{494,800},{0,0}


};

const unsigned int music_data1[][2] =   //乐谱：荷塘月色。
{
     {523,600},{784,200},{523,200},{784,200},{523,200},{587,200},{659,1600},
     {523,600},{784,200},{523,200},{784,200},{523,200},{587,200},{587,1600},
     {523,600},{784,200},{523,200},{784,200},{587,200},{523,200},
     {440,1000},{392,200},{523,200},{587,200},
     {523,600},{784,200},{523,200},{784,200},{523,200},{440,200},{523,1600},
     {523,200},{523,400},{440,200},{392,400},{440,400},
     {523,600},{784,200},{523,200},{784,200},{523,200},{587,200},{659,1600},
     {523,400},{523,200},{587,200},{659,800},
     {587,200},{587,400},{523,200},{587,400},{587,200},{784,200},
     {784,200},{659,200},{659,200},{587,200},{659,800},
     {523,200},{523,400},{440,200},{392,400},{784,400},
     {659,200},{587,200},{659,200},{587,200},{523,800},
     {587,200},{587,400},{523,200},{587,200},{587,400},{659,200},
     {587,200},{523,200},{440,200},{587,200},{523,800},
     {523,200},{523,400},{440,200},{392,400},{440,400},
     {523,200},{523,400},{587,200},{659,800},
     {587,200},{587,400},{523,200},{587,400},{587,200},{784,200},
     {784,200},{659,200},{659,200},{587,200},{659,800},
     {523,200},{523,200},{523,200},{440,200},{392,400},{784,400},
     {659,200},{587,200},{659,200},{587,200},{523,800},
     {587,200},{587,400},{523,200},{587,200},{587,400},{659,200},
     {587,200},{523,200},{440,200},{587,200},{523,800},
     {659,200},{784,400},{784,200},{784,400},{784,400},
     {880,200},{784,200},{659,200},{587,200},{523,800},
     {880,200},{1047,200},{880,200},{784,200},{659,200},{587,200},{523,200},{440,200},
     {587,400},{587,200},{659,200},{659,200},{587,600},
     {659,200},{784,400},{784,200},{784,400},{784,400},
     {880,200},{784,200},{659,200},{587,200},{523,800},
     {440,200},{523,200},{440,200},{392,200},{587,400},{659,400},{523,1200},{0,400},
     {0,0}


};
unsigned int audio_frequency = 0;  //当前音频频率
unsigned int audio_dura = 0;  //当前音频持续时间
unsigned int audio_ptr = 0;  //辅助读谱指针
unsigned int play_flag = 0;  //音乐播放标志。0时不播放，1时开始播放
void Init_Timer1(void)   //初始化计数器A1
{

    TA1CTL = TASSEL_2 + MC_1;
    TA1CCTL1 = OUTMOD_7;
    TA1CCR0 = 1000000/440;  //设定周期，100000为定时器1的时钟频率，440为音频频率
    TA1CCR1 = TA1CCR0/2;  //设置占空比为50%
}

//  MCU???????????????????????????
void Init_Devices(void)
{
	WDTCTL = WDTPW + WDTHOLD;     // Stop watchdog timer?????????
	if (CALBC1_8MHZ ==0xFF || CALDCO_8MHZ == 0xFF)
	{
		while(1);            // If calibration constants erased, trap CPU!!
	}

    //???????????RC??????     DCO??8MHz,??CPU???;  SMCLK??1MHz,??????????
	BCSCTL1 = CALBC1_8MHZ; 	 // Set range
	DCOCTL = CALDCO_8MHZ;    // Set DCO step + modulation
	BCSCTL3 |= LFXT1S_2;     // LFXT1 = VLO
	IFG1 &= ~OFIFG;          // Clear OSCFault flag
	BCSCTL2 |= DIVS_3;       //  SMCLK = DCO/8

    Init_Ports();           //???ú??????????I/O??
    Init_Timer0();          //调用函数，初始化定时器0
    Init_Timer1();          //调用函数，初始化定时器1
    _BIS_SR(GIE);           //??????ж?
   //all peripherals are now initialized
}

//////////////////////////////
//      中断服务程序                   //
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
	// ????????????LED????
	//TM1638_RefreshDIGIandLED(digit,pnt,led);
	TM1638_RefreshDIGIandLED(digit,pnt,led);

	// 检查当前键盘输入
	//   ???????????λ???????
	key_code=TM1638_Readkeyboard();
	//digit[6]=key_code%10;
	//digit[5]=key_code/10;

	//音乐播放代码段
	if(play_flag == 3)
	{
	    if(audio_dura != 0) audio_dura --;
	    else
	    {
	        TA1CTL = 0;   //计时器A1停止
	        audio_frequency = music_data1[audio_ptr][0];  //读取下一个音符的频率和持续时间（下行）
	        audio_dura = music_data1[audio_ptr][1]/15;  //改变20可以调整节奏快慢
	        if(audio_frequency == 0 && audio_dura == 0) {audio_ptr = 0; play_flag = 0;}  //音乐播放结束，指针归0
	        else
	        {
	            audio_ptr++;  //指针后移一位
	            if(audio_frequency == 0) TA1CTL = 0;
	            else{
	            //启动计时器A1
	            P2SEL |= BIT1;  //将P2.1作为输出口
	            P2DIR |= BIT1;
	            TA1CCR0 = 1000000/audio_frequency;
	            TA1CCR1 = TA1CCR0/2;
	            TA1CTL = TASSEL_2 + MC_1;  //Source:SMCLK = 1MHz, PWM mode
	            }
	        }
	    }
	}
	else if(play_flag == 4)
	    {
	        if(audio_dura != 0) audio_dura --;
	        else
	        {
	            TA1CTL = 0;   //计时器A1停止
	            audio_frequency = music_data2[audio_ptr][0];  //读取下一个音符的频率和持续时间（下行）
	            audio_dura = music_data2[audio_ptr][1]/15;  //改变20可以调整节奏快慢
	            if(audio_frequency == 0 && audio_dura == 0) {audio_ptr = 0; play_flag = 0;}  //音乐播放结束，指针归0
	            else
	            {
	                audio_ptr++;  //指针后移一位
	                if(audio_frequency == 0) TA1CTL = 0;
	                else{
	                //启动计时器A1
	                P2SEL |= BIT1;  //将P2.1作为输出口
	                P2DIR |= BIT1;
	                TA1CCR0 = 1000000/audio_frequency;
	                TA1CCR1 = TA1CCR0/2;
	                TA1CTL = TASSEL_2 + MC_1;  //Source:SMCLK = 1MHz, PWM mode
	                }
	            }
	        }
	    }

}

//////////////////////////////
//         主程序                       //
//////////////////////////////

void update_level(void){
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
    digit[3] = (digit[3]&0x80) + level%10;
    digit[2] = (digit[2]&0x80) + level/10;

    P1OUT=(unsigned char)((P1OUT & 0xF0) + (level & 0x0F));
}

int main(void)
{
	//unsigned char i=0,temp;
	Init_Devices();
	while (clock100ms<3);   // 延迟60ms秒等待TM1638上电完成
	init_TM1638();      //初始化TM1638

	while(1)
	{
		if(key_code == 3 && play_flag != 3) {TA1CTL = 0; audio_ptr = 0; play_flag = 3;}  //将3号按钮作为播放音乐1的控制开关
		if(key_code == 4 && play_flag != 4) {TA1CTL = 0; audio_ptr = 0; play_flag = 4;}  //将4号按钮作为播放音乐2的控制开关
		if(key_code == 8) {TA1CTL = 0; audio_ptr = 0; play_flag = 0;} //将8号按钮作为终止按钮。
	    if (clock100ms_flag==1)   // ???0.1??????
		{
			clock100ms_flag=0;
            update_level();
		}

		if (clock500ms_flag==1)   // ???0.5??????
		{
			clock500ms_flag=0;
		}
	}
}
