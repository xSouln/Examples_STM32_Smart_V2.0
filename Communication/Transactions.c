/*
 * Transactions.c
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */
//==============================================================================
#include <string.h>
#include "Transactions.h"
#include "Common/xTx.h"
#include "Common/xRx.h"
#include "Requests.h"
//==============================================================================
#define PACKET_EVENT_IDENTIFICATOR DEVICE_EVENT_IDENTIFICATOR
#define PACKET_RESPONSE_IDENTIFICATOR DEVICE_RESPONSE_IDENTIFICATOR
#define PACKET_DEVICE_KEY PACKET_DEVICE_KEY
#define PACKET_END PACKET_END
//==============================================================================
extern const xTransactionT Transactions[];

#define TRANSACTIONS Transactions
//==============================================================================
void TransactionGetTime(xRequestManagerT* manager)
{
	extern uint32_t time_ms;
	
	xDataBufferAdd(manager->Rx->Tx->ObjectBuffer, &time_ms, sizeof(time_ms));
}
//------------------------------------------------------------------------------
void TransactionTryResetTime(xRequestManagerT* manager)
{
	extern uint32_t time_ms;
	int16_t result = RESULT_ACCEPT;
	
	time_ms = 0;
	
	xDataBufferAdd(manager->Rx->Tx->ObjectBuffer, &result, sizeof(result));
	xDataBufferAdd(manager->Rx->Tx->ObjectBuffer, &time_ms, sizeof(time_ms));
}
//------------------------------------------------------------------------------
void TransactionSetTime(xRequestManagerT* manager, int* request, uint16_t request_size)
{
	extern uint32_t time_ms;
	int16_t result = RESULT_ACCEPT;
	
	time_ms = *request;
	
	xDataBufferAdd(manager->Rx->Tx->ObjectBuffer, &result, sizeof(result));
	xDataBufferAdd(manager->Rx->Tx->ObjectBuffer, &time_ms, sizeof(time_ms));
}
//==============================================================================
int Device_TransmitEvent(xTxT* tx, DEVICE_TRANSACTIONS transaction, xObject data, uint16_t data_size)
{
	//event array:
	//Header: [#][Description][:][DeviceKey];
	//Info: [RequestId][ActionKey][ContentSize]
	//Content: [uint8_t Content[ContentSize]]
	//End packet marker: [\r]
	
	//filling in the package information
	PacketInfoT packet_info =
	{
		.ActionKey = transaction,
		.ContentSize = data_size
	};
	
	//start transmission logic implementation selected "tx" line
	xTxStartTransmission(tx);
	
	//Packet header start
	xTxTransmitData(tx, &PACKET_EVENT_IDENTIFICATOR, sizeof(PACKET_EVENT_IDENTIFICATOR));
	xTxTransmitData(tx, &PACKET_DEVICE_KEY, sizeof(PACKET_DEVICE_KEY));
	//Packet header end
	
	//Packet info: command "id" and "content_size"
	xTxTransmitData(tx, &packet_info, sizeof(packet_info));
	
	//sending content
	xTxTransmitData(tx, data, data_size);
	
	//Packet end
	xTxTransmitData(tx, PACKET_END, SIZE_STRING(PACKET_END));
	
	//transmission logic implementation selected "tx" line
	xTxEndTransmission(tx);
	
	return 0;
}
//------------------------------------------------------------------------------
void Device_RequestTransaction(xRequestManagerT* manager, uint8_t* object, uint16_t size)
{
	//request array:
	//Packet header: [#][Description][:][DeviceKey]; - the "uint8_t*
	//object" is offset by the length of the header because the function is called
	//when the header "PacketHeaderT" matches
	//Packet info: [RequestId][ActionKey][ContentSize]
	//Packet content: [uint8_t Content[ContentSize]]
	//Packet end packet marker: [\r]; - not included in "uint16_t size"
	
	//received packet info
	PacketInfoT* info = (PacketInfoT*)object;
	
	//finde result by ActionKey
  xTransactionT* result = xTransactionIdentify((xTransactionT*)TRANSACTIONS, info->ActionKey);
  
  if(result)
  {
		xTxT* tx = manager->Rx->Tx;
		
		//receiving the transmitter corresponding to the selected receiver
		PacketInfoT packet_info;
    
    object += sizeof(PacketInfoT);
    size -= sizeof(PacketInfoT);
    
    if(result->Action)
    {
			//clearing the data buffer
			if (tx)
			{
				xDataBufferClear(tx->ObjectBuffer);
			}
			
			//function call corresponding to the request ActionKey
      result->Action(manager, object, size);
			
			if (tx)
			{
				//filling in the package information structure
				packet_info.RequestId = info->RequestId; //must match the request RequestId
				packet_info.ActionKey = result->Id;
				//setting content size according to added in call function "result->Action"
				if (tx->ObjectBuffer)
				{
					packet_info.ContentSize = tx->ObjectBuffer->DataSize;
				}
				
				//response array:
				//Packet header: [#][Description][:][DeviceKey]
				//Packet info: [RequestId][ActionKey][ContentSize]
				//Packet content: [uint8_t Content[ContentSize]]
				//Packet end packet marker: [\r]
				
				//start transmission logic implementation selected "tx" line
				xTxStartTransmission(tx);
				
				//Packet header start
				xTxTransmitData(tx, &PACKET_RESPONSE_IDENTIFICATOR, sizeof(PACKET_RESPONSE_IDENTIFICATOR));
				xTxTransmitData(tx, &PACKET_DEVICE_KEY, sizeof(PACKET_DEVICE_KEY));
				//Packet header end
				
				//Packet info: unique request key: "RequestId", command key: "ActionKey" and "content_size"
				xTxTransmitData(tx, &packet_info, sizeof(packet_info));
				
				//Packet content:
				if (tx->ObjectBuffer)
				{
					xTxTransmitData(tx,
					tx->ObjectBuffer->Data,
					tx->ObjectBuffer->DataSize);
				}
				
				//Packet end
				xTxTransmitData(tx, PACKET_END, SIZE_STRING(PACKET_END));
				
				//transmission logic implementation selected "tx" line
				xTxEndTransmission(tx);
			}
    }
  }
}
//==============================================================================
const xTransactionT Transactions[] =
{
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	//GET
  {
		.Id = TRANSACTION_GET_FIRMWARE_VERSION,
	},
	//----------------------------------------------------------------------------
	{
		.Id = TRANSACTION_GET_TIME,
		.Action = (xTransactionAction)TransactionGetTime,
	},
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	//SET
	{
		.Id = TRANSACTION_SET_TIME,
		.Action = (xTransactionAction)TransactionSetTime,
	},
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	//TRY
	{
		.Id = TRANSACTION_TRY_RESET_TIME,
		.Action = (xTransactionAction)TransactionTryResetTime,
	},
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	// end of transactions marker
  { .Id = -1 }
};
//==============================================================================
