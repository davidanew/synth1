#include "Voice.h"

Voice::Voice(const float sample_tick_freq, const Parameters &parameters_arg, uint32_t freq, const float &velocity_arg)
	: parameters(parameters_arg)
  , freq_1(freq)
  , freq_2(freq)
	, velocity(velocity_arg){
	
	/*	
	const float period_1 = (float) 1.0 / freq_1;
	const float period_2 = (float) 1.0 / freq_2;
	
	const float period_1_in_us = 1000000 * period_1 ;
	const float period_2_in_us = 1000000 * period_2 ;
	
	period_1_in_ticks = period_1_in_us / sample_tick_us ;
	period_2_in_ticks = period_2_in_us / sample_tick_us ;
		
	//need phase change per tick
	
	tick freq
	wave freq
*/	
		
	//phase per tick = wave frq/tick feq
	
	phase_rel_per_tick_1 = freq_1/sample_tick_freq;
	phase_rel_per_tick_2 = freq_2/sample_tick_freq;
	
		

}
	
Voice::Voice(Voice &source){
	//error on copy
	(void)source;
	while(1);//error on copy

}

//rename to get_sample
float Voice::get_value(const uint64_t sample_tick) {
	//coded to prevent costly int to float conversion and fmod
	//allows recovery from missed sample ticks
	if (previous_sample_tick == 0) {
		//there is not a long delay due to the first sample
		previous_sample_tick = sample_tick;
	}
	while (sample_tick > previous_sample_tick){
		phase_rel_1 += phase_rel_per_tick_1;
		if (phase_rel_1 > (float) 1.0) 
			phase_rel_1 -= 1;
		phase_rel_2 += phase_rel_per_tick_2;
		if (phase_rel_2 > (float) 1.0) 
			phase_rel_2 -= 1;
		previous_sample_tick += 1;
	}
	
	
	//const phase_1 = previous_phase_1 + 
	
	/*
	const uint64_t tick_delta = sample_tick - sample_tick_start;
	const float tick_delta_float = (float) tick_delta;
	const float phase_rel_1 = fmod ( tick_delta_float , period_1_in_ticks) /  period_1_in_ticks;
	const float phase_rel_2 = fmod ( tick_delta_float , period_2_in_ticks) /  period_2_in_ticks;
	*/
	return (float) (parameters.wave_1->get_value(phase_rel_1) * parameters.ampl_1 + 
									parameters.wave_2->get_value(phase_rel_2) * parameters.ampl_2)
	                * velocity;
}	
