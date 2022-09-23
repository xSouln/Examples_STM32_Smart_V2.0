//==============================================================================
#include <string.h>
#include "RGBCups/Communications/RGBCups_RxRequests.h"
//==============================================================================
void RGBCups_RequestSetPixels(xRxRequestManagerT* manager, uint8_t* request, uint16_t size)
{
	
}
//------------------------------------------------------------------------------
void RGBCups_RequestSetPixelsState(xRxRequestManagerT* manager,RGBCups_RequestSetPixelsStateT* request)
{
	
}
//------------------------------------------------------------------------------
void RGBCups_RequestGetPixels(xRxRequestManagerT* manager, RGBCups_RequestGetPixelsT* request)
{
	
}
//==============================================================================
extern void RGBCups_ReceiverRequestTransaction(xRxRequestManagerT* manager, uint8_t* object, uint16_t size);
//------------------------------------------------------------------------------
const xRxRequestT RGBCups_RxRequests[] =
{
  NEW_RX_REQUEST1(RGB_CUPS_REQUEST_HEADER, RGBCups_ReceiverRequestTransaction),
  { 0 }
};
//==============================================================================