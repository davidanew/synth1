#include "Voice.h"

Voice::Voice(){
	wave_1 = new Sine();
	wave_2 = new Square();
}
uint32_t Voice::get_value(float phase_rel) {
	return (uint32_t) (wave_1->get_value(phase_rel) * ampl_1 + wave_2->get_value(phase_rel) * ampl_2);
}	




