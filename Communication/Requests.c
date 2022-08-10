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
//==============================================================================
extern void Device_RequestTransaction(xRxT* rx, xRequestT* request, uint8_t* object, uint16_t size);
extern const xRequestT Requests[];
//==============================================================================
//------------------------------------------------------------------------------
void RequestGetFirmware(xRxT* rx, xRequestT* request, uint8_t* object, uint16_t size)
{
	const char response[] = "firmware qwerty\r";
	
	xTxTransmitData((xTxT*)rx->Tx, (void*)response, SIZE_STRING(response));
}
//------------------------------------------------------------------------------
xRxEndLineResult RxEndLine(xObject rx, uint8_t* obj, uint16_t size)
{
	//casting data to package structure
	PacketT* request = (PacketT*)obj;
	
	//whether the package is a transaction
  if(size >= sizeof(PacketIdentificatorT)
		&& (request->Header.Identificator.Value & PACKET_HEADER_IDENTIFICATOR_MASK) == PACKET_HEADER_IDENTIFICATOR)
  {
		//size check for minimum transaction length
    if (size < sizeof(PacketT)) { return xRxEndLineResultContinue; }
		
    int content_size = (int)size - sizeof(PacketT);
		
		//checking if the package content size matches the actual size, if the size is short
    if(content_size < request->Info.ContentSize) { return xRxEndLineResultContinue; }
		
		//reset size when content exceeds size specified in packet.Info
    if(content_size > request->Info.ContentSize) { return xRxEndLineResultReset; }
  }
	
	xRequestIdentify(rx, (xRequestT*)Requests, obj, size);
  
  return xRxEndLineResultReset;
}
//==============================================================================
//------------------------------------------------------------------------------
const xRequestT Requests[] =
{
  NEW_REQUEST1(DEVICE_REQUEST_HEADER, Device_RequestTransaction),
  { 0 }
};
//==============================================================================
