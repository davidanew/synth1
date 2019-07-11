/*This sets up the TIM2 timer*/

extern "C" {
	#include "stm32f4xx_hal.h"
}

//TODO: error on copy constructor

class Tim {
public:
	static TIM_HandleTypeDef htim2;
	static void init(void);
	static void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);
	static uint64_t sample_tick_us();
};

