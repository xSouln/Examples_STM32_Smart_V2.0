//==============================================================================
//includes:

#include "Components.h"
#include "Adapters/UartPort-Adapter.h"
//==============================================================================
//defines:

#define RX_CIRCLE_BUF_SIZE_MASK (UART_PORT_RX_CIRCLE_BUF_SIZE_MASK)
#define RX_OBJECT_BUF_SIZE (UART_PORT_RX_OBJECT_BUF_SIZE)
#define TX_CIRCLE_BUF_SIZE_MASK (UART_PORT_TX_CIRCLE_BUF_SIZE_MASK)
//==============================================================================
//variables:

static uint8_t uart_rx_circle_buf[RX_CIRCLE_BUF_SIZE_MASK + 1];
static uint8_t rx_object_buf[RX_OBJECT_BUF_SIZE];
static uint8_t tx_circle_buf[TX_CIRCLE_BUF_SIZE_MASK + 1];

xPortT UartPort;
//==============================================================================
//import:


//==============================================================================
//functions:

static void EventListener(xPortT* port, xPortSysEventSelector selector, void* arg)
{
	switch((int)selector)
	{
		case xPortSysEventRxFoundEndLine:
			TerminalReceiveData(port,
								((xPortSysEventDataPacketArgT*)arg)->Data,
								((xPortSysEventDataPacketArgT*)arg)->Size);
			break;

		case xPortSysEventRxBufferIsFull:
			TerminalReceiveData(port,
								((xPortSysEventDataPacketArgT*)arg)->Data,
								((xPortSysEventDataPacketArgT*)arg)->Size);
			break;

		default: break;
	}
}
//------------------------------------------------------------------------------

static xResult RequestListener(xPortT* port, xPortSysRequestSelector selector, void* arg)
{
	switch ((uint8_t)selector)
	{
		default: return xResultRequestIsNotFound;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------

void UartPortComponentIRQ()
{
	xPortIRQ(&UartPort, 0);
}
//------------------------------------------------------------------------------
//component functions:
/**
 * @brief main handler
 */
void UartPortComponentHandler()
{
	xPortHandler(&UartPort);
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
void UartPortComponentTimeSynchronization()
{

}

//==============================================================================
//initialization:

static UartPortAdapterT PrivateUartPortAdapter =
{
	.Usart =  (REG_UART_T*)UART_PORT_REG,

	#ifdef UART_PORT_RX_DMA
	.RxDMA = &UART_PORT_RX_DMA,
	#endif
};
//------------------------------------------------------------------------------

static xPortSysInterfaceT PrivatePortSysInterface =
{
	.RequestListener = (xPortSysRequestListenerT)RequestListener,
	.EventListener = (xPortSysEventListenerT)EventListener
};
//==============================================================================
//component initialization:

xResult UartPortComponentInit(void* parent)
{
	xCircleBufferInit(&PrivateUartPortAdapter.RxCircleBuffer, &UartPort, uart_rx_circle_buf, RX_CIRCLE_BUF_SIZE_MASK);
	xCircleBufferInit(&PrivateUartPortAdapter.TxCircleBuffer, &UartPort, tx_circle_buf, TX_CIRCLE_BUF_SIZE_MASK);
	xRxReceiverInit(&PrivateUartPortAdapter.RxReceiver, &UartPort, 0, rx_object_buf, RX_OBJECT_BUF_SIZE);
	
	UartPortAdapterInit(&UartPort, &PrivateUartPortAdapter);
	xPortInit(&UartPort, parent, &PrivatePortSysInterface);
  
	return 0;
}
//==============================================================================

