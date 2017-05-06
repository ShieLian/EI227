#include <msp430g2553.h>
#include <controller.h>
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

}
/**
 * 控制字更新程序
 */
void update_remote_ctrl(){

}
