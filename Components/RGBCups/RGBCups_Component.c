//==============================================================================
#include "RGBCups_Components.h"
//==============================================================================
uint8_t data_buffer_cups_12[RGB_CUPS_SYNC_START_DATA_COUNT
														+ RGB_CUPS_DATA_BUFFER_SIZE_CUP1
														+ RGB_CUPS_DATA_BUFFER_SIZE_CUP2
														+ RGB_CUPS_SYNC_END_DATA_COUNT];
//------------------------------------------------------------------------------
uint8_t data_buffer_cups_34[RGB_CUPS_SYNC_START_DATA_COUNT
														+ RGB_CUPS_DATA_BUFFER_SIZE_CUP3
														+ RGB_CUPS_DATA_BUFFER_SIZE_CUP4
														+ RGB_CUPS_SYNC_END_DATA_COUNT];
//==============================================================================
static RGBCupAdapterT AdapterCup12 =
{
	.DMA = DMA1_Channel2,
	.DrawMemory = data_buffer_cups_12,
	.DrawMemorySize = sizeof(data_buffer_cups_12),
	.PWM_Channel = 2,
	.Timer = (STM32_TIM_REG_T*)TIM2
};
//------------------------------------------------------------------------------
static RGBCupAdapterT AdapterCup34 =
{
	.DMA = DMA1_Channel2,
	.DrawMemory = data_buffer_cups_34,
	.DrawMemorySize = sizeof(data_buffer_cups_34),
	.PWM_Channel = 1,
	.Timer = (STM32_TIM_REG_T*)TIM2,
};
//==============================================================================
static void EventListener(void* cup, RGBCupEventSelector event, uint32_t args, uint32_t count)
{
	switch ((uint8_t)event)
	{
		
		default : return;
	}
}
//==============================================================================
const static RGBCupInterfaceT Interface =
{
	.EventListener = (RGBCupsEventListener)EventListener
};
//==============================================================================
int RGBCups_ComponentInit(void* parent)
{
	RGBCups_Init(parent, (RGBCupInterfaceT*)&Interface);
	
	uint16_t buffer_offset = RGB_CUPS_SYNC_START_DATA_COUNT;
	RGBCups[RGBCupNumber1].DataBuffer = data_buffer_cups_12 + buffer_offset;
	RGBCups[RGBCupNumber1].DataBufferSize = RGB_CUPS_DATA_BUFFER_SIZE_CUP1;
	buffer_offset += RGB_CUPS_DATA_BUFFER_SIZE_CUP1;
	
	RGBCups[RGBCupNumber2].DataBuffer = data_buffer_cups_12 + buffer_offset;
	RGBCups[RGBCupNumber2].DataBufferSize = RGB_CUPS_DATA_BUFFER_SIZE_CUP2;
	
	RGBCup_AdapterInit(&RGBCups[RGBCupNumber1], &AdapterCup12);
	RGBCup_AdapterInit(&RGBCups[RGBCupNumber2], &AdapterCup12);
	//----------------------------------------------------------------------------
	buffer_offset = RGB_CUPS_SYNC_START_DATA_COUNT;
	RGBCups[RGBCupNumber3].DataBuffer = data_buffer_cups_34 + buffer_offset;
	RGBCups[RGBCupNumber3].DataBufferSize = RGB_CUPS_DATA_BUFFER_SIZE_CUP3;
	buffer_offset += RGB_CUPS_DATA_BUFFER_SIZE_CUP3;
	
	RGBCups[RGBCupNumber4].DataBuffer = data_buffer_cups_34 + buffer_offset;
	RGBCups[RGBCupNumber4].DataBufferSize = RGB_CUPS_DATA_BUFFER_SIZE_CUP4;
	
	RGBCup_AdapterInit(&RGBCups[RGBCupNumber3], &AdapterCup34);
	RGBCup_AdapterInit(&RGBCups[RGBCupNumber4], &AdapterCup34);
	//----------------------------------------------------------------------------
	WS2812_PixelT Pixel =
	{
		.Green = 0x01,
		.Red = 0x01,
		.Blue = 0x01
	};
	
	WS2812_FillPixels(&RGBCups[RGBCupNumber1].Driver, Pixel, 0, 8);
	WS2812_FillPixels(&RGBCups[RGBCupNumber2].Driver, Pixel, 0, 8);
	WS2812_FillPixels(&RGBCups[RGBCupNumber3].Driver, Pixel, 0, 8);
	WS2812_FillPixels(&RGBCups[RGBCupNumber4].Driver, Pixel, 0, 8);
	//----------------------------------------------------------------------------
	/*
	xRxRequestManagerInit(&RGBCups.RequestManager,
												&RGBCups,
												&RGBCups,
												(xRxRequestT*)RGBCups_RxRequests);
	*/
  return 0;
}
//==============================================================================
