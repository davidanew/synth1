#include "Wave.h"


class Voice {
	Wave* wave_1 {nullptr};
	Wave* wave_2 {nullptr};
  float ampl_1 {0.5};
	float ampl_2 {0.5};
public:		
	Voice();
  uint32_t get_value(float phase_rel) ;
};




