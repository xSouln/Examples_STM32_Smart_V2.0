/*
 * Transactions.h
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "Common/xType.h"
#include "Common/xTx.h"
#include "Common/xTransaction.h"
//==============================================================================
typedef enum
{
  TRANSACTION_GET_FIRMWARE_VERSION = 100,
	TRANSACTION_GET_TIME,

	TRANSACTION_SET = 1000,
	TRANSACTION_SET_TIME,

	TRANSACTION_TRY = 2000,
	TRANSACTION_TRY_RESET_TIME,

	TRANSACTION_EVT = 10000,
	
} DEVICE_TRANSACTIONS;
//==============================================================================
#define DEVICE_KEY 0x45389560

static uint32_t PACKET_DEVICE_KEY = DEVICE_KEY;
//------------------------------------------------------------------------------
static PacketIdentificatorT DEVICE_RESPONSE_IDENTIFICATOR = 
{
	.Value = (uint32_t)(PACKET_HEADER_IDENTIFICATOR | (PACKET_HEADER_DESCRIPTION_RESPONSE << 8))
};
//------------------------------------------------------------------------------
static PacketIdentificatorT DEVICE_EVENT_IDENTIFICATOR = 
{
	.Value = (uint32_t)(PACKET_HEADER_IDENTIFICATOR | (PACKET_HEADER_DESCRIPTION_EVENT << 8))
};
//------------------------------------------------------------------------------
static PacketHeaderT DEVICE_REQUEST_HEADER =
{
	.Identificator =
	{
		.Value = (uint32_t)(PACKET_HEADER_IDENTIFICATOR | (PACKET_HEADER_DESCRIPTION_REQUEST << 8))
	},
	.DeviceKey = DEVICE_KEY,
};
//------------------------------------------------------------------------------
static char PACKET_END[] = "\r";
//==============================================================================
int Device_TransmitEvent(xTxT* tx, DEVICE_TRANSACTIONS transaction, xObject data, uint16_t data_size);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* TRANSACTIONS_H */
