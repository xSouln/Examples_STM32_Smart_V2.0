//==============================================================================
#ifndef RGB_CUPS_RX_TRANSACTIONS_H
#define RGB_CUPS_RX_TRANSACTIONS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "Common/xRxTransaction.h"
#include "Common/xTx.h"
#include "RGBCups/RGBCups_Info.h"
//==============================================================================
typedef enum
{
  RGB_CUPS_GET_FIRMWARE_VERSION = 100,
	RGB_CUPS_GET_PIXELS,
	
	RGB_CUPS_SET = 1000,
	RGB_CUPS_SET_PIXELS,
	RGB_CUPS_SET_PIXELS_STATE,

	RGB_CUPS_TRY = 2000,

	RGB_CUPS_EVT = 10000,
	
} RGB_CUPS_TRANSACTIONS;
//==============================================================================
static uint32_t RGB_CUPS_PACKET_DEVICE_KEY = RGB_CUPS_DEVICE_KEY;
//------------------------------------------------------------------------------
static PacketIdentificatorT RGB_CUPS_RESPONSE_IDENTIFICATOR = 
{
	.Value = (uint32_t)(PACKET_HEADER_IDENTIFICATOR | (PACKET_HEADER_DESCRIPTION_RESPONSE << 8))
};
//------------------------------------------------------------------------------
static PacketIdentificatorT RGB_CUPS_EVENT_IDENTIFICATOR = 
{
	.Value = (uint32_t)(PACKET_HEADER_IDENTIFICATOR | (PACKET_HEADER_DESCRIPTION_EVENT << 8))
};
//------------------------------------------------------------------------------
static PacketHeaderT RGB_CUPS_REQUEST_HEADER =
{
	.Identificator =
	{
		.Value = (uint32_t)(PACKET_HEADER_IDENTIFICATOR | (PACKET_HEADER_DESCRIPTION_REQUEST << 8))
	},
	.DeviceKey = RGB_CUPS_DEVICE_KEY,
};
//------------------------------------------------------------------------------
static char RGB_CUPS_PACKET_END[] = "\r";
//==============================================================================
int CupsRGB_TransmitEvent(xTxT* tx, RGB_CUPS_TRANSACTIONS transaction, xObject data, uint16_t data_size);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* RGB_CUPS_RX_TRANSACTIONS_H */

