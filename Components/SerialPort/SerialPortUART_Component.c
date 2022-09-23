//==============================================================================
#include "SerialPort_Components.h"
//==============================================================================
#define RX_CIRCLE_BUF_SIZE_MASK 0x1ff
#define RX_OBJECT_BUF_SIZE 0x200
#define TX_CIRCLE_BUF_SIZE_MASK 0x3ff
//------------------------------------------------------------------------------
static uint8_t rx_circle_buf[RX_CIRCLE_BUF_SIZE_MASK + 1];
static uint8_t rx_object_buf[RX_OBJECT_BUF_SIZE];
static uint8_t tx_circle_buf[TX_CIRCLE_BUF_SIZE_MASK + 1];
//------------------------------------------------------------------------------
extern void RxReceiverEventReceiver(xRxReceiverT* receiver, xRxReceiverEventSelector event, uint32_t args, uint32_t count);
extern xDataBufferT MainDataBuffer;

SerialPortT SerialPortUART;
//==============================================================================
static xRxReceiverInterfaceT RxReceiverInterface =
{
	.EventReceiver = (xRxReceiverEventReceiver)RxReceiverEventReceiver
};
//------------------------------------------------------------------------------
static SerialPortUART_AdapterT Adapter =
{
	.RxDMA = DMA1_Channel5,
	.Usart = (volatile STM32_UART_REG_T*)USART1
};
//==============================================================================
int SerialPortUART_ComponentInit(void* parent)
{
	Adapter.TxDataBuffer = &MainDataBuffer;
	
	xCircleBufferInit(&Adapter.RxCircleBuffer,
										&SerialPortUART.Rx,
										rx_circle_buf,
										RX_CIRCLE_BUF_SIZE_MASK);
	
	xCircleBufferInit(&Adapter.TxCircleBuffer,
										&SerialPortUART.Tx,
										tx_circle_buf,
										TX_CIRCLE_BUF_SIZE_MASK);
	
	xRxReceiverInit(&Adapter.RxReceiver,
									&SerialPortUART.Rx,
									&RxReceiverInterface,
									rx_object_buf,
									RX_OBJECT_BUF_SIZE);
	
	SerialPort_Init(&SerialPortUART, parent);
	SerialPortUART_AdapterInit(&SerialPortUART, &Adapter);
  
  return 0;
}
//==============================================================================
