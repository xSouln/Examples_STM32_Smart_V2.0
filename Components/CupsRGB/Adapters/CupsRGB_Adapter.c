//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "CupsRGB_Adapter.h"
#include "main.h"
#include "usart.h"
//==============================================================================
static CupsRGB_Result Write(WS2812_T* driver, WS2812_PixelT* pixels, uint16_t pixels_count)
{
	
	return CupsRGB_ResultError;
}
//==============================================================================
static WS2812_ControlT control =
{
	.Handle = &huart1
};
//==============================================================================
CupsRGB_Result CapsRGB_AdapterInit(CupsRGB_AdapterT* adapter, void* parent)
{
	if (adapter)
	{
		adapter->Description = "WS2812_AdapterT";
		adapter->Parent = parent;
		
		return WS2812_Init(&adapter->Driver1,
												adapter,
												&control,
												0, 0);
	}
	return WS2812_ResultNullPointer;
}
//==============================================================================
