/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Nov 14 07:57:37 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_user_btn.h>
#include <f3d_uart.h>
#include "queue.h"

volatile int systick_flag = 0;

void f3d_systick_init(int value) {
    SysTick_Config(SystemCoreClock/value);
}
int lednum = 8;
int off = 0;
int userbtn = 0;
void SysTick_Handler(void) {
        f3d_led_on(lednum);
        f3d_led_off(lednum-1);
        if(userbtn != user_btn_read()){
            userbtn = !userbtn;
            if(userbtn)
                f3d_systick_init(10);
            else
                f3d_systick_init(100);
        }
        lednum++;
        if(lednum > 16)
            lednum = 8;
}

/* f3d_systick.c ends here */
