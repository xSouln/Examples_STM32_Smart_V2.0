//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "WS2812.h"
//==============================================================================
WS2812_Result WS2812_SetOptions(WS2812_T* driver, WS2812_OptionsT* options)
{
	if (driver && options)
	{
		memcpy((uint8_t*)&driver->Options, (uint8_t*)options, sizeof(WS2812_OptionsT));
		
		return WS2812_ResultAccept;
	}
	
	return WS2812_ResultError;
}
//------------------------------------------------------------------------------
uint16_t PutColorToArray(WS2812_T* driver, uint8_t color, uint8_t* data)
{
	uint16_t offset = 0;
	uint16_t period;
	
	if (driver && driver->Status.IsInit)
	{
		period = driver->Control->GetPeriod(driver);
		
		for (uint8_t bit_number = 0; bit_number < sizeof(color) * 8; bit_number++)
		{
			if (color & 0x80)
			{
				data[offset] = (uint8_t)((float)(period + 1) * 0.85 / 1.25);
			}
			else
			{
				data[offset] = (uint8_t)((float)(period + 1) * 0.4 / 1.25);
			}
			
			color <<= 1;
			offset++;
		}
	}
	
	return offset;
}
//==============================================================================
WS2812_Result WS2812_Init(WS2812_T* driver,
													void* parent,
													WS2812_ControlT* control,
													uint8_t* buffer,
													uint16_t buffer_size)
{
	if (driver)
	{
		driver->Description = "WS2812_T";
		driver->Parent = parent;
		
		
	}
	
	return WS2812_ResultError;
}
//==============================================================================
