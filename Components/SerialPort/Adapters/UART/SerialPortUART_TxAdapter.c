//==============================================================================
#include "SerialPortUART_TxAdapter.h"
//==============================================================================
static void Handler(xTxT *tx)
{
	SerialPortUART_AdapterT* adapter = tx->Adapter;
	
	if (!adapter->Usart->Control_1.TxEmptyInterruptEnable
		&& adapter->TxCircleBuffer.TotalIndex != adapter->TxCircleBuffer.HandlerIndex)
	{
		adapter->Usart->Control_1.TxEmptyInterruptEnable = true;
	}
}
//------------------------------------------------------------------------------
static void EventListener(xTxT *tx, xTxEventSelector event, uint32_t args, uint32_t count)
{
	SerialPortUART_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)event)
	{
		case xTxEventIRQ :
		{
			if (adapter->Usart->Control_1.TxEmptyInterruptEnable && adapter->Usart->Status.TxEmpty)
			{
				if (adapter->TxCircleBuffer.HandlerIndex != adapter->TxCircleBuffer.TotalIndex)
				{
					adapter->Usart->Data = xCircleBufferGet(&adapter->TxCircleBuffer);
				}
				else
				{
					adapter->Usart->Control_1.TxEmptyInterruptEnable = false;
				}
			}
		}
		break;
		
		default : break;
	}
}
//------------------------------------------------------------------------------
static xResult RequestListener(xTxT* tx, xTxRequestSelector selector, uint32_t args, uint32_t count)
{
	SerialPortUART_AdapterT* adapter = tx->Adapter;
	
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
	SerialPortUART_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)selector)
	{
		case xTxValueTransmitterStatus :
			return adapter->Usart->Control_1.TxEmptyInterruptEnable ? xTxStatusIsTransmits : xTxStatusFree;
		
		default : return -1;
	}
}
//------------------------------------------------------------------------------
static xResult SetValue(xTxT* tx, xTxValueSelector selector, uint32_t value)
{
	SerialPortUART_AdapterT* adapter = tx->Adapter;
	
	switch ((uint32_t)selector)
	{
		default : return xResultValueIsNotFound;
	}
	
	return xResultValueIsNotFound;
}
//------------------------------------------------------------------------------
static int TransmitData(xTxT *tx, xObject data, uint32_t data_size)
{
	SerialPortUART_AdapterT* adapter = tx->Adapter;
	
	return xCircleBufferAdd(&adapter->TxCircleBuffer, data, data_size);
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
int SerialPortUART_TxAdapterInit(xTxT* tx, void* parent, SerialPortUART_AdapterT* adapter)
{
	if (tx && adapter)
	{
		adapter->Usart->Control_1.TransmitterEnable = true;
		adapter->Usart->Control_1.TxCompleteInterruptEnable = false;
		adapter->Usart->Control_1.TxEmptyInterruptEnable = false;
		
		return xTxInit(tx, parent, adapter, &interface, adapter->TxDataBuffer);
	}
	return SerialPortAdapterResultError;
}
//==============================================================================
