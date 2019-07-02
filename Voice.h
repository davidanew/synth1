//#include "Wave.h"
#include "Parameters.h"

extern "C" {
	#include "stm32f4xx_hal.h"
}

class Voice {
	float freq_1 {1000};
	float freq_2 {1000};
	float period_1_in_ticks {1000000};
	float period_2_in_ticks {1000000};	
	uint64_t sample_tick_start {0};
public:		
	Voice(const uint64_t sample_tick, const uint32_t sample_tick_us);
  uint32_t get_value(const uint64_t sample_tick);
};
