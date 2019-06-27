#include "main.h"

//#include<stdlib.h>

void SystemClock_Config(void);

int main () {
	
	
	uint64_t sample_tick_local = 0;
	const float freq = 10;
	const Sine sine;
	const Square square;
	Dac dac1;
	Dac dac2_led;
	
	Hal::init();
	Tim::init();
	float period = (float) 1.0 / freq; 
	float period_in_us = 1000000 * period ;
	float period_in_ticks = period_in_us / Tim::sample_tick_us() ;
	
	/*
	4000 samples for 40k
	
	4000 samples for 
	
	25 us period, sample step =4000
	50 us period sample_step = 2000
	
	
	step = 4000 / (period/25us)
	
	
	
	samples per tick = 
	*/
	

	dac1.init(DAC_CHANNEL_1);
	dac2_led.init(DAC_CHANNEL_2);
	dac2_led.low();
	//IRQ_objects::dac1_ptr = &dac1;
	sample_tick_local = IRQ_objects::sample_tick;
	

	
	
	
	
	
	while(1){
		while (IRQ_objects::sample_tick <= sample_tick_local);
		dac1.high();
		sample_tick_local = IRQ_objects::sample_tick;

		while (IRQ_objects::sample_tick <= sample_tick_local);
		dac1.low();		
		sample_tick_local = IRQ_objects::sample_tick;

		
	}
	
	uint32_t dac_value = 0;
	
	while(1){
		/*
		sample_tick_local = IRQ_objects::sample_tick;
		float phase_rel = fmod ( (float) sample_tick_local , period_in_ticks) /  period_in_ticks;
		dac_value = (uint32_t) ( (  square.get_value(phase_rel) * 0.5)+(sine.get_value(phase_rel)*0.5)); 		
		//7 instead 8 125 us/ 8 = 15 us
		*/
		
		//with tim 5 peaks
		//no tim 8 peans. 
		//3*125 = 375, /8 = 46us
		dac1.set_value(dac_value);			
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		
		
		
		//17 without
		// period 58us
		//8 with
		// period 125
		// diff = 67
		// 67/20 = 3.4 us
		
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		dac1.high();
		
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		
		
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();
		dac1.low();


	}

	
	
	while(1){
		while (IRQ_objects::sample_tick <= sample_tick_local);
		//if (IRQ_objects::sample_tick > sample_tick_local) {
		if (IRQ_objects::sample_tick - sample_tick_local > 1 ) {
			dac2_led.high();
			//while(1);
		}
		else {
	//		dac2_led.low();
		}
		sample_tick_local = IRQ_objects::sample_tick;
		float phase_rel = fmod ( (float) sample_tick_local , period_in_ticks) /  period_in_ticks;
		uint32_t dac_value = (uint32_t) ( (  square.get_value(phase_rel) * 0.5)+(sine.get_value(phase_rel)*0.5)); 
		
		//uint32_t sample_num_mod = (IRQ_objects::sample_tick % period_in_ticks);
		//percentage through = mod/ period in ticks;
		//sample num = percentage * num_samples
		

		dac1.set_value(dac_value);

		//}
	}
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
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
    Error_Handler();
  }
}


