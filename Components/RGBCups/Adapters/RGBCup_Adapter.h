//==============================================================================
#ifndef RGB_CUP_ADAPTER_H
#define RGB_CUP_ADAPTER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "RGBCup_AdapterTypes.h"
#include "RGBCup_AdapterInfo.h"
#include "RGBCup_AdapterConfig.h"
#include "RGBCups/RGBCups_Types.h"
//==============================================================================
RGBCupsResult RGBCup_AdapterInit(RGBCupT* cup, RGBCupAdapterT* adapter);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* RGB_CUP_ADAPTER_H */

