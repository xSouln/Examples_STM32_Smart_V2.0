//==============================================================================
//includes:

#include "TCPServer_WIZspiTxAdapter.h"
#include "WIZ/W5500/w5500.h"
#include "socket.h"
//==============================================================================
//functions:

static void PrivateHandler(xTxT *tx)
{

}
//------------------------------------------------------------------------------
static void PrivateEventListener(xTxT *tx, xTxEventSelector selector, void* arg, ...)
{
	//TCPServerWIZspiAdapterT* adapter = tx->Adapter;
	
	switch ((int)selector)
	{
		default : break;
	}
}
//------------------------------------------------------------------------------

static void PrivateIRQListener(xTxT *tx)
{
	//TCPServerWIZspiAdapterT* adapter = tx->Adapter;
}
//------------------------------------------------------------------------------

static xResult PrivateRequestListener(xTxT* tx, xTxRequestSelector selector, void* arg, ...)
{
	//TCPServerWIZspiAdapterT* adapter = tx->Adapter;

	switch ((uint32_t)selector)
	{
		default : return xResultRequestIsNotFound;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------

static xResult PrivateTransmitData(xTxT* tx, void* data, uint32_t size)
{
	TCPServerT* server = tx->Object.Parent;

	if (server->Sock.State == TCPServerSockConnected && data && size)
	{
		while (send(server->Sock.Number, (uint8_t*)data, size) != size) { }

		return xResultAccept;
	}
	return xResultError;
}
//------------------------------------------------------------------------------

static uint32_t PrivateGetBufferSize(xTxT* tx)
{
	TCPServerT* server = tx->Object.Parent;

	return server->Sock.State == TCPServerSockConnected ? getSn_TxMAX(server->Sock.Number) : 0;
}
//------------------------------------------------------------------------------

static uint32_t PrivateGetFreeBufferSize(xTxT* tx)
{
	TCPServerT* server = tx->Object.Parent;

	return server->Sock.State == TCPServerSockConnected ? getSn_TX_FSR(server->Sock.Number) : 0;
}
//==============================================================================
//interfaces:

static xTxInterfaceT interface =
{
	INITIALIZATION_HANDLER(xTx, PrivateHandler),
	INITIALIZATION_EVENT_LISTENER(xTx, PrivateEventListener),
	INITIALIZATION_IRQ_LISTENER(xTx, PrivateIRQListener),
	INITIALIZATION_REQUEST_LISTENER(xTx, PrivateRequestListener),

	.TransmitData = (xTxTransmitDataT)PrivateTransmitData,

	.GetBufferSize = (xTxGetBufferSizeActionT)PrivateGetBufferSize,
	.GetFreeBufferSize = (xTxGetFreeBufferSizeActionT)PrivateGetFreeBufferSize
};
//==============================================================================
//initialization:

xResult TCPServerWIZspiTxAdapterInit(TCPServerT* server, TCPServerWIZspiAdapterT* adapter)
{
	if (server && adapter)
	{
		return xTxInit(&server->Tx, server, adapter, &interface);
	}
	
	return xResultError;
}
//==============================================================================
