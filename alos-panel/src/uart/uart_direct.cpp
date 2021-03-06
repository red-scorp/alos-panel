/*!
  \file uart_direct.cpp
  \brief ALoS-Panel Project direct UART code
  \author Andriy Golovnya (andriy.golovnya@googlemail.com)
*/

#include "../private.h"
#include "uart.h"

#if defined(UART_DIRECT)

#include <HardwareSerial.cpp>

/*! \brief Initialization of UART
 * - Set UART baud rate
 */
void uart_init() {
  Serial.begin(UART_BAUD);
}

/*! \brief Write byte to UART
 */
#undef uart_putch
uint8_t uart_putch(uint8_t txbyte) {
  return Serial.write(txbyte);
}

/*! \brief Read byte from UART
 * - Read UART if data available
 * - Start background task while waiting for data from UART
 */
#undef uart_getch
uint8_t uart_getch() {

  while(Serial.available() == 0) {
    los_yield();
  }

  return Serial.read();
}

/*! \brief Prepare data in UART to be read
 */
void uart_prefill() {
  /* Does nothing for direct UART */
}

#endif//UART_DIRECT
