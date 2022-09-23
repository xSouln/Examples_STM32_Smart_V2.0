//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "WS2812.h"
//==============================================================================
uint16_t WS2812_PutColorToArray(uint8_t* buffer, uint16_t period, uint8_t color)
{
	uint16_t offset = 0;
	
	for (uint8_t bit_number = 0; bit_number < sizeof(color) * 8; bit_number++)
	{
		if (color & 0x80)
		{
			buffer[offset] = (uint8_t)((float)(period + 1) * 0.85 / 1.25);
		}
		else
		{
			buffer[offset] = (uint8_t)((float)(period + 1) * 0.4 / 1.25);
		}
		
		color <<= 1;
		offset++;
	}
	
	return offset;
}
//------------------------------------------------------------------------------
uint16_t WS2812_PutPixelsToArray(uint8_t* buffer, uint16_t period, WS2812_PixelT* pixels, int pixels_count, WS2812_PixelAddMode mode)
{
	uint16_t size = 0;
	
	for (uint16_t i = 0; i < pixels_count; i += (uint8_t)mode)
	{
		size += WS2812_PutColorToArray(buffer + size, period, pixels[i].Green);
		size += WS2812_PutColorToArray(buffer + size, period, pixels[i].Red);
		size += WS2812_PutColorToArray(buffer + size, period, pixels[i].Blue);
	}
	
	return size;
}
//------------------------------------------------------------------------------
uint16_t WS2812_FillPixels(WS2812_T* driver, WS2812_PixelT pixel, uint16_t start_position, uint16_t pixels_count)
{
	if (driver && driver->Status.DriverInit)
	{
		uint16_t buffer_position = start_position * WS2812_BITS_IN_PIXEL;
		
		if ((buffer_position + (pixels_count * WS2812_BITS_IN_PIXEL)) <= driver->BufferSize)
		{
			return WS2812_PutPixelsToArray(driver->Buffer + buffer_position,
																			driver->Interface->GetValue(driver, WS2812_ValuePeriod),
																			&pixel,
																			pixels_count,
																			WS2812_PixelAddModeFill);
		}
	}
	return 0;
}
//------------------------------------------------------------------------------
uint16_t WS2812_SetPixels(WS2812_T* driver, WS2812_PixelT* pixels, uint16_t start_position, uint16_t pixels_count)
{
	if (driver && driver->Status.DriverInit)
	{
		uint16_t buffer_position = start_position * WS2812_BITS_IN_PIXEL;
		
		if ((buffer_position + (WS2812_BITS_IN_PIXEL * pixels_count)) <= driver->BufferSize)
		{
			return WS2812_PutPixelsToArray(driver->Buffer + buffer_position,
																			driver->Interface->GetValue(driver, WS2812_ValuePeriod),
																			pixels,
																			pixels_count,
																			WS2812_PixelAddModePut);
		}
	}
	
	return 0;
}
//------------------------------------------------------------------------------
uint16_t WS2812_SetPixel(WS2812_T* driver, WS2812_PixelT pixel, uint16_t position)
{
	return WS2812_SetPixels(driver, &pixel, position, 1);
}
//------------------------------------------------------------------------------
WS2812_TransmitterStatus WS2812_GetTransmitterStatus(WS2812_T* driver)
{
	if (driver && driver->Status.DriverInit)
	{
		driver->Status.Transmitter = driver->Interface->GetValue(driver, WS2812_ValueTransmitterStatus);
		return driver->Status.Transmitter;
	}
	
	return WS2812_TransmitterUndefined;
}
//------------------------------------------------------------------------------
WS2812_Result WS2812_DrawingStart(WS2812_T* driver, WS2812_DrawingManagerT* drawing_manager)
{
	if (driver && driver->Status.DriverInit && drawing_manager)
	{
		memcpy((uint8_t*)&driver->DrawingManager, (uint8_t*)drawing_manager, sizeof(WS2812_DrawingManagerT));
		
		driver->Status.DrawingIsEnable = true;
	}
	
	return WS2812_ResultError;
}
//------------------------------------------------------------------------------
void WS2812_DrawingStop(WS2812_T* driver)
{
	if (driver && driver->Status.DriverInit)
	{
		driver->Status.DrawingIsEnable = false;
	}
}
//------------------------------------------------------------------------------
void WS2812_Draw(WS2812_T* driver)
{
	if (driver->Status.DrawingIsEnable)
	{
		driver->DrawingManager.Handler(driver);
	}
}
//------------------------------------------------------------------------------
WS2812_Result WS2812_UpdateLayout(WS2812_T* driver)
{
	if (driver && driver->Status.DriverInit)
	{
		driver->Interface->RequestListener(driver, WS2812_RequestTransmitterEnable, 0, 0);
	}
	
	return WS2812_ResultError;
}
//==============================================================================
WS2812_Result WS2812_Init(WS2812_T* driver,
													void* parent,
													WS2812_AdapterT* adapter,
													WS2812_InterfaceT* interface,
													uint8_t* buffer,
													uint16_t buffer_size)
{
	if (driver)
	{
		driver->Description = "WS2812_T";
		driver->Parent = parent;
		
		driver->Adapter = adapter;
		driver->Interface = interface;
		
		driver->Buffer = buffer;
		driver->BufferSize = buffer_size;
		
		driver->Status.DriverInit = true;
	}
	
	return WS2812_ResultError;
}
//==============================================================================
