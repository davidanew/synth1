#ifndef USART1_H
#define USART1_H 1

#include "stm32f4xx_hal.h"

class Usart_1 {
	static UART_HandleTypeDef huart1 ;
public:	
	static void init (void);
  static void receive(uint8_t* buffer_ptr, uint16_t size);
};

#endif
