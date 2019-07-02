extern "C" {
	#include "stm32f4xx_hal.h"
}
#include <math.h>

class Wave {
protected:
	const uint32_t num_samples {4000};
  //uint32_t* data_ptr {nullptr};
	virtual void fill_memory() =0;
	virtual uint32_t get_value_wptr(uint32_t* data_ptr, float phase_rel) const ;
	void set_ptr(uint32_t* &data_ptr);

public:
	Wave();
  virtual ~Wave() =0;
	[[noreturn]] Wave(Wave &wave);

  virtual uint32_t get_value(float phase_rel) const =0;
};

class Square : public Wave {
	void fill_memory();
	static uint32_t* data_ptr;
public:
	Square();	
	~Square();	
  uint32_t get_value(float phase_rel) const;
};

class Sine : public Wave{
	void fill_memory();
	static uint32_t* data_ptr;
public:
	Sine();	
	~Sine();
  uint32_t get_value(float phase_rel) const;
};


