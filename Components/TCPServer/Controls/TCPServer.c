//==============================================================================
#include "Common/xMemory.h"
#include "TCPServer.h"
//==============================================================================
void TCPServer_Handler(TCPServerT* server)
{
	xRxHandler(&server->Rx);
  xTxHandler(&server->Tx);
}
//==============================================================================
static void EventListener(TCPServerT* server, TCPServerEventSelector selector, uint32_t args, uint32_t count)
{
	switch ((uint32_t)selector)
	{
		default : return;
	}
}
//------------------------------------------------------------------------------
TCPServerResult TCPServer_Init(TCPServerT* server, void* parent)
{
	if (server)
	{
		server->Description = "TCPServerT";
		server->Parent = parent;
		server->Interface.EventListener = (TCPServerEventListener)EventListener;
		
		return TCPServerResultAccept;
	}
	return TCPServerResultError;
}
//==============================================================================
