#include "main.h"

//why is this not working in main?
//int sample_tick_local;

int main () {

	
	uint64_t sample_tick_local = 0;
	sample_tick_local ++;
	Hal::init();
	Tim::init();
	Dac dac1;
	IRQ_objects::dac1_ptr = &dac1;

	dac1.init(DAC_CHANNEL_1);
	while(1){
		if (IRQ_objects::sample_tick > sample_tick_local) {
			sample_tick_local = IRQ_objects::sample_tick;
			if (sample_tick_local % 10  >= 5) {
				dac1.high();
			}
			else {
				dac1.low();
			}
		}
	}
}
