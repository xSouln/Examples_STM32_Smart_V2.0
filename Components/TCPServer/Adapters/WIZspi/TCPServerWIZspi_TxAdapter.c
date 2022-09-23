//==============================================================================
#include "TCPServerWIZspi_TxAdapter.h"
#include "TCPServer/TCPServer_Types.h"
//==============================================================================
static int TransmitData(xTxT *tx, void* data, uint32_t data_size)
{
	TCPServerWIZspi_AdapterT* adapter = tx->Adapter;
	TCPServerT* server = adapter->Server;
	
	if(data && data_size && server->Sock.Status.Connected)
  {
    return send(server->Sock.Number, (uint8_t*)data, data_size);
  }
	
	return data_size;
}
//==============================================================================
static void Handler(xTxT *tx)
{

}
//------------------------------------------------------------------------------
static void EventListener(xTxT *tx, xTxEventSelector event, uint32_t args, uint32_t count)
{
	TCPServerWIZspi_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)event)
	{
		default : break;
	}
}
//------------------------------------------------------------------------------
static xResult RequestListener(xTxT* tx, xTxRequestSelector selector, uint32_t args, uint32_t count)
{
	TCPServerWIZspi_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)selector)
	{
		case xTxRequestTransmitData: return TransmitData(tx, (void*)args, count);
			
		default : return xResultRequestIsNotFound;
	}
	
	return xResultRequestIsNotFound;
}
//------------------------------------------------------------------------------
static int GetValue(xTxT* tx, xTxValueSelector selector)
{
	TCPServerWIZspi_AdapterT* adapter = tx->Adapter;
	TCPServerT* server = adapter->Server;
	
	switch ((uint32_t)selector)
	{
		case xTxValueFreeBufferSize:
			return server->Sock.Status.Connected ? getSn_TX_FSR(server->Sock.Number) : 0;
		
		default : return -1;
	}
}
//------------------------------------------------------------------------------
static xResult SetValue(xTxT* tx, xTxValueSelector selector, uint32_t value)
{
	TCPServerWIZspi_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)selector)
	{
		default : return xResultValueIsNotFound;
	}
	
	return xResultValueIsNotFound;
}
//------------------------------------------------------------------------------
static xTxInterfaceT interface =
{
	.Handler = (xTxActionHandler)Handler,
	.EventListener = (xTxEventListener)EventListener,
	.RequestListener = (xTxRequestListener)RequestListener,
	.GetValue = (xTxActionGetValue)GetValue,
	.SetValue = (xTxActionSetValue)SetValue,
	//.TransmitData = (xTxActionTransmitData)TransmitData
};
//==============================================================================
int TCPServerWIZspi_TxAdapterInit(xTxT* tx, void* parent, TCPServerWIZspi_AdapterT* adapter)
{
	if (tx && adapter)
	{
		return xTxInit(tx, parent, adapter, &interface, adapter->TxDataBuffer);
	}
	return TCPServerAdapterResultError;
}
//==============================================================================
