#include <msp430g2553.h>
#include <controller.h>

typedef short bool;
#define false 0
#define true 1

int pulse_counter=0;
bool pulse_done=false;
const int threshold=10;//!
/**
 * 初始化
 */

void init_remote_control(){
    set_input(P1,BIT7);
}
/**
 * 20ms中断更新程序
 */
void update_remote_inter(){
    if(P1IN & BIT7){
        ++pulse_counter;
    } else if(pulse_counter!=0) {
        pulse_done=true;
    }
}
/**
 * 控制字更新程序
 */
void update_remote_ctrl(){
    if(pulse_done){
        if(pulse_counter<threshold){
            --level;
            if(level==0)
                level=15;
        } else {
            ++level;
            if(level==16)
                level=1;
        }
        pulse_counter=0;
        pulse_done=false;
    }
}
