#include <msp430g2553.h>
#include <controller.h>

void Init_Ports_music(void)
{
	set_output(P2,0x2);   //将P2.1设置为输出
}

//****************************************************************************
//*********************************增加部分分鸽线*******************************
//****************************************************************************
//const unsigned int a = 294;
enum note{do=262,re=294,mi=330,fa=349,so=392,la=440,ti=494,
          $do=523,$re=587,$mi=659,$fa=698,$so=784,$la=880,$ti=988,
          $$do = 1047, $$re = 1174, $$mi = 1318};

const unsigned int music_data2[][2] =   //乐谱：全场最佳。
{
     //{262,400},{294,400},{330,400},{349,400},{0,400},{392,400},{440,400},{494,400},{523,400},{0,0} //这是一个音阶
     //{220,800}, {327,200},{440,100},{392,100},{327,400},{220,400},{262,200},{392,100},{327,100},{294,600},{262,100},{247,100},{294,200},{262,100},{247,100},{220,600},{563,100},{494,100},{440,400},{440,400},{494,800},{0,0}
    {la,800}, {xi,800},{mi,800},{so,800},{fa,800},{mi,800},{fa,800},{so,800},{$mi,800},{$re,800},{$do,800},{ti,800},
{so,800},{so,800},{la,800},{ti,800},{$mi,800},{$so,400},{$fa,400},{$mi,800},{$mi,400},{$re,400},{la,800},{so,400},
{mi,400},{la,800},{ti,400},{$fa,400},{0,0} //卡农
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
unsigned int speed = 20;       //变速
unsigned int mode_flag = 0; //输出方式标志
unsigned int end_flag = 1;  //模式2-下个音符指针
//unsigned int changespeed_flag = 0;
void Init_Timer1(void)   //初始化计数器A1
{
    TA1CTL = TASSEL_2 + MC_1 + TAIE_0;
    TA1CCTL1 = OUTMOD_7;
    TA1CCR0 = 1000000/440;  //设定周期，100000为定时器1的时钟频率，440为音频频率
    TA1CCR1 = TA1CCR0/2;  //设置占空比为50%
}

void init_music(void)
{
    Init_Timer1();          //调用函数，初始化定时器1
    Init_Ports_music();
}

//////////////////////////////
//      中断服务程序                   //
//////////////////////////////
/**
 *计时器A0触发中断
 */
void update_music(void)
{
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
            audio_dura = music_data2[audio_ptr][1]/speed;  //改变20可以调整节奏快慢
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

/**
 * 主程序更新控制信号
 */
void update_music_ctrl(void)
{
    if(key_code == 3 && play_flag != 3) {TA1CTL = 0; audio_ptr = 0; play_flag = 3;}  //将3号按钮作为播放音乐1的控制开关
    if(key_code == 4 && play_flag != 4) {TA1CTL = 0; audio_ptr = 0; play_flag = 4;}  //将4号按钮作为播放音乐2的控制开关
    if(key_code == 7)
    {
        speed = speed - 5;
        if(speed == 0) speed = 5;
    }
    if(key_code == 8)
    {
        speed = speed + 5;
        if(speed == 35) speed = 30;
    }
    if(key_code == 12) {TA1CTL = 0; audio_ptr = 0; play_flag = 0;}
}

////////////播放方案2///////////////

void Init_Timer1_2(int f)
{
	TA1CTL = TASSEL_2 + MC_1;      // Source: SMCLK=1MHz, UP mode,
	TA1CCR0 = 1000000 / f;                // 1MHz时钟,计满20000次为 20ms
	TA1CCTL0 = CCIE;                // TA0CCR0 interrupt enabled
}
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1 (void)
{
    char flag=P1OUT&0x0F;
	if (flag == (unsigned char) 0x00) P1OUT = (unsigned char)(0x0F + P1OUT & 0xF0);
	else if (flag == (unsigned char) 0x0F) P1OUT = (unsigned char)(0x00 + P1OUT & 0xF0);
	if(audio_dura == 0) end_flag = 1;
	else {end_flag = 0; audio_dura --;}
}
void update_music2(void)
{
	Init_Devices();
	//init_music();
	audio_ptr = 0;
	audio_frequency = 0;
	audio_dura = 0;
	while (clock100ms<3);   // 延时60ms等待TM1638上电完成
	init_TM1638();	    //初始化TM1638
	while(mode == 2)
	{
	   if(end_flag = 1)
	   {
	       audio_frequency = music_data2[audio_ptr][0];
	       audio_dura = music_data2[audio_ptr][1];
	       audio_ptr++;
	       if(audio_frequency == 0 && audio_dura == 0) audio_ptr = 0;
	       else end_flag = 0;
	   }
	   while(end_flag == 0)
	   {
	       Init_Timer1(audio_frequency);          //调用函数，初始化定时器0
	       _BIS_SR(GIE);           //开全局中断
	   }
	}
}
