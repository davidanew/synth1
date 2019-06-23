#include "stm32f4xx_hal.h"

class Hal {
public:
	static void init(void);
	static void HAL_MspInit(void);
};
