#include "Wave.h"

void Wave::set_ptr(uint32_t* &data_ptr){
	try{		
		if (data_ptr == nullptr)
	    data_ptr = new uint32_t[num_samples];
	}
	catch(...){
		while(1);
	}	
}

Wave::Wave(){
}

Wave::~Wave(){
}

Wave::Wave(Wave &wave){//error on copy
	(void)wave;
	while(1);
}

uint32_t Wave::get_value_wptr(uint32_t* data_ptr, float phase_rel) const {

	uint32_t location = uint32_t (phase_rel * (float) (num_samples-1));
	if (location < num_samples){
		return data_ptr[location];
	}
	else //error
		while(1);
}

Square::Square(){
	Wave::set_ptr(data_ptr);
	fill_memory();
}

uint32_t*  Square::data_ptr = nullptr;

Square::~Square(){
}

void Square::fill_memory(){
	uint32_t i {0};
	for(i=0 ; i < (num_samples/2) ; i++){
		data_ptr[i] = 0xFFF;	
	}
	for(i=num_samples/2 ; i < num_samples ; i++){
		data_ptr[i] = 0x0;	
	}	
}

uint32_t Square::get_value(float phase_rel) const {
		return Wave::get_value_wptr(data_ptr,phase_rel);
}

Sine::Sine(){
	Wave::set_ptr(data_ptr);
	fill_memory();
}

uint32_t*  Sine::data_ptr = nullptr;

Sine::~Sine(){
}

void Sine::fill_memory(){
	uint32_t i {0};
	for(i=0 ; i < num_samples ; i++){
		float phase_radians = (float)i/(float)num_samples  * 6.28318530718;
		uint32_t value = (uint32_t) ((float)0xFFF*(0.5+0.5*sin(phase_radians)));
		data_ptr[i] = value;	
	}
}


uint32_t Sine::get_value(float phase_rel) const {
	return Wave::get_value_wptr(data_ptr,phase_rel);
}
