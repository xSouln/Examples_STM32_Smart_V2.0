//==============================================================================
//includes:

#include "TCPServer_WIZspiComponent.h"
#include "TCPServer/Adapters/STM32F4xx/WIZspi/Adapters/TCPServer_WIZspiAdapter.h"

#include "Components.h"
//==============================================================================
//defines:

#define OPERATION_BUF_SIZE TCP_SERVER_WIZ_OPERATION_BUF_SIZE
#define RX_BUF_SIZE TCP_SERVER_WIZ_SPI_RX_RECEIVER_BUF_SIZE
#define TX_BUF_SIZE 0x200
//==============================================================================
//variables:

static uint8_t private_operation_buf[OPERATION_BUF_SIZE];
static uint8_t private_rx_buf[RX_BUF_SIZE];
static uint8_t private_tx_buf[TX_BUF_SIZE];

TCPServerT TCPServerWIZspi;
sfc_spi_t* wiz_spi;

#define WIZspi rSPI1
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

	while(!WIZspi->Status.RxNotEmpty){ };
	byte = WIZspi->Data.Byte;

	(void)WIZspi->Status.Value;
	WIZspi->Status.Value = 0;
/*
	sfc_spi_transfer_t transfer = { 0 };
	transfer.rx_data = &byte;
	transfer.data_size = sizeof(byte);
	transfer.timeout = 1000;

	//sfc_spi_transfer(wiz_spi, &transfer);
	sfc_spi_transfer_async(wiz_spi, &transfer);

	while (wiz_spi->state.is_transmitting) { }
*/
	return byte;
}
//------------------------------------------------------------------------------
void WIZspiTransmiteByte(uint8_t byte)
{
	while(!WIZspi->Status.TxEmpty){ };
	WIZspi->Data.Byte = byte;

	while(!WIZspi->Status.RxNotEmpty){ };
	byte = WIZspi->Data.Byte;

	(void)WIZspi->Status.Value;
	WIZspi->Status.Value = 0;
	/*
	sfc_spi_transfer_t transfer = { 0 };
	transfer.tx_data = &byte;
	transfer.data_size = sizeof(byte);
	transfer.timeout = 1000;

	//sfc_spi_transfer(wiz_spi, &transfer);
	sfc_spi_transfer(wiz_spi, &transfer);
	*/
}
//------------------------------------------------------------------------------
void _TCPServerWIZspiComponentEventListener(TCPServerT* server, TCPServerSysEventSelector selector, void* arg)
{
	switch ((uint8_t)selector)
	{
		case TCPServerSysEventEndLine:
			TerminalReceiveData(&server->Port,
								((TCPServerReceivedDataT*)arg)->Data,
								((TCPServerReceivedDataT*)arg)->Size);
			break;
		
		case TCPServerSysEventBufferIsFull:
			TerminalReceiveData(&server->Port,
								((TCPServerReceivedDataT*)arg)->Data,
								((TCPServerReceivedDataT*)arg)->Size);
			break;
	}
}
//------------------------------------------------------------------------------
xResult _TCPServerWIZspiComponentRequestListener(TCPServerT* server, TCPServerSysRequestSelector selector, void* arg)
{
	switch ((uint8_t)selector)
	{
		case TCPServerSysRequestDelay:
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

TCPServerSysInterfaceT TCPServerInterface =
{
	INITIALIZATION_EVENT_LISTENER(TCPServerSys, _TCPServerWIZspiComponentEventListener),
	INITIALIZATION_REQUEST_LISTENER(TCPServerSys, _TCPServerWIZspiComponentRequestListener)
};

//------------------------------------------------------------------------------

TCPServerWIZspiAdapterT TCPServerWIZspiAdapter =
{
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

xResult _TCPServerWIZspiComponentInit(void* parent)
{
	TCPServerWIZspiAdapter.OperationBuffer = private_operation_buf;
	TCPServerWIZspiAdapter.OperationBufferSize = sizeof(private_operation_buf);
	
	WIZspi->Control1.SpiEnable = true;

	//sfc_spi_component_init(parent);
	//wiz_spi = &sfc_spi_wiz;

	//WIZspi->Control1.SpiEnable = true;

	xDataBufferInit(&TCPServerWIZspiAdapter.TxBuffer,
			&TCPServerWIZspi,
			0,
			private_tx_buf,
			sizeof(private_tx_buf));

	xRxReceiverInit(&TCPServerWIZspiAdapter.RxReceiver,
										&TCPServerWIZspi.Port,
										0,
										private_rx_buf,
										sizeof(private_rx_buf));

	TCPServerInit(&TCPServerWIZspi, parent, &TCPServerInterface);
	TCPServerWIZspiAdapterInit(&TCPServerWIZspi, &TCPServerWIZspiAdapter);
	
  return xResultAccept;
}
//==============================================================================
