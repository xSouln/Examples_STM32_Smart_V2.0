//==============================================================================
#include "SerialPortUSB_RxAdapter.h"
//#include "SerialPort/Controls/SerialPort_Types.h"
//==============================================================================
static void Handler(xRxT* rx)
{
	SerialPortUSB_AdapterT* adapter = rx->Adapter;
	
	xRxReceiverRead(&adapter->RxReceiver, &adapter->RxCircleBuffer);
}
//------------------------------------------------------------------------------
static void EventListener(xRxT* rx, xRxEventSelector event, uint32_t args, uint32_t count)
{
	switch ((uint32_t)event)
	{
		default : break;
	}
}
//------------------------------------------------------------------------------
static xResult RequestListener(xRxT* rx, xRxRequestSelector selector, uint32_t args, uint32_t count)
{
	switch ((uint32_t)selector)
	{
		default : return xResultRequestIsNotFound;
	}
	
	return xResultRequestIsNotFound;
}
//------------------------------------------------------------------------------
static int GetValue(xRxT* rx, xRxValueSelector selector)
{
	switch ((uint32_t)selector)
	{
		default : return -1;
	}
}
//------------------------------------------------------------------------------
static xResult SetValue(xRxT* rx, xRxValueSelector selector, uint32_t value)
{
	switch ((uint32_t)selector)
	{
		default : return xResultValueIsNotFound;
	}
	
	return xResultValueIsNotFound;
}
//==============================================================================
static xRxInterfaceT interface =
{
	.Handler = (xRxActionHandler)Handler,
	.EventListener = (xRxEventListener)EventListener,
	.RequestListener = (xRxRequestListener)RequestListener,
	.GetValue = (xRxActionGetValue)GetValue,
	.SetValue = (xRxActionSetValue)SetValue
};
//==============================================================================
int SerialPortUSB_RxAdapterInit(xRxT* rx, void* parent, SerialPortUSB_AdapterT* adapter)
{
	if (rx && adapter)
	{
		xRxInit(rx, parent, adapter, &interface);
		
		return SerialPortAdapterResultAccept;
	}
	
	return SerialPortAdapterResultError;
}
//==============================================================================
