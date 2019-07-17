#include "stm32f4xx_hal.h"

class Usart_1 {
	UART_HandleTypeDef huart1 ;
public:	
	Usart_1(void);
  void receive(uint8_t* buffer_ptr, uint16_t size);
};

