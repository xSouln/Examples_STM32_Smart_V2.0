//==============================================================================
#ifndef RGB_CUP_ADAPTER_TYPES_H
#define RGB_CUP_ADAPTER_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
//==============================================================================
#include "RGBCups/RGBCups_Types.h"
//==============================================================================
typedef struct
{
	volatile DMA_Channel_TypeDef* DMA;
	volatile STM32_TIM_REG_T* Timer;
	
	uint8_t* DrawMemory;
	uint32_t DrawMemorySize;
	
	uint8_t PWM_Channel;
	
} RGBCupAdapterT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* RGB_CUP_ADAPTER_TYPES_H */