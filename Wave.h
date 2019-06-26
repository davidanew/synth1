extern "C" {
	#include "stm32f4xx_hal.h"
}
#include <math.h>

class Wave {
protected:
	uint32_t num_samples {4096};
	//virtual void fill_memory() =0;
  uint32_t* data_ptr {nullptr};

public:
	Wave();
  ~Wave();
//	[[noreturn]] Wave(Wave &wave);
  uint32_t get_value(float phase_rel);
};

class Square : public Wave {
//	uint32_t num_samples;
	void fill_memory();
//	uint32_t* data_ptr {nullptr};
public:
	Square();	
//  uint32_t get_value(float phase_rel);
	[[noreturn]] Square(Square &square);
};

class Sine : public Wave{
//	uint32_t num_samples;
	void fill_memory();
//	uint32_t* data_ptr {nullptr};
public:
	Sine();	
//  uint32_t get_value(float phase_rel);
	[[noreturn]] Sine(Sine &sine);
};


