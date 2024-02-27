/*
 * sport.h
 *
 *  Created on: May 13, 2020
 *      Author: MrPurpleSocks
 * 		Modifed from original code by: Alka
 */


#include "main.h"

#ifndef SPORT_H_
#define SPORT_H_

void setup_uart();
void sendResponse();
void makeTelemPackage();
uint8_t calculateCheksum(uint8_t *data, uint8_t length)

void telem_UART_Init(void);
void send_telem_DMA();

void telem_UART_Init_CH4(void);
void send_telem_DMA_CH4();

#endif /* SERIAL_TELEMETRY_H_ */
