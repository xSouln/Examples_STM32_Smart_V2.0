//==============================================================================
#include "SerialPortUSB_TxAdapter.h"
//==============================================================================
static int TransmitData(xTxT *tx, xObject data, uint32_t data_size)
{
	SerialPortUSB_AdapterT* adapter = tx->Adapter;
	
	return xCircleBufferAdd(&adapter->TxCircleBuffer, data, data_size);
}
//==============================================================================
static void EventListener(xTxT *tx, xTxEventSelector event, uint32_t args, uint32_t count)
{
	SerialPortUSB_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)event)
	{		
		default : break;
	}
}
//------------------------------------------------------------------------------
static xResult RequestListener(xTxT* tx, xTxRequestSelector selector, uint32_t args, uint32_t count)
{
	SerialPortUSB_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)selector)
	{
		case xTxRequestTransmitData : return xCircleBufferAdd(&adapter->TxCircleBuffer, (uint8_t*)args, count);
		case xTxRequestEnableTransmitter: break;
			
		default : return xResultRequestIsNotFound;
	}
	
	return xResultRequestIsNotFound;
}
//------------------------------------------------------------------------------
static int GetValue(xTxT* tx, xTxValueSelector selector)
{
	SerialPortUSB_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)selector)
	{
		case xTxValueTransmitterStatus :
			return adapter->Handle->TxState == 0 ? xTxStatusFree : xTxStatusIsTransmits;
		
		default : return -1;
	}
}
//------------------------------------------------------------------------------
static xResult SetValue(xTxT* tx, xTxValueSelector selector, uint32_t value)
{
	SerialPortUSB_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)selector)
	{
		default : return xResultValueIsNotFound;
	}
	
	return xResultValueIsNotFound;
}
//------------------------------------------------------------------------------
static void Handler(xTxT *tx)
{
	SerialPortUSB_AdapterT* adapter = tx->Adapter;
	uint16_t data_size;
	
	if (adapter->TxCircleBuffer.HandlerIndex != adapter->TxCircleBuffer.TotalIndex
		&& adapter->Handle->TxState == 0)
	{
		data_size = (adapter->TxCircleBuffer.TotalIndex - adapter->TxCircleBuffer.HandlerIndex)
								& adapter->TxCircleBuffer.SizeMask;
		
		adapter->Handle->TxState = xTxStatusIsTransmits;
		
		HAL_PCD_EP_Transmit(adapter->hUsbDeviceFS->pData,
												CDC_IN_EP,
												adapter->TxCircleBuffer.Buffer + adapter->TxCircleBuffer.HandlerIndex,
												data_size);
		
		adapter->TxCircleBuffer.HandlerIndex += data_size;
		adapter->TxCircleBuffer.HandlerIndex &= adapter->TxCircleBuffer.SizeMask;
	}
}
//------------------------------------------------------------------------------
static xTxInterfaceT interface =
{
	.Handler = (xTxActionHandler)Handler,
	.EventListener = (xTxEventListener)EventListener,
	.RequestListener = (xTxRequestListener)RequestListener,
	.GetValue = (xTxActionGetValue)GetValue,
	.SetValue = (xTxActionSetValue)SetValue
};
//==============================================================================
int SerialPortUSB_TxAdapterInit(xTxT* tx, void* parent, SerialPortUSB_AdapterT* adapter)
{
	if (tx && adapter)
	{
		return xTxInit(tx, parent, adapter, &interface, adapter->TxDataBuffer);
	}
	
	return SerialPortAdapterResultError;
}
//==============================================================================
