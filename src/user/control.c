/*
 * control.c
 *
 *  Created on: 2018-05-17 14:59
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#include "core/lut.h"

#include "board/led.h"
#include "board/dac8571.h"

#include "user/measure.h"
#include "user/control.h"
#include "user/display.h"

static control_data_t control_data = {
    .mode = 0,
    .dac0_data = 0,
    .dac0_voltage = 0.0,
    .min_temp = 0,
    .max_temp = 200,
    .resistance = 0.0,
    .temperature = 0.0,
};
control_data_t *control = &control_data;

const double ratio_x_2 = 0.0000000000317446030908013;
const double ratio_x_1 =  0.0100595137756586;
const double ratio_const = 0.200293818184736;

void control_update(void)
{
    switch (control->mode) {
    case 1: {
// ---------------------ADC => Res-----------------------
        control->resistance = ratio_x_2 * measure->adc0_data * measure->adc0_data + ratio_x_1 * measure->adc0_data + ratio_const;
// ---------------------Res => Temp----------------------
        if (control->resistance > 313.8) {
            control->temperature = 600.0;
        } else if (control->resistance > 297.487) {
            control->temperature = (19650 - sqrt(445827300 - 600000 * control->resistance)) / 6;
        } else if (control->resistance > 264.179) {
            control->temperature = (19650 - sqrt(445809300 - 600000 * control->resistance)) / 6;
        } else if (control->resistance > 229.716) {
            control->temperature = (77800 - sqrt(6935409600 - 8800000 * control->resistance)) / 22;
        } else if (control->resistance > 194.098) {
            control->temperature = (19600 - sqrt(444071200 - 600000 * control->resistance)) / 6;
        } else if (control->resistance > 157.325) {
            control->temperature = (19600 - sqrt(444073600 - 600000 * control->resistance)) / 6;
        } else if (control->resistance > 128.987) {
            control->temperature = (19522 - sqrt(441151900 - 600000 * control->resistance)) / 6;
        } else if (control->resistance > 100.0) {
            control->temperature = (19522 - sqrt(441119200 - 600000 * control->resistance)) / 6;
        } else if (control->resistance > 80.306) {
            control->temperature = (19550 - sqrt(442205200 - 600000 * control->resistance)) / 6;
        } else if (control->resistance > 68.325) {
            control->temperature = 0.0009 * control->resistance * control->resistance + 2.3718 * control->resistance - 246.1989 - 0.062;
        } else if (control->resistance > 56.193) {
            control->temperature = 0.0014 * control->resistance * control->resistance + 2.2950 * control->resistance - 243.4712 + 0.09;
        } else if (control->resistance > 43.876) {
            control->temperature = 0.0017 * control->resistance * control->resistance + 2.2674 * control->resistance - 242.7183 - 0.04;
        } else if (control->resistance > 31.335) {
            control->temperature = 0.0018 * control->resistance * control->resistance + 2.2579 * control->resistance - 242.4967 - 0.03;
        } else if (control->resistance > 18.520) {
            control->temperature = 0.0021 * control->resistance * control->resistance + 2.2355 * control->resistance - 242.1329 + 0.015;
        } else {
            control->temperature = -200.0;
        }
// -------------------Temp => Voltage--------------------
        if (display->mode == 1) {
            if ((control->max_temp - control->min_temp) > 0) {
                if ((control->temperature - control->min_temp) > 0 && (control->max_temp - control->temperature) > 0) {
                    uint16_t dac0_vi_idx = (uint16_t)((control->temperature - control->min_temp) / (control->max_temp - control->min_temp) * 800);
                    control->dac0_voltage = dac0_vi_table[dac0_vi_idx];
                } else if ((control->temperature - control->min_temp) <= 0) {
                    control->dac0_voltage = dac0_vi_table[0];
                } else if ((control->max_temp - control->temperature) <= 0) {
                    control->dac0_voltage = dac0_vi_table[800];
                }
            } else {
                control->dac0_voltage = 0.0;
            }
        }
// --------------------Voltage => DAC--------------------
        control->dac0_data = (double)control->dac0_voltage * 65535 / 3.0;
        dac8571_set_output(control->dac0_data);

        control->mode = 0;
        break;
    }
    default: {
        static uint32_t count = 0;
        if (count++ % 2) {
            led_on();
        } else {
            led_off();
        }
        break;
    }
    }
}
