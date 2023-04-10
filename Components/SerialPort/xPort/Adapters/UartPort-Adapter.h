//==============================================================================
//header:

#ifndef _UART_PORT_ADAPTER_H_
#define _UART_PORT_ADAPTER_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "Common/xPort/xPort.h"
#include "Common/xRxReceiver.h"
#include "Common/xDataBuffer.h"
#include "Registers/registers.h"
//==============================================================================
//types:

typedef struct
{
	xPortAdapterBaseT Base;

	REG_UART_T* Usart;

	DMA_HandleTypeDef* RxDMA;

	xDataBufferT* ResponseBuffer;

	xCircleBufferT RxCircleBuffer;
	xRxReceiverT RxReceiver;

	xCircleBufferT TxCircleBuffer;

} UartPortAdapterT;
//==============================================================================
//functions:

xResult UartPortAdapterInit(xPortT* port, UartPortAdapterT* adapter);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_UART_PORT_ADAPTER_H_
