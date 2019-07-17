/*
Two sections, intialisation and run loop
run loop never exits and execution of each pass is delayed until the next sample tick
The sample tick is updated by TIM2 IRQ
The sample is output at the end of the run loop
This could cause some jitter should be too small to
be audible
*/

#include "main.h"

//set this test flag to use alternative main() for testing
#define test

void SystemClock_Config(void);

#ifdef test

//Currently testing MIDI input and outputting to st-link virtual com port

int main () {
  //testing code
	Hal::init();
	//MIDI input
	Usart_1 usart_1;
	//Virtual com port
	Usart_2_vcom usart_2;
	//Buffer used for recieve and transmit
	uint8_t buffer_arr[1] = {'X'};
	//Output midi to com port
	while (1){
		usart_1.receive(buffer_arr, sizeof(buffer_arr));
		usart_2.transmit(buffer_arr, sizeof(buffer_arr));
	}
}

#else

int main () {
	//Hal init always needs to be run
	Hal::init();
	//TIM2 used to to update sample clock
	Tim::init();
	//Dac 1 used for audio output
	Dac dac1(DAC_CHANNEL_1);
	//currently using dac channel 2 to measure run loop execution time
	Dac dac2_led(DAC_CHANNEL_2);
	//One voice for each key press
	const uint32_t num_voices {16};
	//Stores last processed sample
	uint64_t sample_tick_local = 0;
	//All currrent voices, uses dynamic binding
	Voice* voice_array[num_voices] = {nullptr};
	//Holds parameters that are common to all voices
	Parameters parameters;
	//TODO: Filter will need to be updated on dial change
	//what to stat-up with, do we need a last state save?
	Filter filter {1000,(float)0.0};
	//Move clock to full frequency
	SystemClock_Config();
	
	try{
		//TODO: These will also need to be set with dial
		//make sure deconstructed properly when changed
		parameters.wave_1 = new Sine();
		parameters.wave_2 = new Sine();		
		voice_array[0] = new Voice(40000,parameters, 1000, 1.0);
	}
	catch(...){
		while(1);
	}	
	//currently using dac channel 2 to measure run loop execution time
	dac2_led.low();
	//Set local tick value so there is not a large delta when the first
	//sample is processed
	sample_tick_local = IRQ_objects::sample_tick;
	/*main run loop, never exits*/
	while(1){
		//wait for next sample tick
		while (IRQ_objects::sample_tick <= sample_tick_local);
		//Indicates processing started
		dac2_led.high();
		//Reset variable for next sample
		sample_tick_local = IRQ_objects::sample_tick;
		uint32_t i {0};
		float total {0};
		//Loop though all voices and get the sample for the valid voices
		//Add to total output value
		for (i=0 ; i<num_voices ; i++){
			if (voice_array[i] != nullptr) {
				total += (float) 1 * voice_array[i]->get_value(sample_tick_local);
			}
		}
		//const float dac_value_float = voice_array[0]->get_value(sample_tick_local);
		//_rel means value from 0 to 1
		const float total_rel = total * (float) 0.5 + (float) 0.5; 	
		const float filtered_rel = filter.next_sample(total_rel);
		//dac2_led.set_value_rel(filtered_rel);
		//Output the computed sample to DAC
		dac1.set_value_rel(filtered_rel);
		dac2_led.low();
	}
}

#endif

//Mainly autogenerated code
//180MHz clock
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
	//Tuned for my microcontroller
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


