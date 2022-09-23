//==============================================================================
#ifndef RGB_CUP_TYPES_H
#define RGB_CUP_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "main.h"
#include "WS2812/WS2812.h"
//==============================================================================
#ifndef RGBCupsResult
#define RGBCupsResult int8_t
#endif
//------------------------------------------------------------------------------
#ifndef RGBCupsResultAccept
#define RGBCupsResultAccept 0
#endif
//------------------------------------------------------------------------------
#ifndef RGBCupsResultError
#define RGBCupsResultError (RGBCupsResultAccept + 1)
#endif
//------------------------------------------------------------------------------
#ifndef RGBCupsResultBusy
#define RGBCupsResultBusy (RGBCupsResultError + 1)
#endif
//------------------------------------------------------------------------------
#ifndef RGBCupsResultTimeOut
#define RGBCupsResultTimeOut (RGBCupsResultBusy + 1)
#endif
//------------------------------------------------------------------------------
#ifndef RGBCupsResultNullPointer
#define RGBCupsResultNullPointer (RGBCupsResultTimeOut + 1)
#endif
//------------------------------------------------------------------------------
#ifndef RGBCupsResultInvalidParameter
#define RGBCupsResultInvalidParameter (RGBCupsResultNullPointer + 1)
#endif
//==============================================================================
typedef enum
{
	RGBCupNumber1,
	RGBCupNumber2,
	RGBCupNumber3,
	RGBCupNumber4,
	
	RGBCupsCount
	
} RGBCupsNumbers;
//------------------------------------------------------------------------------
typedef enum
{
	RGBCup1 = 1 << RGBCupNumber1,
	RGBCup2 = 1 << RGBCupNumber2,
	RGBCup3 = 1 << RGBCupNumber3,
	RGBCup4 = 1 << RGBCupNumber4,
	
	RGBCupsAll = 0xff
	
} RGBCupSelector;
//------------------------------------------------------------------------------
typedef enum
{
	RGBCupEventUpdateLayoutStarted,
	RGBCupEventUpdateLayoutComplite,
	
} RGBCupEventSelector;
//==============================================================================
typedef struct
{
	int8_t Diraction;
	
	int16_t Steps;
	int16_t StepNumber;
	
	float GreenIncrement;
	float RedIncrement;
	float BlueIncrement;
	
} RGBCupDrawingRuleT;
//------------------------------------------------------------------------------
typedef union
{
	struct
	{
		uint32_t Free : 3;
	};
	uint32_t Value;
	
} RGBCupStatusT;
//------------------------------------------------------------------------------
typedef void (*RGBCupsEventListener)(void* cup, RGBCupEventSelector event, uint32_t args, uint32_t count);

typedef struct
{
	RGBCupsEventListener EventListener;
	
} RGBCupInterfaceT;
//------------------------------------------------------------------------------
typedef struct
{
	char* Description;
	void* Parent;
	
	void* Adapter;
	
	RGBCupInterfaceT* Interface;
	
	RGBCupStatusT Status;
	WS2812_T Driver;
	
	RGBCupDrawingRuleT DrawingRule;
	
	uint16_t PixelsCount;
	
	uint8_t Id;
	
	uint8_t* DataBuffer;
	uint16_t DataBufferSize;
	
} RGBCupT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* RGB_CUP_TYPES_H */
