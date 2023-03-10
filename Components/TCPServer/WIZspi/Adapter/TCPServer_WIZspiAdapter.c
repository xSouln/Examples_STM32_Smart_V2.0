//==============================================================================
//includes:

#include "TCPServer_WIZspiAdapter.h"
#include "TCPServer_WIZspiTxAdapter.h"
#include "TCPServer_WIZspiRxAdapter.h"
#include "Common/xMemory.h"
#include "w5500.h"
#include "socket.h"
//==============================================================================
//functions:

static void PrivateHandler(TCPServerT* server)
{
	//TCPServerWIZspiAdapterT* adapter = server->Adapter.Child;
	uint8_t state = getSn_SR(server->Sock.Number);

	switch(state)
	{
		case SOCK_ESTABLISHED:
			if(state & Sn_IR_CON)
			{
				server->Sock.State = TCPServerSockConnected;
				setSn_IR(server->Sock.Number, Sn_IR_CON);
			}
			break;

		case SOCK_CLOSE_WAIT:
			disconnect(server->Sock.Number);
			server->Sock.State = TCPServerSockDisconnecting;
			break;

		case SOCK_CLOSED:
			close(server->Sock.Number);
			server->Sock.State = TCPServerSockDisconnect;
			socket(server->Sock.Number, Sn_MR_TCP, server->Sock.Port, 0);
			break;

		case SOCK_INIT:
			server->Sock.State = TCPServerSockDisconnect;
			listen(server->Sock.Number);
			break;

		default: break;
	}
}
//------------------------------------------------------------------------------

static void PrivateEventListener(TCPServerT* server, TCPServerAdapterEventSelector selector, uint32_t args, ...)
{
	switch ((uint32_t)selector)
	{
		default : return;
	}
}
//------------------------------------------------------------------------------

static void PrivateIRQListener(TCPServerT* server)
{

}
//------------------------------------------------------------------------------

static xResult PrivateRequestListener(TCPServerT* server, TCPServerAdapterRequestSelector selector, uint32_t arg, ...)
{
	switch ((uint32_t)selector)
	{

		default : return xResultRequestIsNotFound;
	}
	
	return xResultAccept;
}
//==============================================================================
//interfaces:

static TCPServerAdapterInterfaceT Interface =
{
	INITIALIZATION_HANDLER(TCPServerAdapter, PrivateHandler),

	INITIALIZATION_IRQ_LISTENER(TCPServerAdapter, PrivateIRQListener),

	INITIALIZATION_EVENT_LISTENER(TCPServerAdapter, PrivateEventListener),
	INITIALIZATION_REQUEST_LISTENER(TCPServerAdapter, PrivateRequestListener)
};
//==============================================================================
//initialization:

xResult TCPServerWIZspiAdapterInit(TCPServerT* server, TCPServerWIZspiAdapterT* adapter)
{
	server->Adapter.Object.Description = "TCPServerWIZspiAdapterT";
	server->Adapter.Object.Parent = server;
	server->Adapter.Child = adapter;
	server->Adapter.Interface = &Interface;
	
	TCPServerWIZspiTxAdapterInit(server, adapter);
	TCPServerWIZspiRxAdapterInit(server, adapter);
	
	server->Rx.Tx = &server->Tx;
	
	uint8_t tx_mem_conf[8] = {16,0,0,0,0,0,0,0}; // for setting TMSR regsiter
	uint8_t rx_mem_conf[8] = {16,0,0,0,0,0,0,0}; // for setting RMSR regsiter
	
	adapter->BusInterface.HardwareResetOn();
	TCPServerDelay(server, 10);
	adapter->BusInterface.HardwareResetOff();
	TCPServerDelay(server, 100);

	reg_wizchip_spi_cbfunc(adapter->BusInterface.ReceiveByte, adapter->BusInterface.TransmiteByte);//TCP_driver_transmitter);
	reg_wizchip_cs_cbfunc(adapter->BusInterface.SelectChip, adapter->BusInterface.DeselectChip);

	setMR(MR_RST);
	TCPServerDelay(server, 100);

	wizchip_setinterruptmask(IK_SOCK_ALL);

	wizchip_init(tx_mem_conf, rx_mem_conf);

	setSHAR(server->Options.Mac); // set source hardware address
	setGAR(server->Options.Gateway); // set gateway IP address
	setSUBR(server->Options.NetMask); // set netmask
	setSIPR(server->Options.Ip); // set source IP address

	getSHAR(server->Info.Mac); // get source hardware address
	getGAR(server->Info.Gateway); // get gateway IP address
	getSIPR(server->Info.Ip); // get source IP address
	getSUBR(server->Info.NetMask); // set netmask
	
	if (xMemoryCompare(&server->Options, &server->Info, sizeof(server->Info)) != 0)
	{
		return xResultError;
	}
	
	close(server->Sock.Number);
	disconnect(server->Sock.Number);

	return xResultAccept;
}
//==============================================================================
