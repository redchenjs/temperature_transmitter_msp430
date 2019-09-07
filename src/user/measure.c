/*
 * control.c
 *
 *  Created on: 2018-05-17 14:59
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <stdio.h>
#include <stdint.h>

#include "board/ads1118.h"

#include "user/measure.h"
#include "user/control.h"
#include "user/display.h"

static measure_data_t measure_data = {
    .mode = 1,
    .adc0_data = 0,
    .adc1_data = 0,
    .adc0_voltage = 0.0,
    .adc1_voltage = 0.0
};
measure_data_t *measure = &measure_data;

static double vref[] = {6.144, 4.096, 2.048, 1.024, 0.512, 0.256};

void measure_update(void)
{
    switch (measure->mode) {
    case 1: {
        static uint32_t count = 0;
        static int32_t adc0_sum = 0;
        int16_t data = ads1118_convert();
        adc0_sum += data;
        if (count++ == 63) {
            count = 0;
            measure->adc0_data = adc0_sum / 64;
            measure->adc0_voltage = measure->adc0_data * vref[ads1118->bits.pga] / 32768;
            adc0_sum = 0;
            ads1118_set_channel(1);
            measure->mode = 2;
            control->mode = 1;
            display->flag = 1;
        }
        break;
    }
    case 2: {
        static uint32_t count = 0;
        static int32_t adc1_sum = 0;
        int16_t data = ads1118_convert();
        adc1_sum += data;
        if (count++ == 63) {
            count = 0;
            measure->adc1_data = adc1_sum / 64;
            measure->adc1_voltage = measure->adc1_data * vref[ads1118->bits.pga] / 32768;
            adc1_sum = 0;
            ads1118_set_channel(0);
            measure->mode = 1;
            control->mode = 1;
            display->flag = 1;
        }
        break;
    }
    default:
        break;
    }
}
