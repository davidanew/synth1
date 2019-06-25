#include "Wave.h"

Wave::Wave(){
	data_ptr = new uint32_t[num_samples-1];
}

Wave::~Wave(){
	delete data_ptr;
}

Wave::Wave(Wave &wave){//error on copy
	(void)wave;
	while(1);
}

void Wave::fill_memory(){
}

uint32_t Wave::get_value(float phase_rel){
	uint32_t location = uint32_t (phase_rel * (float) (num_samples-1));
	if (location < num_samples){
		return data_ptr[location];
	}
	else //error
		while(1);
}

Square::Square(){
	data_ptr = new uint32_t[num_samples-1];
	fill_memory();
}


Square::Square(Square &square){//error on copy
	(void)square;
	while(1);
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


