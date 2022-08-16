/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SerialPort/SerialPort.h"
#include "Control.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//==============================================================================
#define MAIN_DATA_BUFFER_SIZE 1024
static uint8_t main_data_buffer_memory[MAIN_DATA_BUFFER_SIZE];

xDataBufferT MainDataBuffer =
{
	.Data = main_data_buffer_memory,
	.Size = MAIN_DATA_BUFFER_SIZE
};
//------------------------------------------------------------------------------
uint8_t time_5ms;
uint8_t time_10ms;
uint16_t time_1000ms;
uint8_t time_tcp_update;
uint32_t time_ms;
uint32_t LedUpdateCount;
uint32_t LedUpdatePerSecond;
//------------------------------------------------------------------------------
volatile STM32_TIM_REG_T* Timer4 = (STM32_TIM_REG_T*)TIM4;
volatile STM32_TIM_REG_T* Timer2 = (STM32_TIM_REG_T*)TIM2;
//==============================================================================
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//==============================================================================
typedef struct
{
	uint8_t Green;
	uint8_t Red;
	uint8_t Blue;
	
} PixelT;
//------------------------------------------------------------------------------
union
{
	struct
	{
		uint32_t PixelsThreadBusy : 1;
	};
	uint32_t Value;
	
} Status;
//------------------------------------------------------------------------------
#define PIXELS_DATA_OFFSET 50
uint8_t pixels_data[8 * 3 * 8 + PIXELS_DATA_OFFSET + 1];
uint8_t pixels_data_count;
//==============================================================================
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//==============================================================================
uint16_t PutColorToArray(uint8_t color, uint8_t* data)
{
	uint16_t offset = 0;
	
	for (uint8_t bit_number = 0; bit_number < sizeof(color) * 8; bit_number++)
	{
		if (color & 0x80)
		{
			data[offset] = (uint8_t)((float)(Timer2->Period + 1) * 0.85 / 1.25);
		}
		else
		{
			data[offset] = (uint8_t)((float)(Timer2->Period + 1) * 0.4 / 1.25);
		}
		
		color <<= 1;
		offset++;
	}
	
	return offset;
}
//------------------------------------------------------------------------------
void PutPixelsToArray(PixelT* pixels, int pixels_count, uint8_t* data)
{
	int i = 0;
	int j = 0;
	pixels_data_count = pixels_count * 3 * 8 + PIXELS_DATA_OFFSET;
	data = data + PIXELS_DATA_OFFSET;
	
	for (int i = 0; i < pixels_count; i++)
	{
		data += PutColorToArray(pixels[i].Green, data);
		data += PutColorToArray(pixels[i].Red, data);
		data += PutColorToArray(pixels[i].Blue, data);
	}
	
	*data = 0;
	pixels_data_count += 1;
}
//------------------------------------------------------------------------------
PixelT Pixels[] =
{
	{
		.Green = 0x01,
		.Blue = 0x01,
		.Red = 0x01
	},
	
	{
		.Green = 0x01,
		.Blue = 0x01,
		.Red = 0x01
	},
	
	{
		.Green = 0x01,
		.Blue = 0x01,
		.Red = 0x01
	},
	
	{
		.Green = 0x01,
		.Blue = 0x01,
		.Red = 0x01
	},
	
	{
		.Green = 0x01,
		.Blue = 0x01,
		.Red = 0x01
	},
	
	{
		.Green = 0x01,
		.Blue = 0x01,
		.Red = 0x01
	},
	
	{
		.Green = 0x01,
		.Blue = 0x01,
		.Red = 0x01
	},
	
	{
		.Green = 0x00,
		.Blue = 0x00,
		.Red = 0x00
	},
};
//------------------------------------------------------------------------------
static volatile DMA_Channel_TypeDef* DMA_TX = DMA1_Channel2;
//------------------------------------------------------------------------------
void Start()
{
	DMA_TX->CCR &= ~DMA_CCR_EN;
	
	DMA_TX->CCR |= DMA_CCR_PL_0;
	DMA_TX->CCR &= ~DMA_CCR_PSIZE;
	DMA_TX->CCR &= ~DMA_CCR_MSIZE;
	DMA_TX->CCR |= DMA_CCR_PSIZE_1;
	
	DMA_TX->CNDTR = pixels_data_count;
	DMA_TX->CPAR = (uint32_t)&Timer2->CaptureCompare3Value;
	DMA_TX->CMAR = (uint32_t)(pixels_data);
	
	Status.PixelsThreadBusy = true;
	DMA_TX->CCR |= DMA_CCR_TCIE;
	DMA_TX->CCR |= DMA_CCR_EN;
	
	WS2812_SYNC_GPIO_Port->ODR |= WS2812_SYNC_Pin;
}
//------------------------------------------------------------------------------
void PixelsTransferComplite()
{
	DMA_TX->CCR &= ~DMA_CCR_EN;
	WS2812_SYNC_GPIO_Port->ODR &= ~WS2812_SYNC_Pin;
	Status.PixelsThreadBusy = false;
}
//==============================================================================
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	//----------------------------------------------------------------------------
	Timer4->DMAOrInterrupts.UpdateInterruptEnable = true;
	Timer4->Control1.CounterEnable = true;
	
	TIM_CCxChannelCmd(TIM2, TIM_CHANNEL_1, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(TIM2, TIM_CHANNEL_2, TIM_CCx_ENABLE);
	
	Timer2->CaptureCompare.Compare2OutputEnable = true;
	Timer2->CaptureCompare.Compare3OutputEnable = true;
	
	Timer2->DMAOrInterrupts.DMA_RequestEnable = true;
	Timer2->Control1.CounterEnable = true;
	
	ControlInit(main);
	//----------------------------------------------------------------------------
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//--------------------------------------------------------------------------
		if (!time_5ms)
		{
			time_5ms = 4;
			
			SerialPortHandler();
		}
		//--------------------------------------------------------------------------
		if (!DMA_TX->CNDTR)
		{
			LedUpdateCount++;
			PutPixelsToArray(Pixels, sizeof(Pixels) / sizeof(PixelT), pixels_data);
			Start();
		}
		//--------------------------------------------------------------------------
		if (!time_1000ms)
		{
			time_1000ms = 999;
			
			//PutPixelsToArray(Pixels, sizeof(Pixels) / sizeof(PixelT), pixels_data);
			//Start();
			
			LedUpdatePerSecond = LedUpdateCount;
			LedUpdateCount = 0;
			//xTxTransmitData(SerialPort.Tx, &Pixel, sizeof(Pixel));
			//xTxTransmitData(SerialPort.Tx, &Pixels, sizeof(Pixels));
		}
		//--------------------------------------------------------------------------
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
