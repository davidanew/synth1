#include "main.h"
//move this to filter class header when done
#include <math.h>

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

class Filter {
	// change filter to 2 pole low pass
	
	//from http://www.eecs.umich.edu/courses/eecs206/archive/spring02/notes.dir/iir4.pdf
	/*
	x0 -- |b0> --+----------- y0
               |       |
               |       z^-1
               |       |
	             +-<a1|--|  y1
               |       |
               |       z^-1
               |       |
	             +-<a2|--|  y2
	*/
	
	//r real pole position on z plane
	//e.g. 0.85460 gives 1kHz cutoff
	//r =exp(-2*pi*(1000/40000))
	//H(Z) = b0/(1-a1*z^-1-a2*z^-2)
  //H(Z) = 1-2*r*cos(theta)+r^2 /(1 - 2*r*cos(theta)z^-1 + r^2z^-2)
	//Theta controls imaginary component of poles
	//theta = 0 gives real poles with standard low pass filter
	//Theta will be mapped to a knob as is too complex to calculate r and theta to get exacly the right response
	//see web site above for more details
	
	float x0 {0};
	float y0 {0};
	float y1 {0};
	float y2 {0};
	float a1 {0};
	float a2 {0};
	float b0 {1};
		
	//sample freq hard coded for now
	const float sample_freq {40000};
	const float pi {(float) 3.14159265359};
	
public:	
	Filter(float cutoff, float theta){
		//const float term1 = (float)-2.0*pi*((float)cutoff/(float)sample_freq);
		//const float r = exp((float)-2.0*pi*((float)cutoff/(float)sample_freq));
	  //volatile float r = pow((float)2.5,term1);
		const float r = exp((float)-2.0*pi*((float)cutoff/(float)sample_freq));
		b0 = (float)1.0-(float)2.0*r*(float)cos(theta)+(float)pow(r,(float)2.0);
		a1 = (float)2.0*r*cos(theta);
		a2 = - pow(r,(float)2.0);
	}
	
	Filter(){
		//default constructor uses default values which do no filtering
	}

	float next_sample(float next_x){
		x0 = next_x;
		y2 = y1;
		y1 = y0;
		y0 = b0*x0 + a1*y1 + a2*y2;
		return y0;	
	}
};

int main () {
	const uint32_t num_voices {16};
	uint64_t sample_tick_local = 0;
	Voice* voice_array[num_voices] = {nullptr};
	Dac dac1;
	Dac dac2_led;
	Parameters parameters;
	//Filter filter {500,(float)0.1571};
	Filter filter {1000,(float)0.0};

	Hal::init();
	SystemClock_Config();
	Tim::init();
	dac1.init(DAC_CHANNEL_1);
	dac2_led.init(DAC_CHANNEL_2);
	
	try{
			
		parameters.wave_1 = new Sine();
		parameters.wave_2 = new Sine();	
		
		/*
		float scale = (float) 0.2;
		
		voice_array[0] = new Voice(40000,parameters, 250, scale);
		voice_array[1] = new Voice(40000,parameters, 500, scale);
		voice_array[2] = new Voice(40000,parameters, 1000, scale);
		voice_array[3] = new Voice(40000,parameters, 2000, scale);
		voice_array[4] = new Voice(40000,parameters, 4000, scale);
		voice_array[5] = new Voice(40000,parameters, 8000, scale);
		voice_array[6] = new Voice(40000,parameters, 16000, scale);
		*/
		
		voice_array[0] = new Voice(40000,parameters, 1000, 1.0);

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
		const float total_rel = total * (float) 0.5 + (float) 0.5; 	
		const float filtered_rel = filter.next_sample(total_rel);
		//dac2_led.set_value_rel(filtered_rel);
		dac1.set_value_rel(filtered_rel);
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




