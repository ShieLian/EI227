#include <msp430g2553.h>
#include <controller.h>

typedef short bool;
#define false 0
#define true 1

int pulse_counter=0;
bool pulse_done=false;
int threshold=10;//!
/**
 * ��ʼ��
 */

void init_remote_control(){
    set_input(P1,BIT7);
}
/**
 * 20ms�жϸ��³���
 */
void update_remote_inter(){
    if(P1IN & BIT7){
        ++pulse_counter;
    } else if(pulse_counter!=0) {
        pulse_done=true;
    }
}
/**
 * �����ָ��³���
 */
void update_remote_ctrl(){
    if(pulse_done){
        if(pulse_counter<threshold){
            --level;
        } else {
            ++level;
        }
        pulse_counter=0;
        pulse_done=false;
    }
}
