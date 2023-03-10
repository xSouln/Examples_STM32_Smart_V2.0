//==============================================================================
//includes:

#include "TCPServer_WIZspiComponent.h"
#include "Adapter/TCPServer_WIZspiAdapter.h"

#include "Components.h"
//==============================================================================
//defines:

#define RX_BUF_SIZE TCP_SERVER_WIZ_SPI_RX_BUF_SIZE
#define RX_RECEIVER_BUF_SIZE TCP_SERVER_WIZ_SPI_RX_RECEIVER_BUF_SIZE
//==============================================================================
//variables:

static uint8_t rx_buf[RX_BUF_SIZE];
static uint8_t rx_receiver_buf[RX_RECEIVER_BUF_SIZE];

TCPServerT TCPServerWIZspi;
REG_SPI_T* WIZspi;
//==============================================================================
//functions:

void WIZspiHardwareResetOn()
{
	WIZ_RESET_GPIO_Port->ODR &= ~WIZ_RESET_Pin;
}
//------------------------------------------------------------------------------
void WIZspiHardwareResetOff()
{
	WIZ_RESET_GPIO_Port->ODR |= WIZ_RESET_Pin;
}
//------------------------------------------------------------------------------
void WIZspiSelectChip()
{
	WIZ_CS_GPIO_Port->ODR &= ~WIZ_CS_Pin;
}
//------------------------------------------------------------------------------
void WIZspiDeselectChip()
{
	WIZ_CS_GPIO_Port->ODR |= WIZ_CS_Pin;
}
//------------------------------------------------------------------------------
uint8_t WIZspiReceiveByte()
{
	uint8_t byte = 0xff;

	while(!WIZspi->Status.TxEmpty){ };
	WIZspi->Data.Byte = byte;

	while(WIZspi->Status.Busy){ };

	while(!WIZspi->Status.RxNotEmpty){ };
	byte = WIZspi->Data.Byte;

	return byte;
}
//------------------------------------------------------------------------------
void WIZspiTransmiteByte(uint8_t byte)
{
	while(!WIZspi->Status.TxEmpty){ };
	WIZspi->Data.Byte = byte;

	while(WIZspi->Status.Busy){ };

	while(!WIZspi->Status.RxNotEmpty){ };
	byte = WIZspi->Data.Byte;
}
//------------------------------------------------------------------------------
void _TCPServerWIZspiComponentEventListener(TCPServerT* server, TCPServerEventSelector selector, void* arg, ...)
{
	switch ((uint8_t)selector)
	{
		case TCPServerEventEndLine:
			TerminalReceiveData(&server->Rx,
								((SerialPortReceivedDataT*)arg)->Data,
								((SerialPortReceivedDataT*)arg)->Size);
			break;
		
		case TCPServerEventBufferIsFull:
			TerminalReceiveData(&server->Rx,
								((SerialPortReceivedDataT*)arg)->Data,
								((SerialPortReceivedDataT*)arg)->Size);
			break;
	}
}
//------------------------------------------------------------------------------
xResult _TCPServerWIZspiComponentRequestListener(TCPServerT* server, TCPServerRequestSelector selector, void* arg, ...)
{
	switch ((uint8_t)selector)
	{
		case TCPServerRequestDelay:
			HAL_Delay((uint32_t)arg);
			break;
		
		default : return xResultRequestIsNotFound;
	}
	
	return xResultAccept;
}
//------------------------------------------------------------------------------

void _TCPServerWIZspiComponentIRQListener(TCPServerT* server, ...)
{

}
//------------------------------------------------------------------------------
/**
 * @brief main handler
 */
inline void _TCPServerWIZspiComponentHandler()
{

}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
inline void _TCPServerWIZspiComponentTimeSynchronization()
{

}
//==============================================================================
//initializations:

TCPServerInterfaceT TCPServerInterface =
{
	INITIALIZATION_EVENT_LISTENER(TCPServer, _TCPServerWIZspiComponentEventListener),
	INITIALIZATION_REQUEST_LISTENER(TCPServer, _TCPServerWIZspiComponentRequestListener)
};

//------------------------------------------------------------------------------

TCPServerWIZspiAdapterT TCPServerWIZspiAdapter =
{
	.SPI = TCP_SERVER_WIZ_SPI_REG,

	.BusInterface =
	{
		.SelectChip = WIZspiSelectChip,
		.DeselectChip = WIZspiDeselectChip,

		.HardwareResetOn = WIZspiHardwareResetOn,
		.HardwareResetOff = WIZspiHardwareResetOff,

		.TransmiteByte = WIZspiTransmiteByte,
		.ReceiveByte = WIZspiReceiveByte
	},
};
//==============================================================================
//initialization:

xResult TCPServerWIZspiComponentInit(void* parent)
{
	TCPServerWIZspiAdapter.ResponseBuffer = &Terminal.ResponseBuffer;
	
	TCPServerWIZspiAdapter.RxBuffer = rx_buf;
	TCPServerWIZspiAdapter.RxBufferSize = sizeof(rx_buf);
	
	WIZspi = TCP_SERVER_WIZ_SPI_REG;
	WIZspi->Control1.SpiEnable = true;

	xRxReceiverInit(&TCPServerWIZspiAdapter.RxReceiver,
										&TCPServerWIZspi.Rx,
										0,
										rx_receiver_buf,
										sizeof(rx_receiver_buf));

	TCPServerInit(&TCPServerWIZspi, parent, &TCPServerInterface);
	TCPServerWIZspiAdapterInit(&TCPServerWIZspi, &TCPServerWIZspiAdapter);
	
  return xResultAccept;
}
//==============================================================================
