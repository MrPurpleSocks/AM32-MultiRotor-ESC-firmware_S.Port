/*
 * sport.c
 *
 *      Author: MrPurpleSocks
 * 		Modifed from original code by: Alka
 */


#include "sport.h"
#include "targets.h"
#include "stm32f0xx.h"
#include <stdint.h>

uint8_t aTxBuffer[3];
uint16_t dataTypeIDs[3] = {0x0200, 0x0600, 0x0500};
uint8_t currentValue = 0;

#ifdef USE_SPORT
#endif

void setup_uart() {
  // Enable USART1
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  // Configure USART1
  USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; // Enable USART, transmitter and receiver
  USART1->CR2 = USART_CR2_STOP_1; // 2 stop bits
  USART1->CR3 = 0; // No flow control
  USART1->BRR = SystemCoreClock / 57600; // Baud rate 57600

}

void sendResponse(uint32_t current, uint32_t consumption, uint32_t e_rpm) {
  makeTelemPackage(current, consumption, e_rpm);
  // listen for polling request
  while (!(USART->ISR & USART_ISR_RXNE)); // wait for data
  uint8_t buffer = USART->RDR; // read data

  // check if polling request is for esc
  if (buffer == 0x01) {
    uint32_t response[4] = {0x10, dataTypeIDs[currentValue], aTxBuffer[currentValue], calculateCheksum(aTxBuffer, 3)};
    if (currentValue == 2) {
      currentValue = 0;
    } else {
      currentValue++;
    }
    for (int i=0; i<4; i++) {
      while (!(USART1->ISR & USART_ISR_TXE)); // wait for transmit data register empty
      USART->TDR = response[i]; // send data
    }
  }
}
void makeTelemPackage(){
  uint16_t amps = (uint16_t)(amps_draw * 100);

  aTxBuffer[0] = amps & 0xFF; //ESC Current (Amps)
  aTxBuffer[1] = e_current & 0xFF; //ESC Consumption (mAh)
  aTxBuffer[2] = e_rpm & 0xFF; //ESC RPM
}

uint8_t calculateCheksum(uint8_t *data, uint8_t length){
  uint8_t checksum = 0;
  for (size_t i = 0; i < length; i++) {
    checksum ^= data[i];
  }
  return checksum;
}

int main() {
  setup_uart();
  while(1) {
    sendResponse(0, 0, 0);
  }
  return 0;
}

