/*
 * input.c
 *
 *  Created on: 2018-05-19 13:46
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <stdint.h>
#include <msp430.h>

#include <inc/user/measure.h>
#include <inc/user/control.h>
#include <inc/user/display.h>

void input0_handle(void)
{
    switch (display->mode) {
    case 1:
        display->mode = 2;
        display->index = 0b001;
        display->flag = 1;
        break;
    case 2:
        if (display->index == 0b100) {
            display->index = 0b000;
            display->mode = 1;
        } else {
            display->index <<= 1;
        }
        display->flag = 1;
        break;
    case 3:
        display->mode = 1;
        display->flag = 1;
        break;
    default:
        break;
    }
}

void input1_handle(void)
{
    switch (display->mode) {
    case 2:
        display->mode = 3;
        display->selected = 0b001;
        display->flag = 1;
        break;
    case 3:
        if (display->index == 0b001) {
            if (display->selected == 0b10000) {
                display->selected = 0b00001;
            } else {
                display->selected <<= 1;
            }
        } else {
            if (display->selected == 0b100) {
                display->selected = 0b001;
            } else {
                display->selected <<= 1;
            }
        }
        display->flag = 1;
        break;
    default:
        break;
    }
}

void input2_handle(void)
{
    if (display->mode == 3) {
        switch (display->index) {
        case 0b001:
            if (display->selected & BIT0) {
                control->dac0_voltage -= 1.0;
            }
            if (display->selected & BIT1) {
                control->dac0_voltage -= 0.1;
            }
            if (display->selected & BIT2) {
                control->dac0_voltage -= 0.01;
            }
            if (display->selected & BIT3) {
                control->dac0_voltage -= 0.001;
            }
            if (display->selected & BIT4) {
                control->dac0_voltage -= 0.0001;
            }
            if (control->dac0_voltage < 0.0) {
                control->dac0_voltage = 0.0;
            }
            break;
        case 0b010:
            if (display->selected & BIT0) {
                control->min_temp -= 100;
            }
            if (display->selected & BIT1) {
                control->min_temp -= 10;
            }
            if (display->selected & BIT2) {
                control->min_temp -= 1;
            }
            if (control->min_temp < -200) {
                control->min_temp = -200;
            }
            break;
        case 0b100:
            if (display->selected & BIT0) {
                control->max_temp -= 100;
            }
            if (display->selected & BIT1) {
                control->max_temp -= 10;
            }
            if (display->selected & BIT2) {
                control->max_temp -= 1;
            }
            if (control->max_temp < -200) {
                control->max_temp = -200;
            }
            if (control->max_temp < control->min_temp) {
                control->max_temp = control->min_temp;
            }
            break;
        default:
            break;
        }
        control->mode = 1;
        display->flag = 1;
    }
}

void input3_handle(void)
{
    if (display->mode == 3) {
        switch (display->index) {
        case 0b001:
            if (display->selected & BIT0) {
                control->dac0_voltage += 1.0;
            }
            if (display->selected & BIT1) {
                control->dac0_voltage += 0.1;
            }
            if (display->selected & BIT2) {
                control->dac0_voltage += 0.01;
            }
            if (display->selected & BIT3) {
                control->dac0_voltage += 0.001;
            }
            if (display->selected & BIT4) {
                control->dac0_voltage += 0.0001;
            }
            if (control->dac0_voltage > 3.0) {
                control->dac0_voltage = 3.0;
            }
            break;
        case 0b010:
            if (display->selected & BIT0) {
                control->min_temp += 100;
            }
            if (display->selected & BIT1) {
                control->min_temp += 10;
            }
            if (display->selected & BIT2) {
                control->min_temp += 1;
            }
            if (control->min_temp > 600) {
                control->min_temp = 600;
            }
            if (control->min_temp > control->max_temp) {
                control->min_temp = control->max_temp;
            }
            break;
        case 0b100:
            if (display->selected & BIT0) {
                control->max_temp += 100;
            }
            if (display->selected & BIT1) {
                control->max_temp += 10;
            }
            if (display->selected & BIT2) {
                control->max_temp += 1;
            }
            if (control->max_temp > 600) {
                control->max_temp = 600;
            }
            break;
        default:
            break;
        }
        control->mode = 1;
        display->flag = 1;
    }
}

void input_update(void)
{
    static uint8_t count[6] = {0};

    if (!(P2IN & BIT0)) {
        if (count[0]++ == 1) {
            count[0] = 0;
            input0_handle();
        }
    } else if (!(P2IN & BIT2)) {
        if (count[1]++ == 1) {
            count[1] = 0;
            input1_handle();
        }
    } else if (!(P2IN & BIT4)) {
        if (count[2]++ == 1) {
            count[2] = 0;
            input2_handle();
        }
    } else if (!(P2IN & BIT5)) {
        if (count[3]++ == 1) {
            count[3] = 0;
            input3_handle();
        }
    }
}
