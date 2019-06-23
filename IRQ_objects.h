#include "Dac.h"

struct IRQ_objects {
	static Dac* dac1_ptr;
	volatile static uint64_t sample_tick;
};
