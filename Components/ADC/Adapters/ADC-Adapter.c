//==============================================================================
//includes:

#include "ADC-Adapter.h"
//==============================================================================
//functions:

static void PrivateHandler(xADC_T* adc)
{
	register ADC_AdapterT* adapter = (ADC_AdapterT*)adc->Adapter.Content;
	DMA_Channel_TypeDef* dma = adapter->Handle->DMA_Handle->Instance;

	adc->Points->Buffer.TotalIndex = ((adc->Points->Buffer.SizeMask + 1) *  adapter->ChannelsCount - dma->CNDTR) / adapter->ChannelsCount;
}
//------------------------------------------------------------------------------
static void PrivateIRQ(xADC_T* object, void* arg)
{
	//register ADC_AdapterT* adapter = (UsartPortAdapterT*)port->Adapter;
}
//------------------------------------------------------------------------------
static xResult PrivateRequestListener(xADC_T* adc, xADC_AdapterRequestSelector selector, void* arg)
{
	register ADC_AdapterT* adapter = (ADC_AdapterT*)adc->Adapter.Content;

	switch ((uint32_t)selector)
	{
		case xADC_AdapterRequestStart:
			adapter->Timer->Control1.CounterEnable = true;
			break;

		case xADC_AdapterRequestStop:
			adapter->Timer->Control1.CounterEnable = false;
			break;

		case xADC_AdapterRequestGetNumberOfNewPoints:
			*((uint32_t*)arg) = xCircleBufferGetFilledSize(&adc->Points->Buffer);
			break;

		default : return xResultRequestIsNotFound;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
static void PrivateEventListener(xADC_T* object, xADC_AdapterEventSelector selector, void* arg)
{
	//register ADC_AdapterT* adapter = (ADC_AdapterT*)port->Adapter;

	switch((int)selector)
	{
		default: return;
	}
}
//------------------------------------------------------------------------------

//==============================================================================
//initializations:

static xADC_AdapterInterfaceT PrivateInterface =
{
	.Handler = (xADC_AdapterHandlerT)PrivateHandler,
	.IRQ = (xADC_AdapterIRQT)PrivateIRQ,

	.RequestListener = (xADC_AdapterRequestListenerT)PrivateRequestListener,
	.EventListener = (xADC_AdapterEventListenerT)PrivateEventListener,
};
//------------------------------------------------------------------------------
xResult ADC_AdapterInit(xADC_T* adc, xADC_AdapterInitT* init)
{
	if (adc && init)
	{
		ADC_AdapterInitT* adapterInit = init->Init;
		ADC_AdapterT* adapter = init->Adapter;

		adc->Adapter.Description = nameof(ADC_AdapterT);
		adc->Adapter.Content = adapter;
		adc->Adapter.Interface = &PrivateInterface;
		
		adc->Points = adapterInit->PointsBuffer;
		adc->NumberOfPointsBuffer = 1;

		adapter->Handle = adapterInit->Handle;
		adapter->Timer = adapterInit->Timer;
		adapter->ChannelsCount = adapterInit->ChannelsCount;

		xCircleBufferInit(&adc->Points->Buffer,
				adapterInit->PointsMemmory,
				adapterInit->SizeOfPointsMemmory / SIZE_ELEMENT(adapterInit->PointsMemmory) / adapterInit->ChannelsCount - 1,
				SIZE_ELEMENT(adapterInit->PointsMemmory) * adapterInit->ChannelsCount);

		HAL_ADC_Start_DMA(adapterInit->Handle,
				adc->Points->Buffer.Memory,
				adapterInit->SizeOfPointsMemmory / SIZE_ELEMENT(adapterInit->PointsMemmory));
		/*
		HAL_DMA_Start(adapter->Handle->DMA_Handle,
						adapter->Handle->Instance->DR,
						(uint32_t)adc->Points->Buffer.Memory,
						initialization->SizeOfPointsMemmory / SIZE_ELEMENT(initialization->PointsMemmory));

		HAL_ADC_Start(adapter->Handle);
		 */
		return xResultAccept;
	}
  
  return xResultError;
}
//==============================================================================
