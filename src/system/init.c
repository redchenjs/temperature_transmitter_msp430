/*
 * init.c
 *
 *  Created on: 2018-02-10 16:37
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <inc/device/wdt.h>
#include <inc/device/ucs.h>
#include <inc/device/soft_i2c.h>
#include <inc/device/usci_a0_spi.h>
#include <inc/device/usci_b0_i2c.h>
#include <inc/device/usci_b1_spi.h>

#include <inc/driver/led.h>
#include <inc/driver/key.h>
#include <inc/driver/ssd1351.h>
#include <inc/driver/ads1118.h>
#include <inc/driver/dac8571.h>

void device_init(void)
{
    ucs_init();
    wdt_init();

    usci_a0_spi_init();
//    usci_b0_i2c_init();
    soft_i2c_init();
    usci_b1_spi_init();
}

void driver_init(void)
{
    led_init();
    key_init();

    ssd1351_init();
    dac8571_init();
    ads1118_init();
}
