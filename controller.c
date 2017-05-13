#include <msp430g2553.h>
#include <tm1638.h>
#include <music.h>
#include <remote_control.h>
#include <autoamp.h>

#define V_T100ms	5

#define V_T500ms	25

unsigned char clock100ms=0;
unsigned char clock500ms=0;

unsigned char clock100ms_flag=0;
unsigned char clock500ms_flag=0;

unsigned int test_counter=0;

extern unsigned char digit[8]={' ','-',0x80,'1','G','A','I','N'};
unsigned char pnt=0x04;
extern unsigned char led[]={0,0,0,0,0,0,0,0};
extern unsigned char key_code=0;
bool upgraded=false;
unsigned short level=1;

enum PIN {P1,P2};

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

void Init_Ports(void)
{
	P2SEL &= ~(BIT7+BIT6);

	P2DIR |= BIT7 + BIT6 + BIT5;
	set_output(P1,0xF);
}

void Init_Timer0(void)
{
	TA0CTL = TASSEL_2 + MC_1 ;
	TA0CCR0 = 20000;
	TA0CCTL0 = CCIE;
}

void Init_Devices(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	if (CALBC1_8MHZ ==0xFF || CALDCO_8MHZ == 0xFF)
	{
		while(1);
	}

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;
	BCSCTL3 |= LFXT1S_2;
	IFG1 &= ~OFIFG;
	BCSCTL2 |= DIVS_3;

    Init_Ports();
    Init_Timer0();
    _BIS_SR(GIE);
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
	if (++clock100ms>=V_T100ms)
	{
		clock100ms_flag = 1;
		clock100ms = 0;
	}
	if (++clock500ms>=V_T500ms)
	{
		clock500ms_flag = 1;
		clock500ms = 0;
	}
	TM1638_RefreshDIGIandLED(digit,pnt,led);

	key_code=TM1638_Readkeyboard();

	update_music();
	update_remote_inter();
	update_autoamp_inter();
}


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
    if(!levelmask){
        P1OUT=(unsigned char)((P1OUT & 0xF0) + (level & 0x0F));
    }
}

int main(void)
{
   	Init_Devices();
	init_music();
	init_remote_control();
	init_adc();
	while (clock100ms<3);
	init_TM1638();

	while(1)
	{
		if (clock100ms_flag==1)
		{
			clock100ms_flag=0;
            update_level();
		}

		if (clock500ms_flag==1)
		{
			clock500ms_flag=0;
		}

		update_music_ctrl();
		update_remote_ctrl();
		update_autoamp();
	}
}
