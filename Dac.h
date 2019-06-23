#ifndef DAC_H
#define DAC_H 1
extern "C" {
	#include "stm32f4xx_hal.h"
}

class Dac {
	DAC_HandleTypeDef hdac ;
	uint32_t channel ;
	bool init_done;
	
public:	
	Dac();
	void init(uint32_t channel); 	
	void high();
	void low();
	uint32_t get_value();
	void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);	
};

#endif

