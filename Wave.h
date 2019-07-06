extern "C" {
	#include "stm32f4xx_hal.h"
}
#include <math.h>
#include <stdlib.h>


class Wave {
protected:
	const uint32_t num_samples {4000};
	virtual void fill_memory() =0;
	virtual float get_value_wptr(float* data_ptr, float phase_rel) const ;
	void set_ptr(float* &data_ptr);

public:
	Wave();
  virtual ~Wave() =0;
	[[noreturn]] Wave(Wave &wave);

  virtual float get_value(float phase_rel) const =0;
};

class Square : public Wave {
	void fill_memory();
	static float* data_ptr;
public:
	Square();	
	~Square();	
  float get_value(float phase_rel) const;
};

class Sine : public Wave{
	void fill_memory();
	static float* data_ptr;
public:
	Sine();	
	~Sine();
  float get_value(float phase_rel) const;
};

class Noise : public Wave{
	void fill_memory();
	static float* data_ptr;
public:
	Noise();	
	~Noise();
  float get_value(float phase_rel) const;
};



