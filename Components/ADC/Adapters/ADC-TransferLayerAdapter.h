//==============================================================================
//header:

#ifndef _ADC_TARNSFER_LAYER_ADAPTER_H_
#define _ADC_TARNSFER_LAYER_ADAPTER_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "Components.h"

#include "Common/xTransferLayer/xTransferLayer.h"
#include "Common/xPort/xPort.h"
#include "Common/xRxRequest.h"
//==============================================================================
//types:

typedef struct
{
	xTransferLayerAdapterBaseT Base;

	xADC_T* ADC;

} ADC_TransferLayerAdapterT;
//------------------------------------------------------------------------------
typedef struct
{
	ADC_TransferLayerAdapterT* Adapter;

	xADC_T* ADC;

} ADC_TransferLayerAdapterInitializationT;
//==============================================================================
//functions:

xResult ADC_TransferLayerAdapterInit(xTransferLayerT* layer, ADC_TransferLayerAdapterInitializationT* initialization);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_ADC_TARNSFER_LAYER_ADAPTER_H_
