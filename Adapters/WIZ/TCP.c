//==============================================================================
#include <string.h>
#include "main.h"
#include "TCP.h"
#include "w5500.h"
#include "socket.h"
//==============================================================================
#define RX_CIRCLE_BUF_SIZE 0xff
#define RX_OBJECT_BUF_SIZE 0x1ff
#define TX_CIRCLE_BUF_SIZE 0x7ff

static uint8_t rx_circle_buf[RX_CIRCLE_BUF_SIZE];
static uint8_t rx_object_buf[RX_OBJECT_BUF_SIZE];

static uint8_t tx_circle_buf[TX_CIRCLE_BUF_SIZE];
//==============================================================================
//==============================================================================
static void TCP_DELAY(uint32_t time)
{
  TCP.Delay = time;
  while(TCP.Delay) { }
}
//==============================================================================
static inline void TCP_RESET_ENABLE()
{
	WIZ_RESET_GPIO_Port->ODR &= ~WIZ_RESET_Pin;
}
//==============================================================================
static inline void TCP_RESET_DISABLE()
{
	WIZ_RESET_GPIO_Port->ODR |= WIZ_RESET_Pin;
}
//==============================================================================
static void TCP_DRIVER_CHIP_SELECT()
{
	WIZ_CS_GPIO_Port->ODR &= ~WIZ_CS_Pin;
}
//==============================================================================
static void TCP_DRIVER_CHIP_DESELECT()
{
  WIZ_CS_GPIO_Port->ODR |= WIZ_CS_Pin;
}
//==============================================================================
static void TCP_DRIVER_TRANSMIT_BYTE(uint8_t byte)
{
  TCP.SPI->CR2.CurrentDataSize = 1;
  TCP.SPI->CR1.SpiEnable = true;
  TCP.SPI->CR1.MasterTransferStart = true;
  
  while(!TCP.SPI->SR.TxEmpty){ };
  TCP.SPI->TXDR = byte;
  
  while(!TCP.SPI->SR.TxFifoComplete){ };
  byte = TCP.SPI->RXDR;
  
  TCP.SPI->IFCR.EndOfTransfer = true;
  TCP.SPI->IFCR.TransferFilledClear = true;
  TCP.SPI->CR1.SpiEnable = false;
}
//==============================================================================
static uint8_t TCP_DRIVER_RECEIVE_BYTE()
{
  uint8_t byte = 0xff;
  
  TCP.SPI->CR2.CurrentDataSize = 1;
  TCP.SPI->CR1.SpiEnable = true;
  TCP.SPI->CR1.MasterTransferStart = true;

  while(!TCP.SPI->SR.TxEmpty){ };
  TCP.SPI->TXDR = byte;

  while(!TCP.SPI->SR.TxFifoComplete){ };
  byte = TCP.SPI->RXDR;

  TCP.SPI->IFCR.EndOfTransfer = true;
  TCP.SPI->IFCR.TransferFilledClear = true;
  TCP.SPI->CR1.SpiEnable = false;

  return byte;
}
//==============================================================================
void TCP_RX_HANDLER()
{  
  if(TCP.Sock.Status.Connected)
  {
    uint16_t len = getSn_RX_RSR(TCP.Sock.Number);		
    if(len)
    {
      if (len > RX_CIRCLE_BUF_SIZE) { len = RX_CIRCLE_BUF_SIZE; }
      len = recv(TCP.Sock.Number, TCP.Rx.CircleReceiver.Buffer, len);
      xRxObjectReceive(&TCP.Rx, TCP.Rx.CircleReceiver.Buffer, len);
    }
  }
}
//==============================================================================
void TCP_TX_HANDLER()
{
  
}
//==============================================================================
void TCP_CONNECTION_HANDLER()
{  
  TCP.Sock.State = getSn_SR(TCP.Sock.Number);
  //TCP.Sock.FreeSize = getSn_TX_FSR(TCP.Sock.Number);
  
  switch(TCP.Sock.State)
  {
    case SOCK_ESTABLISHED:
			if((TCP.Sock.State & Sn_IR_CON))
			{
				TCP.Sock.Status.Connected = true;
				setSn_IR(TCP.Sock.Number, Sn_IR_CON);
			}
			break;

    case SOCK_CLOSE_WAIT:      
			disconnect(TCP.Sock.Number);
			TCP.Sock.Status.Connected = false;
			break;

    case SOCK_CLOSED:
			close(TCP.Sock.Number);
			TCP.Sock.Status.Connected = false;
			socket(TCP.Sock.Number, Sn_MR_TCP, TCP.Sock.Port, 0);
			break;
    
    case SOCK_INIT:
			TCP.Sock.Status.Connected = false;
			listen(TCP.Sock.Number);
			break;
    
    default: break;    
  }
}
//==============================================================================
xResult TCP_TransmitData(xObject data, uint16_t size)
{
  if(data && size && TCP.Sock.Status.Connected)
  {
    return send(TCP.Sock.Number, (uint8_t*)data, size);
  }
  return -1;
}
//==============================================================================
xResult TCP_TransmitString(char* string)
{
  return TCP_TransmitData(string, strlen(string));
}
//==============================================================================
//------------------------------------------------------------------------------
static xTxStatus GetTransmiterStatus(xTxT* tx)
{
	return TCP.Sock.Status.Connected ? xTxStatusFree : xTxStatusError;
}
//------------------------------------------------------------------------------
static xResult TransmitData(xTxT* tx, xObject data, uint16_t data_size)
{
	xResult result = xResultError;
	if (TCP.Sock.Status.Connected && data && data_size)
	{
		result = send(TCP.Sock.Number, (uint8_t*)data, data_size) > 0 ? xResultAccept : xResultError;
	}
	
	if (result == xResultError)
	{
		return result;
	}
	return result;
}
//------------------------------------------------------------------------------
static uint16_t GetFreeSize(xTxT* tx)
{
	if (TCP.Sock.Status.Connected)
	{
		TCP.Sock.FreeSize = getSn_TX_FSR(TCP.Sock.Number);
	}
	else
	{
		TCP.Sock.FreeSize = 0;
	}
	return TCP.Sock.FreeSize;
}
//------------------------------------------------------------------------------
static xTxControlT tx_control =
{
	.TransmitData = (xTxActionTransmitData)TransmitData,
	.GetFreeSize = (xTxActionGetFreeSize)GetFreeSize,
	.GetTransmiterStatus = (xTxActionGetTransmiterStatus)GetTransmiterStatus
};
//==============================================================================
int TCP_Init(void* spi_reg, void* parent)
{
	if (TCP.Description) { TCP.Description = "TCP_T"; }
	TCP.Parent = parent;
	
  uint8_t tx_mem_conf[8] = {16,0,0,0,0,0,0,0}; // for setting TMSR regsiter
  uint8_t rx_mem_conf[8] = {16,0,0,0,0,0,0,0}; // for setting RMSR regsiter
	
	TCP.SPI = (volatile STM32_SPI_REG_T*)spi_reg;
	
	TCP.Tx.Rx = &TCP.Rx;
  TCP.Rx.Tx = &TCP.Tx;
  
	extern xResult RxEndLine(xObject rx, uint8_t* obj, uint16_t size);
  xRxInit(&TCP.Rx, &TCP,
          rx_circle_buf, RX_CIRCLE_BUF_SIZE,
          rx_object_buf, RX_OBJECT_BUF_SIZE,
          (xRxEventEndLine)RxEndLine);
  
	extern xDataBufferT MainDataBuffer;
  xTxInit(&TCP.Tx, &TCP,
					0, 0,
          //tx_circle_buf, TX_CIRCLE_BUF_SIZE,
          &tx_control);
	TCP.Tx.ObjectBuffer = &MainDataBuffer;
  
  TCP_RESET_ENABLE();
  TCP_DELAY(100);
  TCP_RESET_DISABLE();
  TCP_DELAY(200);
  
  reg_wizchip_spi_cbfunc(TCP_DRIVER_RECEIVE_BYTE, TCP_DRIVER_TRANSMIT_BYTE);//TCP_driver_transmitter);
  reg_wizchip_cs_cbfunc(TCP_DRIVER_CHIP_SELECT, TCP_DRIVER_CHIP_DESELECT);
	
	wizchip_setinterruptmask(IK_SOCK_ALL);
  
  setMR(MR_RST);  
  TCP_DELAY(300);
  
  wizchip_init(tx_mem_conf, rx_mem_conf);
  
  setSHAR((uint8_t*)TCP_DEFAULT_MAC_ADDRESS); // set source hardware address
  setGAR((uint8_t*)TCP_DEFAULT_GETAWAY); // set gateway IP address
  setSUBR((uint8_t*)TCP_DEFAULT_NETMASK);
  setSIPR((uint8_t*)TCP_DEFAULT_IP); // set source IP address
  
  getSHAR(TCP.Info.Mac); // get source hardware address	
  getGAR(TCP.Info.Gateway); // get gateway IP address
  getSIPR(TCP.Info.Ip); // get source IP address	
  getSUBR(TCP.Info.Subnet);
  
  close(TCP.Sock.Number);
  disconnect(TCP.Sock.Number);
	
	return 0;
}
//==============================================================================
TCP_T TCP =
{
  .Sock = 
  {
    .Port = TCP_DEFAULT_PORT
  }
};
//==============================================================================
