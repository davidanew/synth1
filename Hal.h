/*general Hal setup*/

#include "stm32f4xx_hal.h"

//TODO: Error on copy constructor

class Hal {
public:
	static void init(void);
	static void HAL_MspInit(void);
};
