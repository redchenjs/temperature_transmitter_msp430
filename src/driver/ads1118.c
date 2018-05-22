/*
 * ads1118.c
 *
 *  Created on: 2018-05-17 11:23
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <stdint.h>
#include <string.h>

#include <msp430.h>

#include <inc/driver/ads1118.h>
#include <inc/device/usci_b1_spi.h>

#define SS_NONE     0b0
#define SS_START    0b1

#define MUX_D0_1    0b000
#define MUX_D0_3    0b001
#define MUX_D1_3    0b010
#define MUX_D2_3    0b011
#define MUX_S0      0b100
#define MUX_S1      0b101
#define MUX_S2      0b110
#define MUX_S3      0b111

#define PGA_6_144   0b000
#define PGA_4_096   0b001
#define PGA_2_048   0b010
#define PGA_1_024   0b011
#define PGA_0_512   0b100
#define PGA_0_256   0b101

#define MODE_CONTINOUS  0b0
#define MODE_SINGLE     0b1

#define DR_8_SPS    0b000
#define DR_16_SPS   0b001
#define DR_32_SPS   0b010
#define DR_64_SPS   0b011
#define DR_128_SPS  0b100
#define DR_250_SPS  0b101
#define DR_475_SPS  0b110
#define DR_860_SPS  0b111

#define TS_MODE_ADC     0b0
#define TS_MODE_TEMP    0b1

#define PULL_UP_EN_OFF  0b0
#define PULL_UP_EN_ON   0b1

#define NOP_DATA_INVALID    0b00
#define NOP_DATA_VALID      0b01

#define RESERVED_BIT    0b1

static ads1118_t ads1118_dev;
ads1118_t *ads1118 = &ads1118_dev;

static int16_t ads1118_transmit(uint8_t *data)
{
    unsigned char tx_buff[4] = {0};
    unsigned char rx_buff[4] = {0};

    tx_buff[0] = data[1];
    tx_buff[1] = data[0];
    tx_buff[2] = 0;
    tx_buff[3] = 0;

    P4OUT &=~BIT0;
    usci_b1_spi_transmit_frame(tx_buff, rx_buff, 4);
    P4OUT |= BIT0;

    return rx_buff[0] << 8 | rx_buff[1];
}

void ads1118_set_channel(uint8_t channel)
{
    switch (channel) {
    case 0:
        ads1118->bits.mux = MUX_S0;
        break;
    case 1:
        ads1118->bits.mux = MUX_S1;
        break;
    case 2:
        ads1118->bits.mux = MUX_S2;
        break;
    case 3:
        ads1118->bits.mux = MUX_S3;
        break;
    default:
        return;
    }

    ads1118_transmit((uint8_t *)ads1118);
}

int16_t ads1118_convert(void)
{
    return ads1118_transmit((uint8_t *)"\x00\x00");
}

void ads1118_init(void)
{
    P4DIR |= BIT0;
    P4OUT |= BIT0;

    memset(&ads1118_dev, 0, sizeof(ads1118_dev));

    ads1118->bits.ss = SS_NONE;
    ads1118->bits.mux = MUX_S0;
    ads1118->bits.pga = PGA_2_048;
    ads1118->bits.mode = MODE_CONTINOUS;
    ads1118->bits.dr = DR_860_SPS;
    ads1118->bits.ts_mode = TS_MODE_ADC;
    ads1118->bits.pull_up_en = PULL_UP_EN_ON;
    ads1118->bits.nop = NOP_DATA_VALID;
    ads1118->bits.reserved = RESERVED_BIT;

    ads1118_transmit((uint8_t *)ads1118);
}
