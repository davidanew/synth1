#include "Parameters.h"
/*
Wave*  Parameters::wave_1 {nullptr};
Wave*  Parameters::wave_2 {nullptr};
float Parameters::ampl_1 {0.01};
float Parameters::ampl_2 {0.5};
uint64_t Parameters::period_in_ticks {1000};
*/


Parameters::Parameters()
	:wave_1(nullptr),
	wave_2(nullptr),
	ampl_1(0.5),
	ampl_2(0.5),
  period_in_ticks(1000){	
}


Parameters::Parameters(const Parameters &source)
	:wave_1(source.wave_1),
	wave_2(source.wave_2),
	ampl_1(source.ampl_1),
	ampl_2(source.ampl_2),
  period_in_ticks(source.period_in_ticks){	
}
	

