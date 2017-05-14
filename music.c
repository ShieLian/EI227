#include <msp430g2553.h>
#include <controller.h>

typedef short bool;
#define false 0
#define true 1

void Init_Ports_music(void)
{
	set_output(P2,0x2);   //将P2.1设置为输出
}

//****************************************************************************
//*********************************增加部分分鸽线*******************************
//****************************************************************************
enum note{do_=262,re=294,mi=330,fa=349,so=392,la=440,ti=494,
          $do=523,$re=587,$mi=659,$fa=698,$so=784,$la=880,$ti=988,
          $$do = 1047, $$re = 1174, $$mi = 1318};

const unsigned int music_data2[][2] =   //乐谱
{
    {$mi,800},{$re,800},{$do,800},{ti,800},{la,800},{so,800},{la,800},{ti,800},
    {$do,200},{ti,200},{$do,200},{mi,200},
    {so,400},{la,200},{ti,200},
    {$do,200},{ti,200},{$do,200},{mi,200},
    {$so,200},{$mi,200},{$so,200},{$la,200},
    {$fa,200},{$mi,200},{$re,200},{$fa,200},
    {$mi,200},{$re,200},{$do,200},{ti,200},//
    {la,200},{so,200},{fa,200},{$do,200},
    {ti,800},//-*
    {$do,200},{ti,200},{$do,200},{mi,200},
    {so,400},{la,200},{ti,200},
    {$do,200},{ti,200},{$do,200},{mi,200},
    {$so,200},{$mi,200},{$so,200},{$la,200},
    {$fa,200},{$mi,200},{$re,200},{$fa,200},
    {$mi,200},{$re,200},{$do,200},{ti,200},//
    {la,200},{so,200},{fa,200},{$do,200},
    {ti,200},{so,200},{$do,200},{ti,200},
    //auto-gen
    {$so,200},{$mi,100},{$fa,100},{$so,200},{$mi,100},{$fa,100},
    {$so,100},{so,100},{la,100},{ti,100},{$do,100},{$re,100},{$mi,100},{$fa,100},
    {$mi,200},{$do,100},{$re,100},{$mi,200},{mi,100},{fa,100},
    {so,100},{la,100},{so,100},{mi,100},{so,100},{$do,100},{ti,100},{$do,100},
    {la,200},{$do,100},{ti,100},{la,200},{so,100},{fa,100},
    {so,100},{fa,100},{mi,100},{fa,100},{so,100},{la,100},{ti,100},{$do,100},
    {la,200},{$do,100},{ti,100},{$do,200},{ti,100},{la,100},
    {ti,100},{la,100},{ti,100},{$do,100},{$re,100},{$mi,100},{$fa,100},{$so,100},
    {$so,200},{$mi,100},{$fa,100},{$so,200},{$mi,100},{$fa,100},
    {$so,100},{so,100},{la,100},{ti,100},{$do,100},{$re,100},{$mi,100},{$fa,100},
    {$mi,200},{$do,100},{$re,100},{$mi,200},{mi,100},{fa,100},
    {so,100},{la,100},{so,100},{mi,100},{so,100},{$do,100},{ti,100},{$do,100},
    {la,200},{$do,100},{ti,100},{la,200},{so,100},{fa,100},
    {so,100},{fa,100},{mi,100},{fa,100},{so,100},{la,100},{ti,100},{$do,100},
    {la,200},{$do,100},{ti,100},{$do,200},{ti,100},{la,100},
    {ti,100},{la,100},{ti,100},{$do,100},{$re,100},{$mi,100},{$fa,100},{$so,100},
    {$mi,200},{$do,100},{$re,100},{$mi,200},{$re,100},{$do,100},
    {$re,100},{ti,100},{$do,100},{$re,100},{$mi,100},{$re,100},{$do,100},{ti,100},
    {$do,200},{la,100},{ti,100},{$do,200},{mi,100},{fa,100},
    {so,100},{la,100},{so,100},{fa,100},{so,100},{$do,100},{ti,100},{$do,100},
    {la,200},{$do,100},{ti,100},{la,200},{so,100},{fa,100},
    {so,100},{fa,100},{mi,100},{fa,100},{so,100},{la,100},{ti,100},{$do,100},
    {la,200},{$do,100},{ti,100},{$do,200},{ti,100},{la,100},
    {ti,100},{$do,100},{$re,100},{$do,100},{ti,100},{$do,100},{la,100},{ti,100},
    {$do,800},{so,800},{la,800},{mi,800},{fa,800},{$do,800},{la,800},{ti,800},{$do,1600},
    //
    {0,0} //卡农
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
unsigned int mode = 1; //输出方式标志
unsigned int end_flag = 1;  //模式2-下个音符指针
extern int levelmask=0;
double freq_multi=1.0;
//unsigned int changespeed_flag = 0;
void Init_Timer1(void)   //初始化计数器A1
{
    if(mode == 1){
        //TA1CTL = TASSEL_2 + MC_1;
        TA1CCTL1 = OUTMOD_7;
        TA1CCR0 = 1000000/440;  //设定周期，100000为定时器1的时钟频率，440为音频频率
        TA1CCR1 = TA1CCR0/2;  //设置占空比为50%
    }else if(mode == 2){
        if(audio_frequency==0)
            TA1CTL=0;
        else{
            TA1CTL = TASSEL_2 + MC_1;
            TA1CCR0=500000/audio_frequency;
            TA1CCTL0=CCIE;
        }
    }
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
 *计时器A0触发中断,20ms
 */
int divcounter=0;//分频计数器
void update_music(void)
{
	//音乐播放代码段,方式1
    const unsigned int (*musicptr)[2];
    if(play_flag==3)
        musicptr=music_data1;
    else if(play_flag==4)
        musicptr=music_data2;
    if(play_flag>0){
        if(audio_dura != 0) audio_dura --;
        else{
            if(mode==1){
                TA1CTL = 0;   //计时器A1停止
                audio_frequency = musicptr[audio_ptr][0]*freq_multi;  //读取下一个音符的频率和持续时间（下行）
                audio_dura = musicptr[audio_ptr][1]/speed;  //改变20可以调整节奏快慢
                if(audio_frequency == 0 && audio_dura == 0) {audio_ptr = 0; play_flag = 0;}  //音乐播放结束，指针归0
                else
                {
                    audio_ptr++;  //指针后移一位
                    if(audio_frequency == 0) TA1CTL = 0;
                    else{
                    //启动计时器A1
                    P2SEL |= BIT1;  //将P2.1作为方波输出
                    TA1CCR0 = 1000000/audio_frequency;
                    TA1CCR1 = TA1CCR0/2;
                    TA1CCTL0=0;
                    TA1CTL = TASSEL_2 + MC_1;  //Source:SMCLK = 1MHz, PWM mode
                    }
                }
            }
            else {
                //TA1CTL = 0;   //计时器A1停止
                audio_frequency = musicptr[audio_ptr][0]*freq_multi;  //读取下一个音符的频率和持续时间（下行）
                audio_dura = musicptr[audio_ptr][1]/speed;  //改变20可以调整节奏快慢
                if(audio_frequency == 0 && audio_dura == 0) {audio_ptr = 0; play_flag = 0;}  //音乐播放结束，指针归0
                else{
                    audio_ptr++;  //指针后移一位
                    if(audio_frequency == 0) TA1CTL = 0;
                    else{
                        //启动计时器A1,开中断
                        //P2OUT|=BIT2;
                        TA1CTL = TASSEL_2 + MC_1;
                        TA1CCR0=500000/audio_frequency;
                        TA1CCTL0=CCIE;
                    }
                }
            }
        }
    }else{
        TA1CTL=0;
    }
    ++divcounter;
}

/**
 * 主程序更新控制信号
 */
void update_play_flag(){
    if(key_code == 3 && play_flag != 3) {TA1CTL = 0; audio_ptr = 0; play_flag = 3;}  //将3号按钮作为播放音乐1的控制开关
    if(key_code == 4 && play_flag != 4) {TA1CTL = 0; audio_ptr = 0; play_flag = 4;}  //将4号按钮作为播放音乐2的控制开关
    if(key_code == 16) {TA1CTL = 0; audio_ptr = 0; play_flag = 0;}
}
bool update_speed=false;
void update_play_speed(){
    if(!update_speed & key_code == 7){
        speed-=5;
        if(speed == 0)
            speed = 5;
        update_speed=true;
    }
    if(!update_speed & key_code == 8)
    {
        speed+=5;
        if(speed == 35)
            speed = 30;
        update_speed=true;
    }
    if(update_speed & key_code==0)
        update_speed=false;
}
bool update_mode=false;
void update_play_mode(){
    if(!update_mode & key_code==15){
        mode = mode==1 ? 2:1;
        levelmask= mode==1 ? 0:1;
        update_mode=true;
    } else if(update_mode & key_code==0){
        update_mode=false;
    }

    led[0]=(unsigned char)mode;
}
bool update_freq=false;
int freq_level=0;

void update_requency(){
    if(!update_freq & key_code == 11){
        freq_multi/=2;
        --freq_level;
        if(freq_level==-2){
            freq_level=-1;
            freq_multi=0.5;
        }
        update_freq=true;
    }
    if(!update_freq & key_code == 12)
    {
        freq_multi*=2;
        ++freq_level;
        if(freq_level==3){
            freq_level=2;
            freq_multi=4;
        }
        update_freq=true;
    }
    if(update_freq & key_code==0)
        update_freq=false;
    //state display
    if(freq_level<0){
        led[1]=1;
    }else if(freq_level>0){
        if(freq_level==2)
            led[1]=2;
        else{
            if(divcounter==25){
                led[1]=led[1]==2?0:2;
                divcounter=0;
            }
        }

    }else
        led[1]=0;
    if(divcounter==25)
        divcounter=0;
}

void update_music_ctrl(void)
{
    update_play_flag();
    update_play_speed();
    update_play_mode();
    update_requency();
}

////////////播放方案2///////////////
/**
 * 变时长中断，用于产生4路方波
 */
int togglestate=0;
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A1 (void)
{
    togglestate = togglestate?0:1;
    if(togglestate)
        P1OUT =(unsigned char)((P1OUT & 0xF0) + (level&0x0F));
    else
        P1OUT =(unsigned char)((P1OUT & 0xF0) + 0x00);
}
