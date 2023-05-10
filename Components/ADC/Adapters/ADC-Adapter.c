//==============================================================================
//includes:

#include "ADC-Adapter.h"
//==============================================================================
//functions:

static void PrivateHandler(xADC_T* adc)
{
	register ADC_AdapterT* adapter = (ADC_AdapterT*)adc->Adapter;
	DMA_Channel_TypeDef* dma = adapter->Handle->DMA_Handle->Instance;

	adc->Points->Buffer.TotalIndex = ((adc->Points->Buffer.SizeMask + 1) *  adapter->ChannelsCount - dma->CNDTR) / adapter->ChannelsCount;
}
//------------------------------------------------------------------------------
static void PrivateIRQ(xADC_T* object, void* arg)
{
	//register ADC_AdapterT* adapter = (UsartPortAdapterT*)port->Adapter;
}
//------------------------------------------------------------------------------
static xResult PrivateRequestListener(xADC_T* adc, xADC_RequestSelector selector, void* arg)
{
	register ADC_AdapterT* adapter = (ADC_AdapterT*)adc->Adapter;

	switch ((uint32_t)selector)
	{
		case xADC_RequestStart:
			adapter->Timer->Control1.CounterEnable = true;
			break;

		case xADC_RequestStop:
			adapter->Timer->Control1.CounterEnable = false;
			break;

		case xADC_RequestGetNumberOfNewPoints:
			*((uint32_t*)arg) = xCircleBufferGetFilledSize(&adc->Points->Buffer);
			break;

		default : return xResultRequestIsNotFound;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
static void PrivateEventListener(xADC_T* object, xADC_EventSelector selector, void* arg)
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

static xADC_InterfaceT PrivatePortInterface =
{
	.Handler = (xADC_HandlerT)PrivateHandler,
	.IRQ = (xADC_IRQT)PrivateIRQ,

	.RequestListener = (xADC_RequestListenerT)PrivateRequestListener,
	.EventListener = (xADC_EventListenerT)PrivateEventListener,
};
//------------------------------------------------------------------------------
xResult ADC_AdapterInit(xADC_T* adc, ADC_AdapterInitializationT* initialization)
{
	if (adc && initialization)
	{
		ADC_AdapterT* adapter = initialization->Adapter;

		adc->Adapter = (xADC_AdapterBaseT*)adapter;

		adc->Adapter->Base.Note = nameof(ADC_AdapterT);
		adc->Adapter->Base.Parent = adc;
		
		adapter->Base.Interface = &PrivatePortInterface;
		
		adc->Points = initialization->PointsBuffer;
		adc->NumberOfPointsBuffer = 1;

		adapter->Handle = initialization->Handle;
		adapter->Timer = initialization->Timer;
		adapter->ChannelsCount = initialization->ChannelsCount;

		xCircleBufferInit(&adc->Points->Buffer,
				initialization->PointsMemmory,
				initialization->SizeOfPointsMemmory / SIZE_ELEMENT(initialization->PointsMemmory) / initialization->ChannelsCount - 1,
				SIZE_ELEMENT(initialization->PointsMemmory) * initialization->ChannelsCount);

		HAL_ADC_Start_DMA(initialization->Handle, adc->Points->Buffer.Memory, initialization->SizeOfPointsMemmory / SIZE_ELEMENT(initialization->PointsMemmory));
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
