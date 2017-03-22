//������ʱ�Ӳ����ڲ�RC������     DCO��8MHz,��CPUʱ��;  SMCLK��1MHz,����ʱ��ʱ��
#include <msp430g2553.h>
#include <tm1638.h>  //��TM1638�йصı���������������ڸ�H�ļ���

//////////////////////////////
//         ��������         //
//////////////////////////////

// 0.1s�����ʱ�����ֵ��5��20ms
#define V_T100ms	5
// 0.5s�����ʱ�����ֵ��25��20ms
#define V_T500ms	25

//////////////////////////////
//       ��������           //
//////////////////////////////

// �����ʱ������
unsigned char clock100ms=0;
unsigned char clock500ms=0;
// �����ʱ�������־
unsigned char clock100ms_flag=0;
unsigned char clock500ms_flag=0;
// �����ü�����
unsigned int test_counter=0;
// 8λ�������ʾ�����ֻ���ĸ����
// ע����������λ�������������Ϊ4��5��6��7��0��1��2��3
unsigned char digit[8]={' ','-',0x80,'1','G','A','I','N'};
// 8λС���� 1��  0��
// ע����������λС����������������Ϊ4��5��6��7��0��1��2��3
unsigned char pnt=0x04;
// 8��LEDָʾ��״̬��ÿ����4����ɫ״̬��0��1�̣�2�죬3�ȣ���+�̣�
// ע������ָʾ�ƴ������������Ϊ7��6��5��4��3��2��1��0
//     ��ӦԪ��LED8��LED7��LED6��LED5��LED4��LED3��LED2��LED1
unsigned char led[]={0,0,0,0,0,0,0,0};
// ��ǰ����ֵ
unsigned char key_code=0;
bool upgraded=false;
unsigned short level=1;

enum PIN {P1,P2};
//////////////////////////////
//       ϵͳ��ʼ��         //
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
//  I/O�˿ں����ų�ʼ��
void Init_Ports(void)
{
	P2SEL &= ~(BIT7+BIT6);       //P2.6��P2.7 ����Ϊͨ��I/O�˿�
	  //������Ĭ�������⾧�񣬹�����޸�

	P2DIR |= BIT7 + BIT6 + BIT5; //P2.5��P2.6��P2.7 ����Ϊ���
	set_output(P1,0x7);
}

//  ��ʱ��TIMER0��ʼ����ѭ����ʱ20ms
void Init_Timer0(void)
{
	TA0CTL = TASSEL_2 + MC_1 ;      // Source: SMCLK=1MHz, UP mode,
	TA0CCR0 = 20000;                // 1MHzʱ��,����20000��Ϊ 20ms
	TA0CCTL0 = CCIE;                // TA0CCR0 interrupt enabled
}

//  MCU������ʼ����ע���������������
void Init_Devices(void)
{
	WDTCTL = WDTPW + WDTHOLD;     // Stop watchdog timer��ͣ�ÿ��Ź�
	if (CALBC1_8MHZ ==0xFF || CALDCO_8MHZ == 0xFF)
	{
		while(1);            // If calibration constants erased, trap CPU!!
	}

    //����ʱ�ӣ��ڲ�RC������     DCO��8MHz,��CPUʱ��;  SMCLK��1MHz,����ʱ��ʱ��
	BCSCTL1 = CALBC1_8MHZ; 	 // Set range
	DCOCTL = CALDCO_8MHZ;    // Set DCO step + modulation
	BCSCTL3 |= LFXT1S_2;     // LFXT1 = VLO
	IFG1 &= ~OFIFG;          // Clear OSCFault flag
	BCSCTL2 |= DIVS_3;       //  SMCLK = DCO/8

    Init_Ports();           //���ú�������ʼ��I/O��
    Init_Timer0();          //���ú�������ʼ����ʱ��0
    _BIS_SR(GIE);           //��ȫ���ж�
   //all peripherals are now initialized
}

//////////////////////////////
//      �жϷ������        //
//////////////////////////////

// Timer0_A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
	// 0.1������ʱ������
	if (++clock100ms>=V_T100ms)
	{
		clock100ms_flag = 1; //��0.1�뵽ʱ�������־��1
		clock100ms = 0;
	}
 	// 0.5������ʱ������
	if (++clock500ms>=V_T500ms)
	{
		clock500ms_flag = 1; //��0.5�뵽ʱ�������־��1
		clock500ms = 0;
	}
	// ˢ��ȫ������ܺ�LEDָʾ��
	//TM1638_RefreshDIGIandLED(digit,pnt,led);
	TM1638_RefreshDIGIandLED(digit,pnt,led);

	// ��鵱ǰ�������룬0�����޼�������1-16��ʾ�ж�Ӧ����
	//   ������ʾ����λ�������
	key_code=TM1638_Readkeyboard();
	//digit[6]=key_code%10;
	//digit[5]=key_code/10;

}

//////////////////////////////
//         ������           //
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
	while (clock100ms<3);   // ��ʱ60ms�ȴ�TM1638�ϵ����
	init_TM1638();	    //��ʼ��TM1638

	while(1)
	{
		if (clock100ms_flag==1)   // ���0.1�붨ʱ�Ƿ�
		{
			clock100ms_flag=0;
		   	// ÿ0.1���ۼӼ�ʱֵ�����������ʮ������ʾ���м�����ʱ��ͣ��ʱ
            //if (++test_counter>=10000) test_counter=0;
            //��������ȼ�
            update_level();
		}

		if (clock500ms_flag==1)   // ���0.5�붨ʱ�Ƿ�
		{
			clock500ms_flag=0;
			// 8��ָʾ��������Ʒ�ʽ��ÿ0.5�����ң�ѭ�����ƶ�һ��
			/*
			temp=led[0];
			for (i=0;i<7;i++) led[i]=led[i+1];
			led[7]=temp;*/
		}
	}
}
