/* allow communication between main thread and IRQ handlers */

#include "Dac.h"

//This needs to be global to allow communication between main thread and IRQ handlers
struct IRQ_objects {
	volatile static uint64_t sample_tick;
};
