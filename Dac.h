#ifndef DAC_H
#define DAC_H 1
extern "C" {
	#include "stm32f4xx_hal.h"
}

class Dac {
	DAC_HandleTypeDef hdac {};
	uint32_t channel = DAC_CHANNEL_1;
	bool init_done = false;
	
//need move construtor that goes to error	
		
public:	
//	Dac();
	void init(uint32_t channel); 
  void set_value_rel(double value_rel);
  void set_value(uint32_t value_rel);
	void high();
	void low();
	uint32_t get_value();
	void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);	
};

#endif

