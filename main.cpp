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
	const uint32_t num_voices {16};
	uint64_t sample_tick_local = 0;
	Voice* voice_array[num_voices] = {nullptr};
	Dac dac1;
	Dac dac2_led;
	Parameters parameters;

	Hal::init();
	SystemClock_Config();
	Tim::init();
	dac1.init(DAC_CHANNEL_1);
	dac2_led.init(DAC_CHANNEL_2);
	
	try{
		parameters.wave_1 = new Sine();
		parameters.wave_2 = new Sine();	
			
		voice_array[0] = new Voice(40000,parameters, 100, 1);
		voice_array[1] = new Voice(40000,parameters, 300, (float) 1.0/ (float) 3.0);
		voice_array[2] = new Voice(40000,parameters, 500, (float) 1.0/ (float) 5.0);
		voice_array[3] = new Voice(40000,parameters, 700, (float) 1.0/ (float) 7.0);
		voice_array[4] = new Voice(40000,parameters, 900, (float) 1.0/ (float) 9.0);
		voice_array[5] = new Voice(40000,parameters, 1100, (float) 1.0/ (float) 11.0);
		voice_array[6] = new Voice(40000,parameters, 1300, (float) 1.0/ (float) 13.0);
		voice_array[7] = new Voice(40000,parameters, 1500, (float) 1.0/ (float) 15.0);
		voice_array[8] = new Voice(40000,parameters, 1700, (float) 1.0/ (float) 17.0);
		voice_array[9] = new Voice(40000,parameters, 1900, (float) 1.0/ (float) 19.0);
		voice_array[10] = new Voice(40000,parameters, 2100, (float) 1.0/ (float) 21.0);
		voice_array[11] = new Voice(40000,parameters, 2300, (float) 1.0/ (float) 23.0);
		voice_array[12] = new Voice(40000,parameters, 2500, (float) 1.0/ (float) 25.0);
		voice_array[13] = new Voice(40000,parameters, 2700, (float) 1.0/ (float) 27.0);
		voice_array[14] = new Voice(40000,parameters, 2900, (float) 1.0/ (float) 29.0);
		voice_array[15] = new Voice(40000,parameters, 3100, (float) 1.0/ (float) 31.0);
	
	}
	catch(...){
		while(1);
	}	
	dac2_led.low();
	sample_tick_local = IRQ_objects::sample_tick;
	while(1){
		while (IRQ_objects::sample_tick <= sample_tick_local);
		dac2_led.high();
		sample_tick_local = IRQ_objects::sample_tick;
		uint32_t i {0};
		float total {0};
		for (i=0 ; i<num_voices ; i++){
			if (voice_array[i] != nullptr) {
				total += (float) 1 * voice_array[i]->get_value(sample_tick_local);
			}
		}
		//const float dac_value_float = voice_array[0]->get_value(sample_tick_local);
		const float dac_value_rel = total * (float) 0.5 + (float) 0.5; 	
		dac1.set_value_rel(dac_value_rel);
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




