#include "IRQ_objects.h"

//Dac* IRQ_objects::dac1_ptr = nullptr;
Dac* IRQ_objects::dac1_ptr ;
volatile uint64_t IRQ_objects::sample_tick = 0;
