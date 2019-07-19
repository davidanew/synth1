#include "Tests.h"

void Tests::uart(void) {
  //testing code
	Hal::init();
	//MIDI input
	Usart_1::init();
	//Virtual com port
	Usart_2_vcom::init();
	//Buffer used for recieve and transmit
	uint8_t buffer_arr[1] = {'X'};
	//Output midi to com port
	while (1){
		Usart_1::receive(buffer_arr, sizeof(buffer_arr));
		Usart_2_vcom::transmit(buffer_arr, sizeof(buffer_arr));
	}
}
