#include "Wave.h"

S



void Square::fill_memory(){
	uint32_t i {0};
	data_ptr = new uint32_t[num_samples-1];
	for(i=0 ; i < (num_samples/2) ; i++){
		data_ptr[i] = 0xFFF;	
	}
	for(i=num_samples/2 ; i < num_samples ; i++){
		data_ptr[i] = 0x0;	
	}	
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


