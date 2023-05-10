//==============================================================================
//header:

#ifndef _TCP_SERVER_WIZ_SPI_COMPONENT_H_
#define _TCP_SERVER_WIZ_SPI_COMPONENT_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "TCPServer_WIZspiComponentConfig.h"
#include "TCPServer/Controls/TCPServer.h"
//==============================================================================
//defines:


//==============================================================================
//functions:

xResult _TCPServerWIZspiComponentInit(void* parent);
void _TCPServerWIZspiComponentHandler();
void _TCPServerWIZspiComponentTimeSynchronization();
void _TCPServerWIZspiComponentEventListener(TCPServerT* server, TCPServerSysEventSelector selector, void* arg);
xResult _TCPServerWIZspiComponentRequestListener(TCPServerT* server, TCPServerSysRequestSelector selector, void* arg);
void _TCPServerWIZspiComponentIRQListener(TCPServerT* port, ...);
//==============================================================================
//import:


//==============================================================================
//override:

#if TCP_SERVER_WIZ_SPI_COMPONENT_ENABLE == 1
#define TCPServerWIZspiComponentInit(parent) _TCPServerWIZspiComponentInit(parent)

#define TCPServerWIZspiComponentHandler() TCPServerHandler(&TCPServerWIZspi)
#define TCPServerWIZspiComponentTimeSynchronization() TCPServerTimeSynchronization(&TCPServerWIZspi)

#define TCPServerWIZspiComponentIRQListener(server, ...) _TCPServerComponentIRQListener(server, ##__VA_ARGS__)

#define TCPServerWIZspiComponentEventListener(server, selector, arg, ...) _SerialPortUARTComponentEventListener(port, selector, arg, ##__VA_ARGS__)
#define TCPServerWIZspiComponentRequestListener(server, selector, arg, ...) _SerialPortUARTComponentRequestListener(port, selector, arg, ##__VA_ARGS__)
#else
#define TCPServerWIZspiComponentInit(parent)

#define TCPServerWIZspiComponentHandler()
#define TCPServerWIZspiComponentTimeSynchronization()

#define TCPServerWIZspiComponentIRQListener(server, ...)

#define TCPServerWIZspiComponentEventListener(server, selector, arg, ...)
#define TCPServerWIZspiComponentRequestListener(server, selector, arg, ...)
#endif
//==============================================================================
//export:

extern TCPServerT TCPServerWIZspi;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_TCP_SERVER_WIZ_SPI_COMPONENT_H_
