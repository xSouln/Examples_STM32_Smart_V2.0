//==============================================================================
#include "TCPServer_Components.h"
//==============================================================================
#define RX_BUF_SIZE 0x100
#define RX_RECEIVER_BUF_SIZE 0x200
//------------------------------------------------------------------------------
static uint8_t rx_buf[RX_BUF_SIZE];
static uint8_t rx_receiver_buf[RX_RECEIVER_BUF_SIZE];
//------------------------------------------------------------------------------
extern void RxReceiverEventReceiver(xRxReceiverT* receiver, xRxReceiverEventSelector event, uint32_t args, uint32_t count);
extern xDataBufferT MainDataBuffer;
//------------------------------------------------------------------------------
TCPServerT TCPServerWIZspi;

static TCPServerWIZspi_AdapterT Adapter;
//==============================================================================
void HardwareResetOn()
{
}
//------------------------------------------------------------------------------
void HardwareResetOff()
{
}
//------------------------------------------------------------------------------
uint8_t ReceiveByte()
{
	return 0;
}
//------------------------------------------------------------------------------
void TransmiteByte()
{
	
}
//==============================================================================
static xRxReceiverInterfaceT RxReceiverInterface =
{
	.EventReceiver = (xRxReceiverEventReceiver)RxReceiverEventReceiver
};
//==============================================================================
int TCPServerWIZspi_ComponentInit(void* parent)
{
	Adapter.Server = &TCPServerWIZspi;
	
	Adapter.BusInterface.HardwareResetOn = HardwareResetOn;
	Adapter.BusInterface.HardwareResetOff = HardwareResetOff;
	Adapter.BusInterface.ReceiveByte = ReceiveByte;
	Adapter.BusInterface.TransmiteByte = TransmiteByte;
	
	Adapter.TxDataBuffer = &MainDataBuffer;
	
	Adapter.RxBuffer = rx_buf;
	Adapter.RxBufferSize = sizeof(rx_buf);
	
	xRxReceiverInit(&Adapter.RxReceiver,
									&TCPServerWIZspi.Rx,
									&RxReceiverInterface,
									rx_receiver_buf,
									sizeof(rx_receiver_buf));
	
	TCPServer_Init(&TCPServerWIZspi, parent);
	
	TCPServerWIZspi_AdapterInit(&TCPServerWIZspi, &Adapter);
	
  return 0;
}
//==============================================================================
