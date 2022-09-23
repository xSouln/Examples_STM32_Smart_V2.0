//==============================================================================
#ifndef SERIAL_PORT_ADAPTERS_TYPES_H
#define SERIAL_PORT_ADAPTERS_TYPES_H
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
	SerialPortAdapterResultAccept,
	SerialPortAdapterResultError
	
} SerialPortAdapterResult;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* SERIAL_PORT_ADAPTERS_TYPES_H */

