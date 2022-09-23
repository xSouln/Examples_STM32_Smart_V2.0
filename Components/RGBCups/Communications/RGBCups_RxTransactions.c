/*
 *
 */
//==============================================================================
#include <string.h>
#include "RGBCups/Communications/RGBCups_RxTransactions.h"
#include "RGBCups/Communications/RGBCups_RxRequests.h"
#include "RGBCups/Adapters/RGBCup_Adapter.h"
#include "RGBCups/Controls/RGBCups.h"
//==============================================================================
#define PACKET_EVENT_IDENTIFICATOR RGB_CUPS_EVENT_IDENTIFICATOR
#define PACKET_RESPONSE_IDENTIFICATOR RGB_CUPS_RESPONSE_IDENTIFICATOR
#define PACKET_DEVICE_KEY RGB_CUPS_PACKET_DEVICE_KEY
#define PACKET_END RGB_CUPS_PACKET_END
//==============================================================================
extern const xRxTransactionT WS2812_RxTransactions[];

#define TRANSACTIONS WS2812_RxTransactions
//==============================================================================
int RGBCups_TransmitEvent(xTxT* tx, RGB_CUPS_TRANSACTIONS transaction, xObject data, uint16_t data_size)
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
	xTxDeclareRequest(tx, xTxRequestStartTransmission, 0, 0);
	
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
	xTxDeclareRequest(tx, xTxRequestStopTransmission, 0, 0);
	
	//generate event
	xTxDeclareEvent(tx, xTxEventTransmissionComplete, 0, 0);
	
	return 0;
}
//------------------------------------------------------------------------------
void RGBCups_ReceiverRequestTransaction(xRxRequestManagerT* manager, uint8_t* object, uint16_t size)
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
  xRxTransactionT* result = xRxTransactionIdentify((xRxTransactionT*)TRANSACTIONS, info->ActionKey);
  
  if(result)
  {
		xTxT* tx = manager->RxLine->Tx;
		
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
				xTxDeclareRequest(tx, xTxRequestStartTransmission, 0, 0);
				
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
				xTxDeclareRequest(tx, xTxRequestStopTransmission, 0, 0);
				
				//generate event
				xTxDeclareEvent(tx, xTxEventTransmissionComplete, 0, 0);
			}
    }
  }
}
//==============================================================================
const xRxTransactionT WS2812_RxTransactions[] =
{
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	//GET
		
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	//SET
		
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	//TRY
	
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	// end of transactions marker
  { .Id = -1 }
};
//==============================================================================
