//==============================================================================
#include "SerialPortUART_RxAdapter.h"
//==============================================================================
static void Handler(xRxT* rx)
{
	SerialPortUART_AdapterT* adapter = rx->Adapter;
	
	adapter->RxCircleBuffer.TotalIndex = (adapter->RxCircleBuffer.SizeMask + 1) - adapter->RxDMA->CNDTR;
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
		default : return xResultRequestIsNotFound;
	}
	
	return xResultRequestIsNotFound;
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
int SerialPortUART_RxAdapterInit(xRxT* rx, void* parent, SerialPortUART_AdapterT* adapter)
{
	if (rx && adapter)
	{
		xRxInit(rx, parent, adapter, &interface);
		
		adapter->Usart->Control_1.ReceiverEnable = true;
		adapter->Usart->Control_3.DMA_EnableReceiver = true;
		
		if (adapter->RxDMA)
		{
			adapter->RxDMA->CCR &= ~DMA_CCR_EN;
			adapter->RxDMA->CCR |= DMA_CCR_CIRC;
			adapter->RxDMA->CCR |= DMA_CCR_MINC;
			adapter->RxDMA->CCR |= DMA_CCR_PL_0;
			
			adapter->RxDMA->CNDTR = adapter->RxCircleBuffer.SizeMask + 1;
			adapter->RxDMA->CPAR = (uint32_t)&adapter->Usart->Data;
			adapter->RxDMA->CMAR = (uint32_t)adapter->RxCircleBuffer.Buffer;
			
			adapter->RxDMA->CCR |= DMA_CCR_EN;
		}
		
		return SerialPortAdapterResultAccept;
	}
	
	return SerialPortAdapterResultError;
}
//==============================================================================
