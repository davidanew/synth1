#include "IRQ.h"


void TIM2_IRQHandler_cpp(void){
	/*
	if (IRQ_objects::dac1_ptr->get_value() == 0) {
		IRQ_objects::dac1_ptr->high();
	}
	else {
		IRQ_objects::dac1_ptr->low();
	}
	*/
	IRQ_objects::sample_tick++;
	HAL_TIM_IRQHandler(&(Tim::htim2));
}


extern "C" {
	void SysTick_Handler(void)
	{
		HAL_IncTick();
	}
			
	void TIM2_IRQHandler(void)
	{
		TIM2_IRQHandler_cpp();
	}
}




