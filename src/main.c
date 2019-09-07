/*
 * main.c
 *
 *  Created on: 2018-03-11 15:57
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <msp430.h>

#include "chip/wdt.h"
#include "chip/ucs.h"
#include "chip/soft_i2c.h"
#include "chip/usci_a0_spi.h"
#include "chip/usci_b0_i2c.h"
#include "chip/usci_b1_spi.h"

#include "board/led.h"
#include "board/key.h"
#include "board/ssd1351.h"
#include "board/ads1118.h"
#include "board/dac8571.h"

#include "user/input.h"
#include "user/measure.h"
#include "user/control.h"
#include "user/display.h"

static void chip_init(void)
{
    ucs_init();
    wdt_init();

    usci_a0_spi_init();
    soft_i2c_init();
    usci_b1_spi_init();
}

static void board_init(void)
{
    led_init();
    key_init();

    ssd1351_init();
    dac8571_init();
    ads1118_init();
}

static void user_init(void) {}

static void user_loop(void)
{
    input_update();
    measure_update();
    control_update();
    display_update();
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    chip_init();
    board_init();

    user_init();

    while (1) {
        user_loop();

        __bis_SR_register(LPM0_bits + GIE);
    }
}
