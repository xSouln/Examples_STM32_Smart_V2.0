//==============================================================================
#ifndef WS2812_H
#define WS2812_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
#include <stdint.h>
//------------------------------------------------------------------------------
#ifdef WS2812_USER_CONFIG
#include "WS2812_UserConfig.h" // override default macroses
#endif
//==============================================================================
typedef enum
{
	#ifndef WS2812_ResultAcceptValue
	WS2812_ResultAccept = 0,
	#else
	WS2812_ResultAccept = WS2812_ResultAcceptValue,
	#endif
	
	#ifndef WS2812_ResultErrorValue
	WS2812_ResultError,
	#else
	WS2812_ResultError = WS2812_ResultErrorValue,
	#endif

	#ifndef WS2812_ResultBusyValue
	WS2812_ResultBusy,
	#else
	WS2812_ResultBusy = WS2812_ResultBusyValue,
	#endif

	#ifndef WS2812_ResultNullPointerValue
	WS2812_ResultNullPointer,
	#else
	WS2812_ResultNullPointer = WS2812_ResultNullPointerValue,
	#endif

	#ifndef WS2812_ResultInvalidParameterValue
	WS2812_ResultInvalidParameter,
	#else
	WS2812_ResultInvalidParameter = WS2812_ResultInvalidParameterValue,
	#endif
	
} WS2812_Results;
//------------------------------------------------------------------------------
#ifndef WS2812_Result
#define WS2812_Result int8_t
#endif
//==============================================================================
typedef enum
{
	Stopped,
	IsTransmit,
	Error
	
} WS2812_TransmitterStatus;
//------------------------------------------------------------------------------
typedef enum
{
	WS2812_StateDisable,
	WS2812_StateEnable,
	
} WS2812_State;
//------------------------------------------------------------------------------
typedef enum
{
	WS2812_Event_TransferComplite,
	
} WS2812_Events;
//------------------------------------------------------------------------------
typedef struct
{
	uint8_t Green;
	uint8_t Red;
	uint8_t Blue;
	
} WS2812_PixelT;
//------------------------------------------------------------------------------
typedef WS2812_Result (*WS2812_ActionWritePixels)(void* driver, uint8_t* data, uint16_t data_size);

typedef uint16_t (*WS2812_ActionGetPeriod)(void* driver);
typedef void (*WS2812_ActionSetPeriod)(void* driver, uint16_t period);

typedef WS2812_TransmitterStatus (*WS2812_ActionGetTransmitterStatus)(void* driver);
typedef WS2812_Result (*WS2812_ActionSetTransmitterState)(void* driver, WS2812_State state);

typedef void (*WS2812_ActionDelay)(void* driver, uint32_t ms);
typedef void (*WS2812_EventHandler)(void* driver, WS2812_Events argument);
//------------------------------------------------------------------------------
typedef struct
{
	void* Handle;
	
	WS2812_ActionWritePixels WritePixels;
	
	WS2812_ActionGetPeriod GetPeriod;
	WS2812_ActionSetPeriod SetPeriod;
	
	WS2812_ActionGetTransmitterStatus GetTransmitterStatus;
	WS2812_ActionSetTransmitterState SetTransmitterState;
	
	WS2812_EventHandler Handler;
	
	WS2812_ActionDelay Delay;
	
} WS2812_ControlT;
//------------------------------------------------------------------------------
typedef struct
{
	uint16_t Period;
	
} WS2812_OptionsT;
//------------------------------------------------------------------------------
typedef union
{
	struct
	{
		uint32_t IsInit : 1;
		uint32_t ErrorState : 3;
		
		uint32_t Transmitter : 3;
	};
	uint32_t Value;
	
} WS2812_StatusT;
//------------------------------------------------------------------------------
typedef union
{
	struct
	{
		uint32_t Free : 3;
	};
	uint32_t Value;
	
} WS2812_HandleT;
//------------------------------------------------------------------------------
typedef struct
{
	char* Description;
	void* Parent;
	
	WS2812_StatusT Status;
	WS2812_HandleT Handle;
	
	WS2812_ControlT* Control;
	WS2812_OptionsT Options;
	
	uint8_t* Buffer;
	uint16_t BufferSize;
	
} WS2812_T;
//==============================================================================
void WS2812_Handler(WS2812_T* driver);
void WS2812_EventReceive(WS2812_T* driver, WS2812_Events event);

WS2812_Result WS2812_WritePixel(WS2812_T* driver, WS2812_PixelT pixel, uint16_t position);
WS2812_Result WS2812_WritePixels(WS2812_T* driver, WS2812_PixelT* pixels, uint16_t pixels_count);

WS2812_Result WS2812_SetOptions(WS2812_T* driver, WS2812_OptionsT* options);

WS2812_Result WS2812_Init(WS2812_T* driver,
													void* parent,
													WS2812_ControlT* control,
													uint8_t* buffer,
													uint16_t buffer_size);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* WS2812_H */
