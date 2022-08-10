/*
 * Bootloader_Requests.h
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */

#ifndef _BOOT_REQUESTS_H_
#define _BOOT_REQUESTS_H_
//==============================================================================
#include "xType.h"
#include "xTransaction.h"
//==============================================================================
enum BOOT_REQUESTS
{
  BOOT_REQUESTS_START = 0,
    
  BOOT_REQUESTS_GET_START = 100,
  BOOT_GET_FIRMWARE,
  BOOT_GET_INFO,
  BOOT_GET_APP_INFO,
  BOOT_GET_STATUS,
  BOOT_GET_HANDLER,
  BOOT_REQUESTS_GET_END,
  
  BOOT_REQUESTS_SET_START = 200,
  BOOT_SET_FLASH_LOCK_STATE,
  BOOT_SET_OPTIONS,
  BOOT_SET_HANDLER,
  BOOT_REQUESTS_SET_END,
  
  BOOT_REQUESTS_TRY_START = 300,
  BOOT_TRY_RESET,
  BOOT_TRY_ERASE,
  BOOT_TRY_WRITE,
  BOOT_TRY_READ,
  BOOT_TRY_JUMP_TO_BOOT,
  BOOT_TRY_JUMP_TO_MAIN,
  BOOT_TRY_UPDATE_INFO,
  BOOT_TRY_READ_CRC,
  BOOT_REQUESTS_TRY_END,
  
  BOOT_REQUESTS_EVT_START = 400,
  BOOT_EVT_WRITE_COMPLITE,
  BOOT_EVT_READ_COMPLITE,
  BOOT_REQUESTS_EVT_END,
  
  BOOT_REQUESTS_END = 500
};
//==============================================================================
static char BOOT_REQUEST_HEADER[] = "#RQBL:";
static char BOOT_RESPONSE_HEADER[] = "#RSBL:";

static char BOOT_REQUEST_END[] = "\r";
static char BOOT_RESPONSE_END[] = "\r";
//==============================================================================
extern const xTransactionT Bootloader_Requests[];

void Bootloader_RequestAction(xEventBaseT* event, uint8_t* object, uint16_t size);
//==============================================================================
#endif /* _BOOT_REQUESTS_H_ */
