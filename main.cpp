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




UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);

int main () {
  //testing code
	Hal::init();
	MX_GPIO_Init();
  MX_USART1_UART_Init();
	MX_USART2_UART_Init();

	uint8_t buffer[1] = {0x66};
	//uint8_t buffer[1] = {'a'};

	while (1)
   //HAL_UART_Receive(&huart1, buffer, sizeof(buffer), HAL_MAX_DELAY);
	
	 HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), HAL_MAX_DELAY );

}


/*

static UART_HandleTypeDef s_UARTHandle = UART_HandleTypeDef();
 
void ms_delay (int ms); 
 
int main(void)
{
    HAL_Init();
 
    __USART1_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.Pin = GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Alternate = GPIO_AF7_USART1;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    s_UARTHandle.Instance        = USART1;
    s_UARTHandle.Init.BaudRate   = 31250;
    s_UARTHandle.Init.WordLength = UART_WORDLENGTH_8B;
    s_UARTHandle.Init.StopBits   = UART_STOPBITS_1;
    s_UARTHandle.Init.Parity     = UART_PARITY_NONE;
    s_UARTHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    s_UARTHandle.Init.Mode       = UART_MODE_TX_RX;
    
    if (HAL_UART_Init(&s_UARTHandle) != HAL_OK)
        asm("bkpt 255");
    
    for (;;)
    {
        uint8_t buffer[4] = {127,127,127,127};
        //HAL_UART_Receive(&s_UARTHandle, buffer, sizeof(buffer), HAL_MAX_DELAY);
        HAL_UART_Transmit(&s_UARTHandle, buffer, sizeof(buffer), HAL_MAX_DELAY);
				    	ms_delay(800);				
    }
}






void ms_delay (int ms)
{
  while (ms-- > 0)
    {
      volatile int x = 500;
      while (x-- > 0)
	__asm ("nop");
    }
}

*/

/*

int main (void)
{
    RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOARST;    // Reset GPIOA 
    RCC->AHB1RSTR = 0;                         // Exit reset state
    
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;       // Enable GPIOA clock
    GPIOA->MODER |= GPIO_MODER_MODER5_0;	   // Enable Output on A5 (LED2 on Nucleo F401RE board)
	  GPIOA->MODER |= GPIO_MODER_MODER9_0;	   // Enable Output on PA9

    
    while (1)
    {
    	GPIOA->ODR ^= (1 << (5));	// toggle LED pin
			GPIOA->ODR ^= (1 << (9));	// toggle LED pin

    	ms_delay(800);

    }

  return 0;
}
*/

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



static void MX_USART1_UART_Init(void)
{


  huart1.Instance = USART1;
  huart1.Init.BaudRate = 31250;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    while(1);
  }


}


static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
    while(1);
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}


static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
}


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {

    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
		
		/*

    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		*/
		

 
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }
	
  if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
		
		
		    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		
  }

}



