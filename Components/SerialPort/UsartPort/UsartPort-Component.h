//==============================================================================
//header:

#ifndef _USART_PORT_COMPONENT_H_
#define _USART_PORT_COMPONENT_H_
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "UsartPort-ComponentConfig.h"
#include "Common/xPort/xPort.h"
//==============================================================================
//defines:


//==============================================================================
//macros:


//==============================================================================
//functions:

xResult UsartPortComponentInit(void* parent);

void UsartPortComponentHandler();
void UsartPortComponentTimeSynchronization();
void UsartPortComponentIRQ();
//==============================================================================
//import:


//==============================================================================
//export:

extern xPortT UsartPort;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_USART_PORT_COMPONENT_H_
