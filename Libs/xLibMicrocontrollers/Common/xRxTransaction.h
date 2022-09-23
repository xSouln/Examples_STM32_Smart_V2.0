/*
 *
 */
#ifndef X_RX_TRANSACTION_H
#define X_RX_TRANSACTION_H
//==============================================================================
#include "xRxRequest.h"
#include "xRx.h"
#include "xDataBuffer.h"
//==============================================================================
typedef union
{
	struct
	{
		uint32_t StartKey : 8; //default '#'
		uint32_t Description : 16; //description of the purpose of the package - request, response, event, etc.
		uint32_t EndKey : 8; //default ':'
	};
	
	uint32_t Value;
	
} PacketIdentificatorT; //aligned for uint32_t
//------------------------------------------------------------------------------
typedef struct
{
	PacketIdentificatorT Identificator; //identifier for recognizing the package
	uint32_t DeviceKey; //unique key of the device, module. 0 - system commands
	
} PacketHeaderT; //aligned for uint32_t
//------------------------------------------------------------------------------
typedef struct
{
	uint32_t RequestId; //generated key - when receiving a response, must match the request key
	
	uint16_t ActionKey; //action(command) key
	uint16_t ContentSize; //size of nested data after packet info
	
} PacketInfoT; //aligned for uint32_t
//------------------------------------------------------------------------------
typedef struct
{
	PacketHeaderT Header; // format: [#][Description][:][DeviceKey]
	PacketInfoT Info; // format: [RequestId][ActionKey][ContentSize]
	//uint8_t Content[Info.ContentSize]
	//uint8_t EndPacketSymbol default('\r')
	
} PacketT; //array: [#][Description][:][DeviceKey][RequestId][ActionKey][ContentSize][uint8_t Content[ContentSize]][\r]
//==============================================================================
#define PACKET_HEADER_IDENTIFICATOR_MASK 0xFF0000FFU
#define PACKET_HEADER_IDENTIFICATOR 0x2300003AU // format: "#{Description}:"
//------------------------------------------------------------------------------
#define PACKET_HEADER_DESCRIPTION_MASK 0x00FFFF00U // format: "#{Description}:"
#define PACKET_HEADER_DESCRIPTION_REQUEST 0x5251U
#define PACKET_HEADER_DESCRIPTION_RESPONSE 0x5253U
#define PACKET_HEADER_DESCRIPTION_EVENT 0x4554U
//==============================================================================
typedef void (*xRxTransactionAction)(xRxRequestManagerT* manager,
																			xObject request,
																			uint16_t request_size);
//==============================================================================
typedef struct
{
  uint16_t Description;
  uint16_t Id;
  xRxTransactionAction Action;
  
} xRxTransactionT;
//==============================================================================
xRxTransactionT* xRxTransactionIdentify(xRxTransactionT* transaction, uint16_t key);
//==============================================================================
#endif /* X_RX_TRANSACTION_H */
