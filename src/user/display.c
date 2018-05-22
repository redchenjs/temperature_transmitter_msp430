/*
 * display.c
 *
 *  Created on: 2018-02-13 22:57
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <msp430.h>

#include <inc/driver/ssd1351.h>
#include <inc/system/fonts.h>

#include <inc/user/measure.h>
#include <inc/user/control.h>
#include <inc/user/display.h>

static display_data_t display_data = {
    .flag = 2,
    .mode = 1,
    .index = 0b000,
    .selected = 0b00000
};
display_data_t *display = &display_data;

void display_update(void)
{
    char str_buff[24] = {0};

    switch (display->mode) {
    case 1: {
        if (display->flag == 2) {
            snprintf(str_buff, sizeof(str_buff), "Res:  %6.1f Ohm", control->resistance);
            ssd1351_display_string(0, 0, str_buff, FONT_1608, Lime, Black);
            snprintf(str_buff, sizeof(str_buff), "Temp: %6.1f C", control->temperature);
            ssd1351_display_string(0, 24, str_buff, FONT_1608, Yellow, Black);
            snprintf(str_buff, sizeof(str_buff), "Out:  %6.4f V", control->dac0_voltage);
            ssd1351_display_string(0, 48, str_buff, FONT_1608, Red, Black);

            ssd1351_draw_h_line(0, 70, 128, Magenta);
            ssd1351_display_string(50, 72, "Temp", FONT_1608, White, Black);
            ssd1351_display_string(16, 92, "MIN", FONT_1608, White, Black);
            ssd1351_display_string(96, 92, "MAX", FONT_1608, White, Black);

            snprintf(str_buff, sizeof(str_buff), "%4d", control->min_temp);
            ssd1351_display_string(8, 112, str_buff, FONT_1608, Magenta, Black);
            snprintf(str_buff, sizeof(str_buff), "%4d", control->max_temp);
            ssd1351_display_string(88, 112, str_buff, FONT_1608, Magenta, Black);
        } else if (display->flag == 1) {
            snprintf(str_buff, sizeof(str_buff), "%6.1f", control->resistance);
            ssd1351_display_string(48, 0, str_buff, FONT_1608, Lime, Black);
            snprintf(str_buff, sizeof(str_buff), "%6.1f", control->temperature);
            ssd1351_display_string(48, 24, str_buff, FONT_1608, Yellow, Black);

            snprintf(str_buff, sizeof(str_buff), "%6.4f", control->dac0_voltage);
            ssd1351_display_string(48, 48, str_buff, FONT_1608, Red, Black);
            snprintf(str_buff, sizeof(str_buff), "%4d", control->min_temp);
            ssd1351_display_string(8, 112, str_buff, FONT_1608, Magenta, Black);
            snprintf(str_buff, sizeof(str_buff), "%4d", control->max_temp);
            ssd1351_display_string(88, 112, str_buff, FONT_1608, Magenta, Black);
        }

        display->flag = 0;
        break;
    }
    case 2: {
        if (display->flag == 1) {
            snprintf(str_buff, sizeof(str_buff), "%6.1f", control->resistance);
            ssd1351_display_string(48, 0, str_buff, FONT_1608, Lime, Black);
            snprintf(str_buff, sizeof(str_buff), "%6.1f", control->temperature);
            ssd1351_display_string(48, 24, str_buff, FONT_1608, Yellow, Black);

            snprintf(str_buff, sizeof(str_buff), "%6.4f", control->dac0_voltage);
            if (display->index & BIT0) {
                ssd1351_display_string(48, 48, str_buff, FONT_1608, Black, Red);
            } else {
                ssd1351_display_string(48, 48, str_buff, FONT_1608, Red, Black);
            }

            snprintf(str_buff, sizeof(str_buff), "%4d", control->min_temp);
            if (display->index & BIT1) {
                ssd1351_display_string(8, 112, str_buff, FONT_1608, Black, Magenta);
            } else {
                ssd1351_display_string(8, 112, str_buff, FONT_1608, Magenta, Black);
            }

            snprintf(str_buff, sizeof(str_buff), "%4d", control->max_temp);
            if (display->index & BIT2) {
                ssd1351_display_string(88, 112, str_buff, FONT_1608, Black, Magenta);
            } else {
                ssd1351_display_string(88, 112, str_buff, FONT_1608, Magenta, Black);
            }
        }

        display->flag = 0;
        break;
    }
    case 3: {
        if (display->flag == 1) {
            snprintf(str_buff, sizeof(str_buff), "%6.1f", control->resistance);
            ssd1351_display_string(48, 0, str_buff, FONT_1608, Lime, Black);
            snprintf(str_buff, sizeof(str_buff), "%6.1f", control->temperature);
            ssd1351_display_string(48, 24, str_buff, FONT_1608, Yellow, Black);

            snprintf(str_buff, sizeof(str_buff), "%6.4f", control->dac0_voltage);
            if (display->index & BIT0) {
                if (display->selected & BIT0) {
                    ssd1351_display_num(48, 48, ((uint8_t)control->dac0_voltage % 10), 1, FONT_1608, Black, Red);
                } else {
                    ssd1351_display_num(48, 48, ((uint8_t)control->dac0_voltage % 10), 1, FONT_1608, Red, Black);
                }
                ssd1351_display_char(56, 48, '.', FONT_1608, Red, Black);
                if (display->selected & BIT1) {
                    ssd1351_display_num(64, 48, ((uint16_t)(control->dac0_voltage * 10) % 10), 1, FONT_1608, Black, Red);
                } else {
                    ssd1351_display_num(64, 48, ((uint16_t)(control->dac0_voltage * 10) % 10), 1, FONT_1608, Red, Black);
                }
                if (display->selected & BIT2) {
                    ssd1351_display_num(72, 48, ((uint16_t)(control->dac0_voltage * 100) % 10), 1, FONT_1608, Black, Red);
                } else {
                    ssd1351_display_num(72, 48, ((uint16_t)(control->dac0_voltage * 100) % 10), 1, FONT_1608, Red, Black);
                }
                if (display->selected & BIT3) {
                    ssd1351_display_num(80, 48, ((uint16_t)(control->dac0_voltage * 1000) % 10), 1, FONT_1608, Black, Red);
                } else {
                    ssd1351_display_num(80, 48, ((uint16_t)(control->dac0_voltage * 1000) % 10), 1, FONT_1608, Red, Black);
                }
                if (display->selected & BIT4) {
                    ssd1351_display_num(88, 48, ((uint16_t)(control->dac0_voltage * 10000) % 10), 1, FONT_1608, Black, Red);
                } else {
                    ssd1351_display_num(88, 48, ((uint16_t)(control->dac0_voltage * 10000) % 10), 1, FONT_1608, Red, Black);
                }
            } else {
                ssd1351_display_string(48, 48, str_buff, FONT_1608, Red, Black);
            }

            snprintf(str_buff, sizeof(str_buff), "%4d", control->min_temp);
            if (display->index & BIT1) {
                if (control->min_temp < 0) {
                    ssd1351_display_char(8, 112, '-', FONT_1608, Magenta, Black);
                } else {
                    ssd1351_display_char(8, 112, ' ', FONT_1608, Black, Black);
                }
                if (display->selected & BIT0) {
                    ssd1351_display_num(16, 112, (abs(control->min_temp) / 100 % 10), 1, FONT_1608, Black, Magenta);
                } else {
                    ssd1351_display_num(16, 112, (abs(control->min_temp) / 100 % 10), 1, FONT_1608, Magenta, Black);
                }
                if (display->selected & BIT1) {
                    ssd1351_display_num(24, 112, (abs(control->min_temp) / 10 % 10), 1, FONT_1608, Black, Magenta);
                } else {
                    ssd1351_display_num(24, 112, (abs(control->min_temp) / 10 % 10), 1, FONT_1608, Magenta, Black);
                }
                if (display->selected & BIT2) {
                    ssd1351_display_num(32, 112, (abs(control->min_temp) % 10), 1, FONT_1608, Black, Magenta);
                } else {
                    ssd1351_display_num(32, 112, (abs(control->min_temp) % 10), 1, FONT_1608, Magenta, Black);
                }
            } else {
                ssd1351_display_string(8, 112, str_buff, FONT_1608, Magenta, Black);
            }

            snprintf(str_buff, sizeof(str_buff), "%4d", control->max_temp);
            if (display->index & BIT2) {
                if (control->max_temp < 0) {
                    ssd1351_display_char(88, 112, '-', FONT_1608,Magenta, Black);
                } else {
                    ssd1351_display_char(88, 112, ' ', FONT_1608, Black, Black);
                }
                if (display->selected & BIT0) {
                    ssd1351_display_num(96, 112, (abs(control->max_temp) / 100 % 10), 1, FONT_1608, Black, Magenta);
                } else {
                    ssd1351_display_num(96, 112, (abs(control->max_temp) / 100 % 10), 1, FONT_1608, Magenta, Black);
                }
                if (display->selected & BIT1) {
                    ssd1351_display_num(104, 112, (abs(control->max_temp) / 10 % 10), 1, FONT_1608, Black, Magenta);
                } else {
                    ssd1351_display_num(104, 112, (abs(control->max_temp) / 10 % 10), 1, FONT_1608, Magenta, Black);
                }
                if (display->selected & BIT2) {
                    ssd1351_display_num(112, 112, (abs(control->max_temp) % 10), 1, FONT_1608, Black, Magenta);
                } else {
                    ssd1351_display_num(112, 112, (abs(control->max_temp) % 10), 1, FONT_1608, Magenta, Black);
                }
            } else {
                ssd1351_display_string(88, 112, str_buff, FONT_1608, Magenta, Black);
            }
        }

        display->flag = 0;
        break;
    }
    default:
        break;
    }
}
