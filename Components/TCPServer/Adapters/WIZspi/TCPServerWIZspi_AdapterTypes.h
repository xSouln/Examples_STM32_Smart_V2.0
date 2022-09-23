//==============================================================================
#ifndef TCP_SERVER_WIZ_SPI_ADAPTER_TYPES_H
#define TCP_SERVER_WIZ_SPI_ADAPTER_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "TCPServer/Adapters/TCPServer_AdaptersTypes.h"
#include "TCPServerWIZspi_AdapterInfo.h"
#include "TCPServerWIZspi_AdapterConfig.h"
//==============================================================================
typedef struct
{
	void (*HardwareResetOn)();
	void (*HardwareResetOff)();
	
	void (*TransmiteByte)();
	uint8_t (*ReceiveByte)();
	  
} TCPServerWIZspi_BusInterfaceT;
//==============================================================================
typedef struct
{
	void* Server;
	
	xDataBufferT* TxDataBuffer;
	TCPServerWIZspi_BusInterfaceT BusInterface;
	
	xRxReceiverT RxReceiver;
	
	uint8_t* RxBuffer;
	uint16_t RxBufferSize;
	  
} TCPServerWIZspi_AdapterT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* TCP_SERVER_WIZ_SPI_ADAPTER_TYPES_H */