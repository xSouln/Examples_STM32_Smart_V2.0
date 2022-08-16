//==============================================================================
#ifndef CUPS_RGB_TRANSACTIONS_H
#define CUPS_RGB_TRANSACTIONS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "Common/xTransaction.h"
#include "Common/xTx.h"
#include "CupsRGB/Controls/CupsRGB_Info.h"
//==============================================================================
typedef enum
{
  CUPS_RGB_GET_FIRMWARE_VERSION = 100,
	CUPS_RGB_GET_PIXELS,
	
	CUPS_RGB_SET = 1000,
	CUPS_RGB_SET_PIXELS,
	CUPS_RGB_SET_PIXELS_STATE,

	CUPS_RGB_TRY = 2000,

	CUPS_RGB_EVT = 10000,
	
} CUPS_RGB_TRANSACTIONS;
//==============================================================================
static uint32_t CUPS_RGB_PACKET_DEVICE_KEY = CUPS_RGB_DEVICE_KEY;
//------------------------------------------------------------------------------
static PacketIdentificatorT CUPS_RGB_RESPONSE_IDENTIFICATOR = 
{
	.Value = (uint32_t)(PACKET_HEADER_IDENTIFICATOR | (PACKET_HEADER_DESCRIPTION_RESPONSE << 8))
};
//------------------------------------------------------------------------------
static PacketIdentificatorT CUPS_RGB_EVENT_IDENTIFICATOR = 
{
	.Value = (uint32_t)(PACKET_HEADER_IDENTIFICATOR | (PACKET_HEADER_DESCRIPTION_EVENT << 8))
};
//------------------------------------------------------------------------------
static PacketHeaderT CUPS_RGB_REQUEST_HEADER =
{
	.Identificator =
	{
		.Value = (uint32_t)(PACKET_HEADER_IDENTIFICATOR | (PACKET_HEADER_DESCRIPTION_REQUEST << 8))
	},
	.DeviceKey = CUPS_RGB_DEVICE_KEY,
};
//------------------------------------------------------------------------------
static char CUPS_RGB_PACKET_END[] = "\r";
//==============================================================================
int CupsRGB_TransmitEvent(xTxT* tx, CUPS_RGB_TRANSACTIONS transaction, xObject data, uint16_t data_size);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* CUPS_RGB_TRANSACTIONS_H */

