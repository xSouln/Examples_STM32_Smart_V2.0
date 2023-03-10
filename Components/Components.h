//==============================================================================
#ifndef _COMPONENTS_H
#define _COMPONENTS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "Components_Types.h"
#include "Components_Config.h"
//==============================================================================
//defines:


//==============================================================================
//includes:

//#include "rng.h"
//==============================================================================
//configurations:

#include "Terminal/Terminal_Component.h"
#include "SerialPort/USART/SerialPort_UART_Component.h"
#include "TCPServer/WIZspi/TCPServer_WIZspiComponent.h"
//==============================================================================
//functions:

xResult ComponentsInit(void* parent);
void ComponentsTimeSynchronization();
void ComponentsHandler();

void ComponentsEventListener(ComponentObjectBaseT* object, int selector, void* arg, ...);
void ComponentsRequestListener(ComponentObjectBaseT* object, int selector, void* arg, ...);
//==============================================================================
//export:


//==============================================================================
//override:

#define ComponentsSysGetTime() HAL_GetTick()
//==============================================================================
//variables:

//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif

