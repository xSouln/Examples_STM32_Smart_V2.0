//==============================================================================
#ifndef SERIAL_PORT_TYPES_H
#define SERIAL_PORT_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "main.h"
#include "Common/xTx.h"
#include "Common/xRx.h"
#include "SerialPort_Config.h"
#include "SerialPort_Info.h"
//==============================================================================
typedef enum
{
	SerialPortResultAccept,
	SerialPortResultError
	
} SerialPortResult;
//==============================================================================
typedef struct
{
	OBJECT_HEADER;
	
	void* Adapter;

	xRxT Rx;
	xTxT Tx;
	  
} SerialPortT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* SERIAL_PORT_TYPES_H */
