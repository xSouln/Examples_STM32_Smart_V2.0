//==============================================================================
//includes:

#include "ADC-TransferLayerAdapter.h"
//==============================================================================
//variables:


//==============================================================================
//functions:

static void PrivateHandler(xTransferLayerT* layer)
{
	int number_of_point = xADC_GetNumberOfNewPoints(&mADC);
	uint8_t* points;

	if (number_of_point)
	{

	}
}
//------------------------------------------------------------------------------
static void PrivateIRQ(xTransferLayerT* layer, void* arg)
{

}
//------------------------------------------------------------------------------
static xResult PrivateRequestListener(xTransferLayerT* layer, xTransferLayerRequestSelector selector, void* arg)
{
	register ADC_TransferLayerAdapterT* adapter = (ADC_TransferLayerAdapterT*)layer->Adapter;
	register xPortT* port = layer->Binding;

	xResult result = xResultAccept;
	uint32_t len;

	switch ((uint32_t)selector)
	{
		case xTransferLayerRequestUpdateRxTxState:
			layer->TxIsBusy = port->Tx.IsTransmitting;
			break;

		case xTransferLayerRequestGetTxBufferSize:
			*(uint32_t*)arg = xPortGetTxBufferSize((xPortT*)layer->Binding);
			break;

		case xTransferLayerRequestGetTxBufferFreeSize:
			*(uint32_t*)arg = xPortGetTxBufferFreeSize((xPortT*)layer->Binding);
			break;

		case xTransferLayerRequestTransferBegin:
			break;

		case xTransferLayerRequestTransferBeginConfirmation:

			break;

		case xTransferLayerRequestTransferConfirmation:
			break;

		case xTransferLayerRequestTransferEnding:
			break;

		case xTransferLayerRequestTransferEndingConfirmation:
			break;

		case xTransferLayerRequestSetBinding:
			layer->Binding = arg;
			break;

		case xTransferLayerRequestResetBinding:
			layer->Binding = 0;
			break;

		default : return xResultRequestIsNotFound;
	}

	return result;
}
//------------------------------------------------------------------------------
static void PrivateEventListener(xTransferLayerT* layer, xPortEventSelector selector, void* arg)
{
	//register TerminalTransferLayerAdapterT* adapter = (TerminalTransferLayerAdapterT*)layer->Adapter;

	switch((int)selector)
	{
		default: return;
	}
}
//------------------------------------------------------------------------------
static int PrivateTransfer(xTransferLayerT* layer, void* data, uint32_t size)
{
	register ADC_TransferLayerAdapterT* adapter = (ADC_TransferLayerAdapterT*)layer->Adapter;

	return xPortTransmitData(layer->Binding, data, size);
}
//==============================================================================
//initializations:

static xTransferLayerInterfaceT PrivateInterface =
{
	.Handler = (xTransferLayerHandlerT)PrivateHandler,
	.IRQ = (xTransferLayerIRQT)PrivateIRQ,

	.RequestListener = (xTransferLayerRequestListenerT)PrivateRequestListener,
	.EventListener = (xTransferLayerEventListenerT)PrivateEventListener,

	.Transfer = (xTransferLayerTransferActionT)PrivateTransfer
};
//------------------------------------------------------------------------------
xResult ADC_TransferLayerAdapterInit(xTransferLayerT* layer, ADC_TransferLayerAdapterInitializationT* initialization)
{
	if (layer && initialization)
	{
		layer->Adapter = (xTransferLayerAdapterBaseT*)initialization->Adapter;

		layer->Adapter->Base.Note = nameof(TerminalTransferLayerAdapterT);
		layer->Adapter->Base.Parent = layer;
		layer->Adapter->Interface = &PrivateInterface;
		
		ADC_TransferLayerAdapterT* adapter = initialization->Adapter;
		
		adapter->ADC = initialization->ADC;

		return xResultAccept;
	}
  
  return xResultError;
}
//==============================================================================
