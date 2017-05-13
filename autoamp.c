#include <msp430g2553.h>
#include <controller.h>

typedef short bool;
#define false 0
#define true 1

bool enautoamp=false;
bool update=false;
double volt=0.0;
//P1.5 A5
//key 5
void init_adc(){
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE;
    ADC10CTL0|=SREF_0; // ADC10ON, interrupt enabled,16*ADC41CLKs
    ADC10CTL1 = INCH_5;                       // input A1
    ADC10AE0 |= BIT5;                         // PA.1 ADC option select
    P1DIR |= BIT6;                            // Set P1.0 to output direction
    P1OUT |= BIT6;
}

int interval=0;
void update_autoamp_inter(){
     if(!enautoamp)
        return;
    if(interval>0){
        --interval;
        return;
    }
    if(volt>2.25){//4.8V
        if(level!=1){
            --level;
        }
        led[7]=(unsigned char)2;
    } else if(volt < 0.02) {//100mVp
        if(level!=15){
            ++level;
        }
        led[7]=(unsigned char)3;
    } else {
        led[7]=(unsigned char)1;
    }
    interval=5;
}

void update_en(){
    if(!update && key_code==5){
        update=true;
        enautoamp=!enautoamp;
    } else if(key_code!=5){
        update=false;
    }
}

void update_autoamp(){
    update_en();
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversionstart
    //__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    if (ADC10MEM < 0x1FF)
        P1OUT &= ~BIT6;                       // Clear P1.0 LED off
    else
        P1OUT |= BIT6;                        // Set P1.0 LED on
        volt=((int)ADC10MEM)*3.55/1023;
    if(!enautoamp){
        led[7]=0;
    }
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void){
  //__bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
