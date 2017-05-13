#ifndef	_TM1638_H
#define	_TM1638_H

#include  "msp430g2553.h"

#define DIO_L   P2OUT &= ~BIT5
#define DIO_H   P2OUT |=  BIT5
#define CLK_L   P2OUT &= ~BIT7
#define CLK_H   P2OUT |=  BIT7
#define STB_L   P2OUT &= ~BIT6
#define STB_H   P2OUT |=  BIT6
#define DIO_IN  P2DIR &= ~BIT5
#define DIO_OUT P2DIR |=  BIT5
#define DIO_DATA_IN    P2IN & BIT5
typedef short bool;
#define false 0
#define true 1

unsigned char TM1638_DigiSegment(unsigned char digit)
{
	unsigned char segment=0;
	switch (digit & 0x7F)
	{
	case 0:segment=0x3F;break;
	case 1:segment=0x06;break;
	case 2:segment=0x5B;break;
	case 3:segment=0x4F;break;
	case 4:segment=0x66;break;
	case 5:segment=0x6D;break;
	case 6:segment=0x7D;break;
	case 7:segment=0x07;break;
	case 8:segment=0x7F;break;
	case 9:segment=0x6F;break;
	case 10:segment=0x77;break;
	case 11:segment=0x7C;break;
	case 12:segment=0x39;break;
	case 13:segment=0x5E;break;
	case 14:segment=0x79;break;
	case 15:segment=0x71;break;
	case '_':segment=0x08;break;
	case '-':segment=0x40;break;
	case ' ':segment=0x00;break;
	case 'G':segment=0x3D;break;
	case 'A':segment=0x77;break;
	case 'I':segment=0x06;break;
	case 'N':segment=0x37;break;
	case 'F':segment=0x71;break;
	case 'U':segment=0x3E;break;
	case 'L':segment=0x38;break;
	case 'R':segment=0x50;break;
	case 'E':segment=0x79;break;
	case 'D':segment=0x5E;break;
	case 'Y':segment=0x6E;break;
	default:segment=0x00;break;
	}
	segment|= digit & 0x80;
	return segment;
}



void TM1638_Serial_Input(unsigned char	DATA)
{
	unsigned char i;
	DIO_OUT;
	for(i=0;i<8;i++)
	{
		CLK_L;
		if(DATA&0X01)
			DIO_H;
		else
			DIO_L;
		DATA>>=1;
		CLK_H;
	}
}


unsigned char TM1638_Serial_Output(void)
{
	unsigned char i;
	unsigned char temp=0;
	DIO_IN;
	for(i=0;i<8;i++)
	{
		temp>>=1;
		CLK_L;
		CLK_H;
		if((DIO_DATA_IN)!=0)
			temp|=0x80;
	}
	return temp;
}


unsigned char TM1638_Readkeyboard(void)
{
	unsigned char c[4],i,key_code=0;
	STB_L;
	TM1638_Serial_Input(0x42);
	__delay_cycles(10);
	for(i=0;i<4;i++)
		c[i]=TM1638_Serial_Output();
	STB_H;

	if(c[0]==0x04) key_code=1;
	if(c[0]==0x40) key_code=2;
	if(c[1]==0x04) key_code=3;
	if(c[1]==0x40) key_code=4;
	if(c[2]==0x04) key_code=5;
	if(c[2]==0x40) key_code=6;
	if(c[3]==0x04) key_code=7;
	if(c[3]==0x40) key_code=8;
	if(c[0]==0x02) key_code=9;
	if(c[0]==0x20) key_code=10;
	if(c[1]==0x02) key_code=11;
	if(c[1]==0x20) key_code=12;
	if(c[2]==0x02) key_code=13;
	if(c[2]==0x20) key_code=14;
	if(c[3]==0x02) key_code=15;
	if(c[3]==0x20) key_code=16;
	return key_code;
}

unsigned short TM1638_Readkeyboard_bit(void)
{
    unsigned char c[4],i;
    unsigned short key_code=0;
    STB_L;
    TM1638_Serial_Input(0x42);
    __delay_cycles(10);
    for(i=0;i<4;i++)
        c[i]=TM1638_Serial_Output();
    STB_H;

    if(c[0]==0x04) key_code|=1;
    if(c[0]==0x40) key_code|=2;
    if(c[1]==0x04) key_code|=4;
    if(c[1]==0x40) key_code|=8;
    if(c[2]==0x04) key_code|=0x10;
    if(c[2]==0x40) key_code|=0x20;
    if(c[3]==0x04) key_code|=0x40;
    if(c[3]==0x40) key_code|=0x80;
    if(c[0]==0x02) key_code|=0x100;
    if(c[0]==0x20) key_code|=0x200;
    if(c[1]==0x02) key_code|=0x400;
    if(c[1]==0x20) key_code|=0x800;
    if(c[2]==0x02) key_code|=0x1000;
    if(c[2]==0x20) key_code|=0x2000;
    if(c[3]==0x02) key_code|=0x4000;
    if(c[3]==0x20) key_code|=0x8000;
    return key_code;
}


void TM1638_RefreshDIGIandLED(unsigned char digit_buf[8],unsigned char pnt_buf,unsigned char led_buf[8])
{
	unsigned char i,mask,buf[16];

	mask=0x01;
	for(i=0;i<8;i++)
	{
		buf[i*2]=TM1638_DigiSegment(digit_buf[i]);
		if ((pnt_buf&mask)!=0) buf[i*2]|=0x80;
		mask=mask*2;

		buf[i*2+1]=led_buf[i];
	}

	STB_L;TM1638_Serial_Input(0x40);STB_H;
	STB_L;TM1638_Serial_Input(0xC0);
	for (i=0;i<16;i++)
	{
		TM1638_Serial_Input(buf[i]);
	}
	STB_H;
}

void TM1638_RefreshDIGIandLED_raw(unsigned char value[8],unsigned char pnt_buf,unsigned char led_buf[8])
{
    unsigned char i,mask,buf[16];

    mask=0x01;
    for(i=0;i<8;i++)
    {
        buf[i*2]=value[i];
        if ((pnt_buf&mask)!=0) buf[i*2]|=0x80;
        mask=mask*2;

        buf[i*2+1]=led_buf[i];
    }

    STB_L;TM1638_Serial_Input(0x40);STB_H;
    STB_L;TM1638_Serial_Input(0xC0);
    for (i=0;i<16;i++)
    {
        TM1638_Serial_Input(buf[i]);
    }
    STB_H;
}

void init_TM1638(void)
{
	STB_L;TM1638_Serial_Input(0x8A);STB_H;
}
#endif
