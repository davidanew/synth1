#include "Hal.h"

void Hal::init(void){
	HAL_Init();
	//Run callback from member function
	//Normal callback will do nothing
	HAL_MspInit();	
}
void Hal::HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
}
