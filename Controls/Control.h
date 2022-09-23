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
#include "SerialPort/SerialPort_Components.h"
#include "TCPServer/TCPServer_Components.h"
#include "RxTransactions.h"
#include "RxRequests.h"
#include "RGBCups/RGBCups_Components.h"
//==============================================================================
typedef struct
{
	OBJECT_HEADER;
	
	xRxRequestManagerT RequestManager;
	
	RGBCupT* RGBCups;
	
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
