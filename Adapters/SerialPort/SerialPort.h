//==============================================================================
#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "Common/xTx.h"
#include "Common/xType.h"
#include "Common/xRx.h"
//==============================================================================
typedef struct
{
	OBJECT_HEADER;

	xRxT* Rx;
	xTxT* Tx;

	volatile STM32_UART_REG_T* Reg;
	  
} SerialPortT;
//==============================================================================
extern SerialPortT SerialPort;
//==============================================================================
int SerialPortInit(void* reg, void* parent);

int SerialPortHandler();

extern void SerialPortIRQ();
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* _SERIALPORT_H_ */
