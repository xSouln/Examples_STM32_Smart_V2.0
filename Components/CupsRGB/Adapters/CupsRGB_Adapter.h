//==============================================================================
#ifndef CUPS_RGB_ADAPTER_H
#define CUPS_RGB_ADAPTER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "CupsRGB_Config.h" // override default macroses
#include "WS2812/WS2812.h"
//==============================================================================
#ifndef CupsRGB_Result
#define CupsRGB_Result int8_t
#endif
//------------------------------------------------------------------------------
#ifndef CupsRGB_ResultAccept
#define CupsRGB_ResultAccept 0
#endif
//------------------------------------------------------------------------------
#ifndef CupsRGB_ResultError
#define CupsRGB_ResultError -1
#endif
//------------------------------------------------------------------------------
#ifndef CupsRGB_ResultBusy
#define CupsRGB_ResultBusy -2
#endif
//------------------------------------------------------------------------------
#ifndef CupsRGB_ResultNullPointer
#define CupsRGB_ResultNullPointer -3
#endif
//------------------------------------------------------------------------------
#ifndef CupsRGB_ResultInvalidParameter
#define CupsRGB_ResultInvalidParameter -4
#endif
//==============================================================================
typedef enum
{
	CupNumber1,
	CupNumber2,
	CupNumber3,
	CupNumber4,
	
} CupNumbers;
//------------------------------------------------------------------------------
typedef enum
{
	CupNumber1Bit = 1 << CupNumber1,
	CupNumber2Bit = 1 << CupNumber2,
	CupNumber3Bit = 1 << CupNumber3,
	CupNumber4Bit = 1 << CupNumber4,
	
} CupNumberBits;
//------------------------------------------------------------------------------
typedef struct
{
	char* Description;
	void* Parent;
	
	WS2812_T Driver1;
	WS2812_T Driver2;
	
} CupsRGB_AdapterT;
//==============================================================================
CupsRGB_Result CupsRGB_AdapterInit(CupsRGB_AdapterT* adapter, void* parent);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* CUPS_RGB_ADAPTER_H */

