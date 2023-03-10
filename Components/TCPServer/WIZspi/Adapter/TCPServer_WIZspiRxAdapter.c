//==============================================================================
//includes:

#include "TCPServer_WIZspiRxAdapter.h"
#include "WIZ/W5500/w5500.h"
#include "WIZ/socket.h"
//==============================================================================
//functions:

static void PrivateHandler(xRxT* rx)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;
	TCPServerT* server = rx->Object.Parent;
	
	if (server->Sock.State == TCPServerSockConnected)
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
static void PrivateEventListener(xRxT* rx, xRxEventSelector event, void* arg, ...)
{
	switch ((uint32_t)event)
	{
		default : break;
	}
}
//------------------------------------------------------------------------------
static void PrivateIRQListener(xRxT* rx)
{

}
//------------------------------------------------------------------------------
static xResult PrivateRequestListener(xRxT* rx, xRxRequestSelector selector, void* arg, ...)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;
	
	switch ((uint32_t)selector)
	{
		case xRxRequestClearBuffer :
			adapter->RxReceiver.BytesReceived = 0;
			break;

		case xRxRequestClearResponseBuffer:
			xDataBufferClear(adapter->ResponseBuffer);
			break;
		
		default: return xResultRequestIsNotFound;
	}
	
	return xResultRequestIsNotFound;
}
//------------------------------------------------------------------------------
static void PrivateRxReceiverEventListener(xRxReceiverT* receiver, xRxReceiverEventSelector event, void* arg, ...)
{
	TCPServerT* server = receiver->Parent->Object.Parent;

	TCPServerReceivedDataT received_data =
	{
		.Data = arg,
		.Size = *(uint32_t*)(&arg + 1),
	};

	switch ((uint8_t)event)
	{
		case xRxReceiverEventEndLine:
			TCPServerEventListener(server, TCPServerEventEndLine, &received_data);
			break;
		
		case xRxReceiverEventBufferIsFull:
			TCPServerEventListener(server, TCPServerEventBufferIsFull, &received_data);
			break;

		default: return;
	}
}
//------------------------------------------------------------------------------

static xResult PrivateReceive(xRxT* rx, uint8_t* data, uint32_t size)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;

	xRxReceiverReceive(&adapter->RxReceiver, data, size);

	return xResultAccept;
}
//------------------------------------------------------------------------------

static xResult PrivatePutInResponseBuffer(xRxT* rx, uint8_t* data, uint32_t size)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;

	xDataBufferAdd(adapter->ResponseBuffer, data, size);

	return xResultAccept;
}
//------------------------------------------------------------------------------

static uint8_t* PrivateGetBuffer(xRxT* rx)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;

	return adapter->RxReceiver.Buffer;
}
//------------------------------------------------------------------------------

static uint32_t PrivateGetBufferSize(xRxT* rx)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;

	return adapter->RxReceiver.BufferSize;
}
//------------------------------------------------------------------------------

static uint32_t PrivateGetBytesCountInBuffer(xRxT* rx)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;

	return adapter->RxReceiver.BytesReceived;
}
//------------------------------------------------------------------------------

static uint8_t* PrivateGetResponseBuffer(xRxT* rx)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;

	return adapter->ResponseBuffer->Data;
}
//------------------------------------------------------------------------------

static uint32_t PrivateGetResponseBufferSize(xRxT* rx)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;

	return adapter->ResponseBuffer->Size;
}
//------------------------------------------------------------------------------

static uint32_t PrivateGetBytesCountInResponseBuffer(xRxT* rx)
{
	TCPServerWIZspiAdapterT* adapter = rx->Adapter;

	return adapter->ResponseBuffer->DataSize;
}
//==============================================================================
//interfaces:

static xRxReceiverInterfaceT rx_receiver_interface =
{
	INITIALIZATION_EVENT_LISTENER(xRxReceiver, PrivateRxReceiverEventListener),
};
//------------------------------------------------------------------------------
static xRxInterfaceT rx_interface =
{
	INITIALIZATION_HANDLER(xRx, PrivateHandler),
	INITIALIZATION_EVENT_LISTENER(xRx, PrivateEventListener),
	INITIALIZATION_IRQ_LISTENER(xRx, PrivateIRQListener),
	INITIALIZATION_REQUEST_LISTENER(xRx, PrivateRequestListener),

	.Receive = (xRxReceiveActionT)PrivateReceive,
	.PutInResponseBuffer = (xRxPutInResponseBufferActionT)PrivatePutInResponseBuffer,

	.GetBuffer = (xRxGetBufferActionT)PrivateGetBuffer,
	.GetBufferSize = (xRxGetBufferSizeActionT)PrivateGetBufferSize,
	.GetBytesCountInBuffer = (xRxGetBytesCountInBufferActionT)PrivateGetBytesCountInBuffer,

	.GetResponseBuffer = (xRxGetResponseBufferActionT)PrivateGetResponseBuffer,
	.GetResponseBufferSize = (xRxGetResponseBufferSizeActionT)PrivateGetResponseBufferSize,
	.GetBytesCountInResponseBuffer = (xRxGetBytesCountInResponseBufferActionT)PrivateGetBytesCountInResponseBuffer
};
//==============================================================================
//initialization:

xResult TCPServerWIZspiRxAdapterInit(TCPServerT* server, TCPServerWIZspiAdapterT* adapter)
{
	if (server && adapter)
	{
		if (!adapter->RxReceiver.Interface)
		{
			adapter->RxReceiver.Interface = &rx_receiver_interface;
		}
		
		return xRxInit(&server->Rx, server, adapter, &rx_interface);
	}
	
	return xResultError;
}
//==============================================================================
