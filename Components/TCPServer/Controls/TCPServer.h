//==============================================================================
#ifndef TCP_SERVER_H
#define TCP_SERVER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "TCPServer/TCPServer_Types.h"
#include "TCPServer/Adapters/TCPServer_Adapters.h"
//==============================================================================
void TCPServer_Handler(TCPServerT* server);

TCPServerResult TCPServer_Init(TCPServerT* server, void* parent);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* TCP_SERVER_H */
