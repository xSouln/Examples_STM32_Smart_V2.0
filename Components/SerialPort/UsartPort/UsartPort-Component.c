//==============================================================================
//includes:

#include "Components.h"
#include "Adapters/UsartPort-Adapter.h"
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

xPortT UsartPort;
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

void UsartPortComponentIRQ()
{
	xPortIRQ(&UsartPort, 0);
}
//------------------------------------------------------------------------------
//component functions:
/**
 * @brief main handler
 */
void UsartPortComponentHandler()
{
	xPortHandler(&UsartPort);
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
void UsartPortComponentTimeSynchronization()
{

}

//==============================================================================
//initialization:

static UsartPortAdapterT PrivateUsartPortAdapter =
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

xResult UsartPortComponentInit(void* parent)
{
	xCircleBufferInit(&PrivateUsartPortAdapter.RxCircleBuffer, uart_rx_circle_buf, RX_CIRCLE_BUF_SIZE_MASK, sizeof(uint8_t));
	xCircleBufferInit(&PrivateUsartPortAdapter.TxCircleBuffer, tx_circle_buf, TX_CIRCLE_BUF_SIZE_MASK, sizeof(uint8_t));
	xRxReceiverInit(&PrivateUsartPortAdapter.RxReceiver, &UsartPort, 0, rx_object_buf, RX_OBJECT_BUF_SIZE);
	
	UsartPortAdapterInit(&UsartPort, &PrivateUsartPortAdapter);
	xPortInit(&UsartPort, parent, &PrivatePortSysInterface);
  
	return 0;
}
//==============================================================================

