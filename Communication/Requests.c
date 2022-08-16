/*
 * Requests.c
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */
//==============================================================================
#include <string.h>
#include "Requests.h"
#include "Transactions.h"
#include "Control.h"
//==============================================================================
void RequestGetFirmware(xRxT* rx, xRequestT* request, uint8_t* object, uint16_t size)
{
	const char response[] = "firmware: qwerty\r";
	
	if (rx->Tx)
	{
		xTxTransmitData(rx->Tx, (void*)response, SIZE_STRING(response));
	}
}
//==============================================================================
xRxEndLineResult RxEndLine(xObject rx, uint8_t* object, uint16_t size)
{
	//if the received data is a transaction:
	//Packet header: [#][Description][:][DeviceKey];
	//Packet info: [RequestId][ActionKey][ContentSize]
	//Packet content: [uint8_t Content[info.ContentSize]]
	//Packet end packet marker: [\r]; - not included in "uint16_t size"
	
	//casting data to package structure
	PacketT* request = (PacketT*)object;
	
	//whether the package is a transaction
  if(size >= sizeof(PacketIdentificatorT)
		&& (request->Header.Identificator.Value & PACKET_HEADER_IDENTIFICATOR_MASK) == PACKET_HEADER_IDENTIFICATOR)
  {
		//size check for minimum transaction length. size - does not include the length of the separator character - '\r'
    if (size < sizeof(PacketT)) { return xRxEndLineResultContinue; }
		
		//content size calculation
    int content_size = (int)size - sizeof(PacketT);
		
		//checking if the package content size matches the actual size, if the size is short
    if(content_size < request->Info.ContentSize) { return xRxEndLineResultContinue; }
		
		//reset size when content exceeds size specified in packet.Info
    if(content_size > request->Info.ContentSize) { return xRxEndLineResultReset; }
  }
	
	//command identification
	xRequestIdentify(rx, &Control.RequestManager, object, size);
  
  return xRxEndLineResultReset;
}
//==============================================================================
extern void Device_RequestTransaction(xRxT* rx, xRequestT* request, uint8_t* object, uint16_t size);
//------------------------------------------------------------------------------
const xRequestT Requests[] =
{
  NEW_REQUEST1(DEVICE_REQUEST_HEADER, Device_RequestTransaction),
	NEW_REQUEST0("get fitmware", RequestGetFirmware),
  { 0 }
};
//==============================================================================
