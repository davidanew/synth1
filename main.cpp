#include "main.h"

#include<stdlib.h>

int main () {
	uint64_t sample_tick_local = 0;
	const double freq = 1000;
	Wave* square = new Square();

		
	volatile uint32_t* testptr {nullptr};
	//testptr = new uint32_t[4096];
//	size_t entry_size = 4;
//	volatile size_t mem_size = 1 * entry_size ;
	testptr = (uint32_t*) calloc (4,4000);
	testptr[0] = 1;
	testptr[0]++;
	
	
	
	
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
			uint32_t dac_value = square->get_value(phase_rel);			
			//double phase_radians = phase_rel * 6.283;
			dac1.set_value_rel(dac_value);
		}
	}
}
