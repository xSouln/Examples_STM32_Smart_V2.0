//==============================================================================
#ifndef CUPS_RGB_REQUESTS_H
#define CUPS_RGB_REQUESTS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
#include "Common/xRequest.h"
#include "Common/xRx.h"
#include "CupsRGB/Adapters/CupsRGB_Adapter.h"
#include "CupsRGB_Transactions.h"
//==============================================================================
typedef struct
{
	CupNumberBits Cups;
	uint16_t StartPixelIndex;
	uint16_t PixelsCount;
	//WS2812_PixelT Pixels[PixelsCount];
	
} CupsRGB_RequestSetPixelsT;
//------------------------------------------------------------------------------
typedef struct
{
	CupNumberBits Cups;
	WS2812_PixelT Pixel;
	
} CupsRGB_RequestSetPixelsStateT;
//------------------------------------------------------------------------------
typedef CupNumbers CupsRGB_RequestGetPixelsT;
//==============================================================================
extern const xRequestT CupsRGB_Requests[];
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* CUPS_RGB_REQUESTS_H */

