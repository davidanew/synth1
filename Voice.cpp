#include "Voice.h"

Voice::Voice(const uint64_t sample_tick, const uint64_t sample_tick_us, const Parameters &parameters_arg, uint32_t freq, const float &velocity_arg)
	: parameters(parameters_arg)
  , freq_1(freq)
  , freq_2(freq)
	, velocity(velocity_arg)
	, sample_tick_start(sample_tick){
		
	const float period_1 = (float) 1.0 / freq_1;
	const float period_2 = (float) 1.0 / freq_2;
	
	const float period_1_in_us = 1000000 * period_1 ;
	const float period_2_in_us = 1000000 * period_2 ;
	
	period_1_in_ticks = period_1_in_us / sample_tick_us ;
	period_2_in_ticks = period_2_in_us / sample_tick_us ;

}
	
Voice::Voice(Voice &source){
	//error on copy
	(void)source;
	while(1);//error on copy

}

float Voice::get_value(const uint64_t sample_tick) {
	const uint64_t tick_delta = sample_tick - sample_tick_start;
	const float phase_rel_1 = fmod ( (float) tick_delta , period_1_in_ticks) /  period_1_in_ticks;
	const float phase_rel_2 = fmod ( (float) tick_delta , period_2_in_ticks) /  period_2_in_ticks;
	return (float) (parameters.wave_1->get_value(phase_rel_1) * parameters.ampl_1 + 
									parameters.wave_2->get_value(phase_rel_2) * parameters.ampl_2)
	                * velocity;
}	
