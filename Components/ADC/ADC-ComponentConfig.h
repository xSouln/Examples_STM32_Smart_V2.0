//==============================================================================
#ifndef _ADC_COMPONENT_CONFIG_H_
#define _ADC_COMPONENT_CONFIG_H_
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "Components-Types.h"

//==============================================================================
//import:

//extern DMA_HandleTypeDef hdma_usart1_rx;
//==============================================================================
//defines:

#define ADC_NUMBER_OF_CHANNELS 3
#define ADC_POINTS_PER_CHANNEL 256
#define ADC_POINTS_IN_PACKET 64
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_ADC_COMPONENT_CONFIG_H_
