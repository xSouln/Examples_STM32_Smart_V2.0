//==============================================================================
#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "SerialPort/Adapters/SerialPort_Adapters.h"
#include "SerialPort/SerialPort_Types.h"
//==============================================================================
void SerialPort_Handler(SerialPortT* serial_port);

SerialPortResult SerialPort_Init(SerialPortT* serial_port, void* parent);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* SERIAL_PORT_H_ */
