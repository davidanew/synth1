#include "Dac.h"

void Dac::init(uint32_t channel_arg) {
	channel = channel_arg;
	DAC_ChannelConfTypeDef sConfig {};
	hdac.Instance = DAC;
	hdac.State = HAL_DAC_STATE_RESET;					
	HAL_DAC_MspInit(&hdac);				
	if (HAL_DAC_Init(&hdac) != HAL_OK)
	{
		while(1);
	}
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	if (HAL_DAC_ConfigChannel(&hdac, &sConfig, channel) != HAL_OK)
	{
		while(1);
	}
	if (HAL_DAC_Start(&hdac , channel) != HAL_OK)
	{
		while(1);
	}
	init_done = true;
}	

void Dac::set_value_rel(double value_rel){
	uint32_t dac_value = (uint32_t) ((double) 0xFFF * value_rel );
	if (HAL_DAC_SetValue(&hdac, channel, DAC_ALIGN_12B_R, dac_value) != HAL_OK || init_done == false){
		while(1);
	}
}

void Dac::set_value(uint32_t value){
//	uint32_t dac_value = value;
	if (HAL_DAC_SetValue(&hdac, channel, DAC_ALIGN_12B_R, value) != HAL_OK || init_done == false){
		while(1);
	}
}

void Dac::high(){
	if (HAL_DAC_SetValue(&hdac, channel, DAC_ALIGN_12B_R, 0xFFF) != HAL_OK || init_done == false){
		while(1);
	}
}

void Dac::low(){
	if (HAL_DAC_SetValue(&hdac, channel, DAC_ALIGN_12B_R, 0x000) != HAL_OK || init_done == false){
		while(1);
	}
}

uint32_t Dac::get_value(){
	return HAL_DAC_GetValue ( &hdac , channel);
}

void Dac::HAL_DAC_MspInit(DAC_HandleTypeDef* hdac_arg)
{
	GPIO_InitTypeDef GPIO_InitStruct {}; //= {0};
	if(hdac_arg->Instance==DAC)
	{
		__HAL_RCC_DAC_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		if (this->channel == DAC_CHANNEL_1)
		  GPIO_InitStruct.Pin = GPIO_PIN_4;
		else if (this->channel == DAC_CHANNEL_2)
			GPIO_InitStruct.Pin = GPIO_PIN_5;
		else //error
			while(1);
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		}
}
