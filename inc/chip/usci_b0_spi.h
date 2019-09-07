/*
 * usci_b0_spi.h
 *
 *  Created on: 2016-06-25
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef INC_CHIP_USCI_B0_SPI_H_
#define INC_CHIP_USCI_B0_SPI_H_

extern unsigned char usci_b0_transmit_frame(unsigned char *p_buff, unsigned int num);
extern unsigned char usci_b0_receive_frame(unsigned char *p_buff, unsigned int num);

extern void usci_b0_spi_tx_isr_handle(void);
extern void usci_b0_spi_rx_isr_handle(void);

extern void usci_b0_init(void);

#endif /* INC_CHIP_USCI_B0_SPI_H_ */
