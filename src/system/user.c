/*
 * user.c
 *
 *  Created on: 2018-02-16 18:00
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <inc/user/input.h>
#include <inc/user/measure.h>
#include <inc/user/control.h>
#include <inc/user/display.h>

void user_init(void)
{

}

void user_loop(void)
{
    input_update();
    measure_update();
    control_update();
    display_update();
}
