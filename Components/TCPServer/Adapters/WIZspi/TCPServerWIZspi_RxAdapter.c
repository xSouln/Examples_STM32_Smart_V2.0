//==============================================================================
#include "TCPServerWIZspi_RxAdapter.h"
#include "TCPServer/TCPServer_Types.h"
//==============================================================================
static void Handler(xRxT* rx)
{
	TCPServerWIZspi_AdapterT* adapter = rx->Adapter;
	TCPServerT* server = adapter->Server;
	
	if (server->Sock.Status.Connected)
	{
		uint16_t len = getSn_RX_RSR(server->Sock.Number);		
    if (len)
    {
      if (len > adapter->RxBufferSize)
			{
				len = adapter->RxBufferSize;
			}
			
      len = recv(server->Sock.Number, adapter->RxBuffer, len);
			xRxReceiverReceive(&adapter->RxReceiver, adapter->RxBuffer, len);
    }
	}
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
	TCPServerWIZspi_AdapterT* adapter = rx->Adapter;
	
	switch ((uint32_t)selector)
	{
		case xRxRequestReceive :
			xRxReceiverReceive(&adapter->RxReceiver, (uint8_t*)args, count);
		
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
	.SetValue = (xRxActionSetValue)SetValue,
};
//==============================================================================
int TCPServerWIZspi_RxAdapterInit(xRxT* rx, void* parent, TCPServerWIZspi_AdapterT* adapter)
{
	if (rx && adapter)
	{
		xRxInit(rx, parent, adapter, &interface);
		
		return TCPServerAdapterResultAccept;
	}
	
	return TCPServerAdapterResultError;
}
//==============================================================================
