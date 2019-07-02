#include "main.h"

//#define test

void SystemClock_Config(void);

#ifdef test
int main () {
	uint64_t sample_tick_local = 0;
	Hal::init();
	SystemClock_Config();
	Tim::init();
	Dac dac1;
	dac1.init(DAC_CHANNEL_1);
	while(1){
		//1 dac is 48 cycles
		//31 instructions on debugger
		dac1.low();
		dac1.high();
	}
}

#else

int main () {
	uint64_t sample_tick_local = 0;
	const float freq = 1000;
	
  //each instace needs adsr/ start time?

	Voice* voice_array[16];
	
	try{		
		voice_array[0] = new Voice();
	}
	catch(...){
		while(1);
	}	
		
	Dac dac1;
	Dac dac2_led;
	
	Hal::init();
	SystemClock_Config();
	Tim::init();
	float period = (float) 1.0 / freq; 
	float period_in_us = 1000000 * period ;
	float period_in_ticks = period_in_us / Tim::sample_tick_us() ;
	
	dac1.init(DAC_CHANNEL_1);
	dac2_led.init(DAC_CHANNEL_2);
	dac2_led.low();
	sample_tick_local = IRQ_objects::sample_tick;
	while(1){
		while (IRQ_objects::sample_tick <= sample_tick_local);
		dac2_led.high();
		sample_tick_local = IRQ_objects::sample_tick;
		float phase_rel = fmod ( (float) sample_tick_local , period_in_ticks) /  period_in_ticks;
		uint32_t dac_value = voice_array[0]->get_value(phase_rel);
		dac1.set_value(dac_value);
		dac2_led.low();
	}
}

#endif

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct {};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 374;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1);
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    while(1);
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    while(1);
  }
}




