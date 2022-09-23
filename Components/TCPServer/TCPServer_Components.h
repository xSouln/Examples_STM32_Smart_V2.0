//==============================================================================
#ifndef TCP_SERVER_COMPONENTS_H
#define TCP_SERVER_COMPONENTS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "TCPServer/Controls/TCPServer.h"
//==============================================================================
extern TCPServerT TCPServerWIZspi;

int TCPServerWIZspi_ComponentInit(void* parent);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* TCP_SERVER_COMPONENT_H */
