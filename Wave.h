extern "C" {
	#include "stm32f4xx_hal.h"
}
#include <math.h>

class Wave {
protected:
	const uint32_t num_samples {4000};
  uint32_t* data_ptr {nullptr};
	virtual void fill_memory();
public:
	Wave();
  virtual ~Wave();
	[[noreturn]] Wave(Wave &wave);
  uint32_t get_value(float phase_rel) const;
  uint32_t get_value(uint32_t sample_num) const;
};

class Square : public Wave {
	void fill_memory();
public:
	Square();	
	~Square();	


};

class Sine : public Wave{
	void fill_memory();
public:
	Sine();	
	~Sine();	
};


