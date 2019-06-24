extern "C" {
	#include "stm32f4xx_hal.h"
}

class Wave {
protected:
	uint32_t* data_ptr {nullptr};
	uint32_t num_samples = 4096;
public:
	Wave();
	virtual void fill_memory();
//make 4k array
//fill with samples
  uint32_t get_value(float phase);
  virtual ~Wave();
};

class Square : public Wave {
	Square();	
	virtual void fill_memory();
};

