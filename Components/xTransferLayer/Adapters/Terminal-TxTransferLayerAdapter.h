//==============================================================================
//header:

#ifndef _TERMINAL_TX_TARNSFER_LAYER_ADAPTER_H_
#define _TERMINAL_TX_TARNSFER_LAYER_ADAPTER_H_
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

} TerminalTxTransferLayerAdapterDataT;
//------------------------------------------------------------------------------
typedef struct
{
	xTransferLayerAdapterBaseT Base;

	TerminalTxTransferLayerAdapterDataT Data;

	const char* HeaderTransferStart;
	const char* HeaderTransfer;
	const char* HeaderTransferEnd;

} TerminalTxTransferLayerAdapterT;
//==============================================================================
//functions:

xResult TerminalTxTransferLayerAdapterInit(xTransferLayerT* layer, TerminalTxTransferLayerAdapterT* adapter);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_TERMINAL_TX_TARNSFER_LAYER_ADAPTER_H_
