//==============================================================================
#include <string.h>
#include "CupsRGB/Communications/CupsRGB_Requests.h"
//==============================================================================
void CupsRGB_RequestSetPixels(xRxT* rx, void* object, uint8_t* request, uint16_t size)
{
	
}
//------------------------------------------------------------------------------
void CupsRGB_RequestSetPixelsState(xRxT* rx, void* object, CupsRGB_RequestSetPixelsStateT* request)
{
	
}
//------------------------------------------------------------------------------
void CupsRGB_RequestGetPixels(xRxT* rx, void* object, CupsRGB_RequestGetPixelsT* request)
{
	
}
//==============================================================================
extern void CupsRGB_RequestTransaction(xRxT* rx, xRequestT* request, uint8_t* object, uint16_t size);
//------------------------------------------------------------------------------
const xRequestT CupsRGB_Requests[] =
{
  NEW_REQUEST1(CUPS_RGB_REQUEST_HEADER, CupsRGB_RequestTransaction),
  { 0 }
};
//==============================================================================