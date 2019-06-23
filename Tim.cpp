#include "Tim.h"

void Tim::init(void)
{
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 16;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 25;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_MspInit (&htim2);
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		while(1);
	}
	if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
	{
		while(1);
	}
}
void Tim::HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();		
		HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}

TIM_HandleTypeDef Tim::htim2 = {0};
