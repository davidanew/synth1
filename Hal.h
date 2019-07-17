/*general Hal setup*/

#include "stm32f4xx_hal.h"

//TODO: Mspinit can be private - also look at other classes

class Hal {
public:
	[[noreturn]] Hal(Hal &source);
	static void init(void);
	static void HAL_MspInit(void);
};
