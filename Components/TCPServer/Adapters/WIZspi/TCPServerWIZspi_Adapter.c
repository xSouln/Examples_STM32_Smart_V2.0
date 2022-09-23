//==============================================================================
#include "TCPServerWIZspi_Adapter.h"
#include "TCPServerWIZspi_TxAdapter.h"
#include "TCPServerWIZspi_RxAdapter.h"
//==============================================================================
static xResult RequestListener(TCPServerT* server, TCPServerRequestSelector selector, uint32_t args, uint32_t count)
{
	switch ((uint32_t)selector)
	{
		default : return xResultRequestIsNotFound;
	}
	
	return xResultRequestIsNotFound;
}
//------------------------------------------------------------------------------
static int GetValue(TCPServerT* server, TCPServerValueSelector selector)
{
	switch ((uint32_t)selector)
	{
		default : return -1;
	}
}
//------------------------------------------------------------------------------
static xResult SetValue(TCPServerT* server, TCPServerValueSelector selector, uint32_t arg)
{
	switch ((uint32_t)selector)
	{
		default : return xResultValueIsNotFound;
	}
	
	return xResultValueIsNotFound;
}
//------------------------------------------------------------------------------
static void Handler(TCPServerT* server)
{
	server->Sock.State = getSn_SR(server->Sock.Number);
  
  switch(server->Sock.State)
  {
    case SOCK_ESTABLISHED:
			if((server->Sock.State & Sn_IR_CON))
			{
				server->Sock.Status.Connected = true;
				setSn_IR(server->Sock.Number, Sn_IR_CON);
			}
			break;

    case SOCK_CLOSE_WAIT:      
			disconnect(server->Sock.Number);
			server->Sock.Status.Connected = false;
			break;

    case SOCK_CLOSED:
			close(server->Sock.Number);
			server->Sock.Status.Connected = false;
			socket(server->Sock.Number, Sn_MR_TCP, server->Sock.Port, 0);
			break;
    
    case SOCK_INIT:
			server->Sock.Status.Connected = false;
			listen(server->Sock.Number);
			break;
    
    default: break;    
  }
}
//==============================================================================
int TCPServerWIZspi_AdapterInit(TCPServerT* server, TCPServerWIZspi_AdapterT* adapter)
{
	server->Adapter = adapter;
	
	server->Interface.Handler = (TCPServerActionHandler)Handler;
	//server->Interface.EventsListener = (TCPServerEventsListener)EventsListener,
	server->Interface.RequestListener = (TCPServerRequestListener)RequestListener,
	server->Interface.GetValue = (TCPServerActionGetValue)GetValue,
	server->Interface.SetValue = (TCPServerActionSetValue)SetValue,
	
	TCPServerWIZspi_TxAdapterInit(&server->Tx, server, adapter);
	TCPServerWIZspi_RxAdapterInit(&server->Rx, server, adapter);
	
	server->Rx.Tx = &server->Tx;
  
  return TCPServerAdapterResultAccept;
}
//==============================================================================
