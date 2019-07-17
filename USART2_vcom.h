#include "stm32f4xx_hal.h"

class Usart_2_vcom {		
	public:
	UART_HandleTypeDef huart2 {};	
	Usart_2_vcom(void);
  void transmit(uint8_t* buffer_ptr, uint16_t size);
};

