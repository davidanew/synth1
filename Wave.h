extern "C" {
	#include "stm32f4xx_hal.h"
}

class Wave {
protected:
	uint32_t* data_ptr {nullptr};
	const uint32_t num_samples = 4096;
	virtual void fill_memory();
public:
	Wave();
  virtual ~Wave();
	[[noreturn]] Wave(Wave &wave);
  uint32_t get_value(float phase_rel);
};

class Square : public Wave {
	virtual void fill_memory();
public:
	Square();	
	[[noreturn]] Square(Square &square);
};

