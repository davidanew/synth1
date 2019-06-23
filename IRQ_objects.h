#include "Dac.h"

struct IRQ_objects {
	static Dac* dac1_ptr;
	static uint64_t sample_tick;
};
