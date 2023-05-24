//==============================================================================
//includes:

#include "WiFi-Adapter.h"
//==============================================================================
//variables:


//==============================================================================
//prototypes:

static xResult PrivateRequestSetConfig(xWiFi_T* wifi, xWiFi_ConfigT* request);
static xResult PrivateRequestEnable(xWiFi_T* wifi);
static xResult PrivateRequestDisable(xWiFi_T* wifi);
static xResult PrivateRequestInit(xWiFi_T* wifi);
//==============================================================================
//functions:

static void PrivateHandler(xWiFi_T* wifi)
{

}
//------------------------------------------------------------------------------
static void PrivateIRQ(xWiFi_T* wifi, void* arg)
{
	//register ADC_AdapterT* adapter = (UsartPortAdapterT*)port->Adapter;
}
//------------------------------------------------------------------------------
static xResult PrivateRequestListener(xWiFi_T* wifi, xWiFi_RequestSelector selector, void* arg)
{
	//register ADC_AdapterT* adapter = (ADC_AdapterT*)adc->Adapter;

	switch ((uint32_t)selector)
	{
		case xWiFi_RequestSetConfig:
			return PrivateRequestSetConfig(wifi, arg);

		case xWiFi_RequestEnable:
			return PrivateRequestEnable(wifi);

		case xWiFi_RequestDisable:
			return PrivateRequestDisable(wifi);

		case xWiFi_RequestInit:
			return PrivateRequestInit(wifi);

		default : return xResultRequestIsNotFound;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
static void PrivateEventListener(xWiFi_T* wifi, xWiFi_EventSelector selector, void* arg)
{
	//register ADC_AdapterT* adapter = (ADC_AdapterT*)port->Adapter;

	switch((int)selector)
	{
		default: return;
	}
}
//------------------------------------------------------------------------------
static xResult PrivateRequestSetConfig(xWiFi_T* wifi, xWiFi_ConfigT* request)
{
	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateRequestEnable(xWiFi_T* wifi)
{
	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateRequestDisable(xWiFi_T* wifi)
{
	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateRequestInit(xWiFi_T* wifi)
{
	return xResultAccept;
}
//==============================================================================
//initializations:

static xWiFi_InterfaceT PrivateInterface =
{
	.Handler = (xWiFi_HandlerT)PrivateHandler,
	.IRQ = (xWiFi_IRQT)PrivateIRQ,

	.RequestListener = (xWiFi_RequestListenerT)PrivateRequestListener,
	.EventListener = (xWiFi_EventListenerT)PrivateEventListener,
};
//------------------------------------------------------------------------------
xResult WiFi_AdapterInit(xWiFi_T* wifi, WiFi_AdapterT* adapter, WiFi_AdapterInitializationT* initialization)
{
	if (!wifi || !adapter || !initialization)
	{
		return xResultLinkError;
	}

	wifi->Adapter = (xWiFi_AdapterBaseT*)adapter;

	wifi->Adapter->Base.Parent = wifi;
	wifi->Adapter->Base.Note = nameof(WiFi_AdapterT);

	wifi->Adapter->Interface = &PrivateInterface;
  
  return xResultError;
}
//==============================================================================
