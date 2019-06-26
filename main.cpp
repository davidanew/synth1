#include "main.h"

//#include<stdlib.h>

int main () {
	uint64_t sample_tick_local = 0;
	const float freq = 10;
	const Sine sine;
	const Square square;
	
	Hal::init();
	Tim::init();
	float period = (float) 1.0 / freq; 
	float period_in_us = 1000000 * period ;
	float period_in_ticks = period_in_us / Tim::sample_tick_us() ;
	
	/*
	4000 samples for 40k
	
	4000 samples for 
	
	25 us period, sample step =4000
	50 us period sample_step = 2000
	
	
	step = 4000 / (period/25us)
	
	
	
	samples per tick = 
	*/
	
	Dac dac1;
	Dac dac2_led;
	dac1.init(DAC_CHANNEL_1);
	dac2_led.init(DAC_CHANNEL_2);
	dac2_led.low();
	//IRQ_objects::dac1_ptr = &dac1;
	sample_tick_local = IRQ_objects::sample_tick;
	while(1){
		while (IRQ_objects::sample_tick <= sample_tick_local);
		//if (IRQ_objects::sample_tick > sample_tick_local) {
		if (IRQ_objects::sample_tick - sample_tick_local > 1 ) {
			dac2_led.high();
			//while(1);
		}
		else {
	//		dac2_led.low();
		}
		sample_tick_local = IRQ_objects::sample_tick;
		float phase_rel = fmod ( (float) sample_tick_local , period_in_ticks) /  period_in_ticks;
		uint32_t dac_value = (uint32_t) ( (  square.get_value(phase_rel) * 0.5)+(sine.get_value(phase_rel)*0.5)); 
		
		//uint32_t sample_num_mod = (IRQ_objects::sample_tick % period_in_ticks);
		//percentage through = mod/ period in ticks;
		//sample num = percentage * num_samples
		

		dac1.set_value(dac_value);

		//}
	}
}
