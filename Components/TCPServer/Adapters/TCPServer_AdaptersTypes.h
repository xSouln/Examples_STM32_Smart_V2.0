//==============================================================================
#ifndef TCP_SEVER_ADAPTERS_TYPES_H
#define TCP_SEVER_ADAPTERS_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
//==============================================================================
#include "main.h"
#include "Common/xTx.h"
#include "Common/xRx.h"
#include "Common/xRxReceiver.h"
//==============================================================================
typedef enum
{
	TCPServerAdapterResultAccept,
	TCPServerAdapterResultError
	
} TCPServerAdapterResult;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* TCP_SEVER_ADAPTERS_TYPES_H */

