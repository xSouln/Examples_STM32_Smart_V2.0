//==============================================================================
#ifndef CONTROL_H
#define CONTROL_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "Types.h"
#include "Info.h"
#include "Config.h"
#include "SerialPort/SerialPort.h"
#include "SerialPort/USBSerialPort.h"
#include "Transactions.h"
#include "Requests.h"
#include "CupsRGB/CupsRGB_Component.h"
//==============================================================================
typedef struct
{
	OBJECT_HEADER;
	
	xRequestManagerT RequestManager;
	
	CupsRGB_ControlT* CupsRGB_Control;
	
} ControlT;
//==============================================================================
extern ControlT Control;
//------------------------------------------------------------------------------
ActionResults ControlInit(void* parent);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* CONTROL_H */
