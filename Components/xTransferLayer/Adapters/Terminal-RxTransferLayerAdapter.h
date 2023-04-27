//==============================================================================
//header:

#ifndef _TERMINAL_RX_TARNSFER_LAYER_ADAPTER_H_
#define _TERMINAL_RX_TARNSFER_LAYER_ADAPTER_H_
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
	TerminalObjectT TerminalObject;

} TerminalRxTransferLayerAdapterDataT;
//------------------------------------------------------------------------------
typedef struct
{
	xTransferLayerAdapterBaseT Base;

	TerminalRxTransferLayerAdapterDataT Data;

} TerminalRxTransferLayerAdapterT;
//==============================================================================
//functions:

xResult TerminalRxTransferLayerAdapterInit(xTransferLayerT* layer, TerminalRxTransferLayerAdapterT* adapter);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_TERMINAL_RX_TARNSFER_LAYER_ADAPTER_H_
