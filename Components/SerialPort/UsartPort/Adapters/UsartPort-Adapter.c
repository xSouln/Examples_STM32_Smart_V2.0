//==============================================================================
//includes:

#include "UsartPort-Adapter.h"
//==============================================================================
//functions:

static void PrivateHandler(xPortT* port)
{
	register UsartPortAdapterT* adapter = (UsartPortAdapterT*)port->Adapter;

	adapter->RxCircleBuffer.TotalIndex = (adapter->RxCircleBuffer.SizeMask + 1) -
											((DMA_Channel_TypeDef*)adapter->RxDMA->Instance)->CNDTR;

	if (!adapter->Usart->Control1.TxEmptyInterruptEnable
	&& adapter->TxCircleBuffer.TotalIndex != adapter->TxCircleBuffer.HandlerIndex)
	{
		adapter->Usart->Control1.TxEmptyInterruptEnable = true;
	}

	port->Tx.IsTransmitting = adapter->Usart->Control1.TxEmptyInterruptEnable;

	xRxReceiverRead(&adapter->RxReceiver, &adapter->RxCircleBuffer);
}
//------------------------------------------------------------------------------
static void PrivateIRQ(xPortT* port, void* arg)
{
	register UsartPortAdapterT* adapter = (UsartPortAdapterT*)port->Adapter;

	if (adapter->Usart->Control1.TxEmptyInterruptEnable && adapter->Usart->InterruptAndStatus.TxEmpty)
	{
		if (adapter->TxCircleBuffer.HandlerIndex != adapter->TxCircleBuffer.TotalIndex)
		{
			adapter->Usart->Data = xCircleBufferGet(&adapter->TxCircleBuffer);
		}
		else
		{
			adapter->Usart->Control1.TxEmptyInterruptEnable = false;
		}
	}
}
//------------------------------------------------------------------------------
static xResult PrivateRequestListener(xPortT* port, xPortRequestSelector selector, void* arg)
{
	register UsartPortAdapterT* adapter = (UsartPortAdapterT*)port->Adapter;

	switch ((uint32_t)selector)
	{
		case xPortRequestEnableTx:
			adapter->Usart->Control1.TransmitterEnable = true;
			break;

		case xPortRequestDisableTx:
			adapter->Usart->Control1.TransmitterEnable = false;
			break;

		case xPortRequestUpdateTxStatus:
			port->Tx.IsEnable = adapter->Usart->Control1.TransmitterEnable;
			port->Tx.IsTransmitting = adapter->Usart->Control1.TxEmptyInterruptEnable;
			break;

		case xPortRequestUpdateRxStatus:
			port->Rx.IsEnable = adapter->Usart->Control1.ReceiverEnable;
			break;

		case xPortRequestGetRxBuffer:
			*(uint8_t**)arg = adapter->RxReceiver.Buffer;
			break;

		case xPortRequestGetRxBufferSize:
			*(uint32_t*)arg = adapter->RxReceiver.BufferSize;
			break;

		case xPortRequestGetRxBufferFreeSize:
			*(uint32_t*)arg = adapter->RxReceiver.BufferSize - adapter->RxReceiver.BytesReceived;
			break;

		case xPortRequestClearRxBuffer:
			adapter->RxReceiver.BytesReceived = 0;
			break;

		case xPortRequestGetTxBufferSize:
			*(uint32_t*)arg = adapter->TxCircleBuffer.SizeMask + 1;
			break;

		case xPortRequestGetTxBufferFreeSize:
			*(uint32_t*)arg = xCircleBufferGetFreeSize(&adapter->TxCircleBuffer);
			break;

		case xPortRequestClearTxBuffer:
			adapter->TxCircleBuffer.HandlerIndex = adapter->TxCircleBuffer.TotalIndex;
			break;

		default : return xResultRequestIsNotFound;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
static void PrivateEventListener(xPortT* port, xPortEventSelector selector, void* arg)
{
	//register UsartPortAdapterT* adapter = (UsartPortAdapterT*)port->Adapter;

	switch((int)selector)
	{
		default: return;
	}
}
//------------------------------------------------------------------------------
static int PrivateTransmit(xPortT* port, void* data, uint32_t size)
{
	register UsartPortAdapterT* adapter = (UsartPortAdapterT*)port->Adapter;

	if (xCircleBufferGetFreeSize(&adapter->TxCircleBuffer) >= size)
	{
		xCircleBufferAdd(&adapter->TxCircleBuffer, (uint8_t*)data, size);

		return size;
	}

	return -xResultError;
}
//------------------------------------------------------------------------------
static int PrivateReceive(xPortT* port, void* data, uint32_t size)
{
	return -xResultNotSupported;
}
//------------------------------------------------------------------------------
static void PrivateRxReceiverEventListener(xRxReceiverT* receiver, xRxReceiverEventSelector event, void* arg)
{
	register xPortT* port = receiver->Base.Parent;

	switch ((uint8_t)event)
	{
		case xRxReceiverEventEndLine:
			xPortEventListener(port, xPortSysEventRxFoundEndLine, arg);
			break;

		case xRxReceiverEventBufferIsFull:
			xPortEventListener(port, xPortSysEventRxBufferIsFull, arg);
			break;

		default: return;
	}
}
//==============================================================================
//initializations:

static xPortInterfaceT PrivatePortInterface =
{
	.Handler = (xPortHandlerT)PrivateHandler,
	.IRQ = (xPortIRQT)PrivateIRQ,

	.RequestListener = (xPortRequestListenerT)PrivateRequestListener,
	.EventListener = (xPortEventListenerT)PrivateEventListener,

	.Transmit = (xPortTransmitActionT)PrivateTransmit,
	.Receive = (xPortReceiveActionT)PrivateReceive
};
//------------------------------------------------------------------------------
static xRxReceiverInterfaceT PrivateRxReceiverInterface =
{
	.EventListener = (xRxReceiverEventListenerT)PrivateRxReceiverEventListener
};
//------------------------------------------------------------------------------
xResult UsartPortAdapterInit(xPortT* port, UsartPortAdapterT* adapter)
{
	if (port && adapter)
	{
		port->Adapter = (xPortAdapterBaseT*)adapter;

		port->Adapter->Base.Note = nameof(UsartPortAdapterT);
		port->Adapter->Base.Parent = port;
		
		port->Adapter->Interface = &PrivatePortInterface;

		adapter->RxReceiver.Interface = &PrivateRxReceiverInterface;

		adapter->Usart->Control1.TxCompleteInterruptEnable = false;
		adapter->Usart->Control1.TxEmptyInterruptEnable = false;
		adapter->Usart->Control1.RxNotEmptyInterruptEnable = false;

		(void)adapter->Usart->Data;
		adapter->Usart->InterruptAndStatus.Value = 0;
		adapter->Usart->Control3.RxDMA_Enable = true;

		if (adapter->RxDMA)
		{
			uint8_t dma_result = HAL_DMA_Start(adapter->RxDMA,
												(uint32_t)&adapter->Usart->Data,
												(uint32_t)adapter->RxCircleBuffer.Buffer,
												adapter->RxCircleBuffer.SizeMask + 1);

			if (dma_result != HAL_OK)
			{
				return xResultError;
			}
		}
		
		adapter->Usart->Control1.TransmitterEnable = true;
		adapter->Usart->Control1.ReceiverEnable = true;
		
		return xResultAccept;
	}
  
  return xResultError;
}
//==============================================================================
