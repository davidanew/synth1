#include "Wave.h"
//#include "stdlib.h"

Wave::Wave(){
	try{		
	  data_ptr = new uint32_t[num_samples];
	}
	catch(...){
		while(1);
	}

}

Wave::~Wave(){
	delete data_ptr;
}


Wave::Wave(Wave &wave){//error on copy
	(void)wave;
	while(1);
}


uint32_t Wave::get_value(float phase_rel) const {
	uint32_t location = uint32_t (phase_rel * (float) (num_samples-1));
	if (location < num_samples){
		return data_ptr[location];
	}
	else //error
		while(1);
}

uint32_t Wave::get_value(uint32_t sample_num) const{
	if (sample_num < num_samples){
		return data_ptr[sample_num];
	}
	else //error
		while(1);
}



Square::Square(){

	fill_memory();
}

/*

Square::Square(Square &square){//error on copy
	(void)square;
	while(1);
}
*/

void Square::fill_memory(){
	uint32_t i {0};

	for(i=0 ; i < (num_samples/2) ; i++){
		data_ptr[i] = 0xFFF;	
	}
	for(i=num_samples/2 ; i < num_samples ; i++){
		data_ptr[i] = 0x0;	
	}	
}



Sine::Sine(){

	fill_memory();
}

/*
Sine::Sine(Sine &sine){//error on copy
	(void)sine;
	while(1);
}
*/

void Sine::fill_memory(){
	uint32_t i {0};
	for(i=0 ; i < num_samples ; i++){
		double phase_radians = (double)i/(double)num_samples  * 6.283;
		uint32_t value = (uint32_t) ((double)0xFFF*(0.5+0.5*sin(phase_radians)));
		data_ptr[i] = value;	
	}
}


