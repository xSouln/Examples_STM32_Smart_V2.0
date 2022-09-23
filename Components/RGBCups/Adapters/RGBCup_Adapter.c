//==============================================================================
#include "RGBCup_Adapter.h"
//==============================================================================
static void StartTransmite(WS2812_T* driver)
{
	RGBCupAdapterT* adapter = driver->Adapter;
	
	adapter->DMA->CCR &= ~DMA_CCR_EN;

	adapter->DMA->CCR |= DMA_CCR_PL_0;
	adapter->DMA->CCR &= ~DMA_CCR_PSIZE;
	adapter->DMA->CCR &= ~DMA_CCR_MSIZE;
	adapter->DMA->CCR |= DMA_CCR_PSIZE_1;

	adapter->DMA->CNDTR = adapter->DrawMemorySize;
	adapter->DMA->CPAR = (uint32_t)(&adapter->Timer->CaptureCompare1Value + adapter->PWM_Channel);
	adapter->DMA->CMAR = (uint32_t)(adapter->DrawMemory);
	
	adapter->DMA->CCR |= DMA_CCR_TCIE;
	adapter->DMA->CCR |= DMA_CCR_EN;
}
//==============================================================================
static void Handler(WS2812_T* driver)
{
	
}
//------------------------------------------------------------------------------
static void EventListener(WS2812_T* driver, WS2812_EventSelector event, uint32_t args, uint32_t count)
{
	switch ((uint32_t)event)
	{
		default : break;
	}
}
//------------------------------------------------------------------------------
static WS2812_Result RequestListener(WS2812_T* driver, WS2812_RequestSelector selector, uint32_t args, uint32_t count)
{
	switch ((uint32_t)selector)
	{
		case WS2812_RequestTransmitterEnable:
			StartTransmite(driver);
			return xResultAccept;
		
		default : break;
	}
	
	return xResultRequestIsNotFound;
}
//------------------------------------------------------------------------------
static int GetValue(WS2812_T* driver, WS2812_ValueSelector selector)
{
	RGBCupAdapterT* adapter = driver->Adapter;
	
	switch ((uint32_t)selector)
	{
		case WS2812_ValuePeriod :
			return adapter->Timer->Period;
		
		case WS2812_ValueTransmitterStatus :
			return adapter->DMA->CNDTR > 0 ? WS2812_TransmitterIsTransmit : WS2812_TransmitterStopped;
		
		default : return -1;
	}
}
//------------------------------------------------------------------------------
static WS2812_Result SetValue(WS2812_T* driver, WS2812_ValueSelector selector, uint32_t value)
{
	switch ((uint32_t)selector)
	{
		default : break;
	}
	
	return xResultValueIsNotFound;
}
//------------------------------------------------------------------------------
static WS2812_InterfaceT Interface =
{
	.Handler = (WS2812_ActionHandler)Handler,
	.EventListener = (WS2812_EventListener)EventListener,
	.RequestListener = (WS2812_RequestListener)RequestListener,
	.GetValue = (WS2812_ActionGetValue)GetValue,
	.SetValue = (WS2812_ActionSetValue)SetValue
};
//==============================================================================
RGBCupsResult RGBCup_AdapterInit(RGBCupT* cup, RGBCupAdapterT* adapter)
{	
	if (cup && adapter)
	{
		volatile uint32_t *compare_reg = &adapter->Timer->CaptureCompare1Value;
		
		adapter->Timer->Control1.CounterEnable = false;
		
		adapter->Timer->CaptureCompare.Value |= 1 << (adapter->PWM_Channel * 4);
		*(compare_reg + adapter->PWM_Channel) = 0;
		
		adapter->Timer->DMAOrInterrupts.DMA_RequestEnable = true;
		adapter->Timer->Control1.CounterEnable = true;
		
		cup->PixelsCount = cup->DataBufferSize / WS2812_BITS_IN_PIXEL;
		cup->DataBufferSize = cup->PixelsCount * WS2812_BITS_IN_PIXEL;
		
		return WS2812_Init(&cup->Driver,
												cup,
												adapter,
												&Interface,
												cup->DataBuffer, cup->DataBufferSize);
	}
	
	return RGBCupsResultError;
}
//==============================================================================
