//==============================================================================
#ifndef RGB_CUPS_RX_REQUESTS_H
#define RGB_CUPS_RX_REQUESTS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
#include "Common/xRxRequest.h"
#include "RGBCups/RGBCups_Types.h"
#include "RGBCups_RxTransactions.h"
#include "WS2812/WS2812.h"
//==============================================================================
typedef struct
{
	RGBCupsNumbers Cups;
	uint16_t StartPixelIndex;
	uint16_t PixelsCount;
	//WS2812_PixelT Pixels[PixelsCount];
	
} RGBCups_RequestSetPixelsT;
//------------------------------------------------------------------------------
typedef struct
{
	RGBCupSelector Cups;
	WS2812_PixelT Pixel;
	
} RGBCups_RequestSetPixelsStateT;
//------------------------------------------------------------------------------
typedef RGBCupsNumbers RGBCups_RequestGetPixelsT;
//==============================================================================
extern const xRxRequestT RGBCups_RxRequests[];
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* RGB_CUPS_RX_REQUESTS_H */

