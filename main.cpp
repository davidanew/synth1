#include "main.h"

int main () {
	
	uint64_t sample_tick_local = 0;
	double freq = 1000;
	Hal::init();
	Tim::init();
	double period = 1.0 / freq; 
	//double period = 0.01;
	uint32_t period_in_us = (uint32_t) ((double) 1000000 * period );
	uint32_t period_in_ticks = (uint32_t) ( period_in_us / Tim::sample_tick_us() );
	Dac dac1;
	Dac dac2;
	IRQ_objects::dac1_ptr = &dac1;
	dac1.init(DAC_CHANNEL_1);
	dac2.init(DAC_CHANNEL_2);

	while(1){
		if (IRQ_objects::sample_tick > sample_tick_local) {
			if (IRQ_objects::sample_tick - sample_tick_local > 1 ) {
				dac2.high();
			}
			else {
				dac2.low();
			}
			sample_tick_local = IRQ_objects::sample_tick;
			double phase_rel = (double)(sample_tick_local % period_in_ticks) / (double) period_in_ticks;
			double phase_radians = phase_rel * 6.283;
			//double phase_radians = phase_rel * 1.0;
			dac1.set_value_rel(0.5+0.5*sin(phase_radians));
			/*
			if (sample_tick_local % 100  >= 50) {
				//dac1.high();
				dac1.set_value_rel(1.0);
			}
			else {
				//dac1.low();
				dac1.set_value_rel(0.0);
			}
			*/
		}
	}
}
