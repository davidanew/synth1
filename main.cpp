#include "main.h"

#include<stdlib.h>

int main () {
	uint64_t sample_tick_local = 0;
	const double freq = 1000;

	//Wave* sine = new Sine;
	//Wave* square = new Square;
	
	volatile uint32_t* ptr_1 {nullptr};
	volatile uint32_t* ptr_2 {nullptr};
	
	ptr_1 = new uint32_t[19];
	ptr_2 = new uint32_t[19];
	
	ptr_1[0] = 0;
	ptr_2[0] = 0;
	
	
	ptr_1[0] ++;
	ptr_2[0] ++;
	
	
	
	
	Sine sine;
	Square square;

	


	/*
		
	uint32_t* testptr {nullptr};
	testptr = new uint32_t[4096];
//	size_t entry_size = 4;
//	volatile size_t mem_size = 1 * entry_size ;
	//testptr = (uint32_t*) calloc (1,1000);
	testptr[0] = 1;
	testptr[0]++;
	
	
	*/
	
	Hal::init();
	Tim::init();
	double period = 1.0 / freq; 
	uint32_t period_in_us = (uint32_t) ((double) 1000000 * period );
	uint32_t period_in_ticks = (uint32_t) ( period_in_us / Tim::sample_tick_us() );
	Dac dac1;
	Dac dac2_led;
	dac1.init(DAC_CHANNEL_1);
	dac2_led.init(DAC_CHANNEL_2);
	IRQ_objects::dac1_ptr = &dac1;
	while(1){
		if (IRQ_objects::sample_tick > sample_tick_local) {
			if (IRQ_objects::sample_tick - sample_tick_local > 1 ) {
				dac2_led.high();
			}
			else {
				dac2_led.low();
			}
			sample_tick_local = IRQ_objects::sample_tick;
			float phase_rel = (float)(sample_tick_local % period_in_ticks) / (float) period_in_ticks;
			uint32_t dac_value =  square.get_value(phase_rel)*0.5+0.5*sine.get_value(phase_rel); 			

			dac1.set_value(dac_value);

		}
	}
}
