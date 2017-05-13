#include <msp430g2553.h>
#include <controller.h>

typedef short bool;
#define false 0
#define true 1

void Init_Ports_music(void)
{
	set_output(P2,0x2);
}


enum note{do_=262,re=294,mi=330,fa=349,so=392,la=440,ti=494,
          $do=523,$re=587,$mi=659,$fa=698,$so=784,$la=880,$ti=988,
          $$do = 1047, $$re = 1174, $$mi = 1318};

const unsigned int music_data2[][2] =
{
    {$mi,800},{$re,800},{$do,800},{ti,800},{la,800},{so,800},{la,800},{ti,800},
    {$do,200},{ti,200},{$do,200},{mi,200},
    {so,400},{la,200},{ti,200},
    {$do,200},{ti,200},{$do,200},{mi,200},
    {$so,200},{$mi,200},{$so,200},{$la,200},
    {$fa,200},{$mi,200},{$re,200},{$fa,200},
    {$mi,200},{$re,200},{$do,200},{ti,200},
    {la,200},{so,200},{fa,200},{$do,200},
    {ti,800},
    {$do,200},{ti,200},{$do,200},{mi,200},
    {so,400},{la,200},{ti,200},
    {$do,200},{ti,200},{$do,200},{mi,200},
    {$so,200},{$mi,200},{$so,200},{$la,200},
    {$fa,200},{$mi,200},{$re,200},{$fa,200},
    {$mi,200},{$re,200},{$do,200},{ti,200},
    {la,200},{so,200},{fa,200},{$do,200},
    {ti,200},{so,200},{$do,200},{ti,200},

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

    {0,0}
};

const unsigned int music_data1[][2] =
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
unsigned int audio_frequency = 0;
unsigned int audio_dura = 0;
unsigned int audio_ptr = 0;
unsigned int play_flag = 0;
unsigned int speed = 20;
unsigned int mode = 1;
unsigned int end_flag = 1;
extern int levelmask=0;
double freq_multi=1.0;

void Init_Timer1(void)
{
    if(mode == 1){

        TA1CCTL1 = OUTMOD_7;
        TA1CCR0 = 1000000/440;
        TA1CCR1 = TA1CCR0/2;
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
    Init_Timer1();
    Init_Ports_music();
}



int divcounter=0;
void update_music(void)
{
    const unsigned int (*musicptr)[2];
    if(play_flag==3)
        musicptr=music_data1;
    else if(play_flag==4)
        musicptr=music_data2;
    if(play_flag>0){
        if(audio_dura != 0) audio_dura --;
        else{
            if(mode==1){
                TA1CTL = 0;
                audio_frequency = musicptr[audio_ptr][0]*freq_multi;
                audio_dura = musicptr[audio_ptr][1]/speed;
                if(audio_frequency == 0 && audio_dura == 0) {audio_ptr = 0; play_flag = 0;}
                else
                {
                    audio_ptr++;
                    if(audio_frequency == 0) TA1CTL = 0;
                    else{
                    P2SEL |= BIT1;
                    TA1CCR0 = 1000000/audio_frequency;
                    TA1CCR1 = TA1CCR0/2;
                    TA1CCTL0=0;
                    TA1CTL = TASSEL_2 + MC_1;
                    }
                }
            }
            else {
                audio_frequency = musicptr[audio_ptr][0]*freq_multi;
                audio_dura = musicptr[audio_ptr][1]/speed;
                if(audio_frequency == 0 && audio_dura == 0) {audio_ptr = 0; play_flag = 0;}
                else{
                    audio_ptr++;
                    if(audio_frequency == 0) TA1CTL = 0;
                    else{
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


void update_play_flag(){
    if(key_code == 3 && play_flag != 3) {TA1CTL = 0; audio_ptr = 0; play_flag = 3;}
    if(key_code == 4 && play_flag != 4) {TA1CTL = 0; audio_ptr = 0; play_flag = 4;}
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
