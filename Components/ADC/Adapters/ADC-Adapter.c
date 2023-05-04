//==============================================================================
//includes:

#include "ADC-Adapter.h"
//==============================================================================
//functions:

static void PrivateHandler(xADC_T* object)
{
	register ADC_AdapterT* adapter = (ADC_AdapterT*)object->Adapter;
}
//------------------------------------------------------------------------------
static void PrivateIRQ(xADC_T* object, void* arg)
{
	//register ADC_AdapterT* adapter = (UsartPortAdapterT*)port->Adapter;
}
//------------------------------------------------------------------------------
static xResult PrivateRequestListener(xADC_T* object, xADC_RequestSelector selector, void* arg)
{
	register ADC_AdapterT* adapter = (ADC_AdapterT*)object->Adapter;

	switch ((uint32_t)selector)
	{
		case xADC_RequestStart:
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
xResult ADC_AdapterInit(xADC_T* object, ADC_AdapterT* adapter)
{
	if (object && adapter)
	{
		object->Adapter = (xADC_AdapterBaseT*)adapter;

		object->Adapter->Base.Note = nameof(ADC_AdapterT);
		object->Adapter->Base.Parent = object;
		
		object->Adapter->Interface = &PrivatePortInterface;
		
		return xResultAccept;
	}
  
  return xResultError;
}
//==============================================================================
