/*
 * main.c
 *
 *  Created on: 2018-03-11 15:57
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <msp430.h>

#include <inc/system/init.h>
#include <inc/system/user.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    device_init();
    driver_init();

    user_init();

    while (1) {
        user_loop();

        __bis_SR_register(LPM0_bits + GIE);
    }
}
