/*
 * Bootloader_Requests.c
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */
//==============================================================================
#include "xTransaction.h"
#include "Bootloader_Requests.h"
#include "xTx.h"
#include "Bootloader.h"
#include "xFlash.h"
#include "xMacroces.h"
//==============================================================================
EVENT_PATTERN(Response,
               xRxT* Rx,
               xTransactionT* Transaction);
               
EVENT_PATTERN(Command,
               xRxT* Rx,
               xRequestT* Command);
//==============================================================================
void Bootloader_RESPONSE_GET(xEventResponseT* event, xObject obj, uint16_t size, int16_t error)
{
  ResponseInfoT info =
  {
    .Key = event->Transaction->Id,
    .Size = event->Transaction->Content.size
  };
  
  xTxAdd(((xEventCommandT*)event)->Rx->Tx, &info, sizeof(info));  
  xTxAdd(((xEventCommandT*)event)->Rx->Tx, event->Transaction->Content.obj, event->Transaction->Content.size);
}
//==============================================================================
void Bootloader_RESPONSE_DEFAULT(xEventResponseT* event, xObject obj, uint16_t size, int16_t error)
{
  ResponseInfoT info =
  {
    .Key = event->Transaction->Id,
    .Size = event->Transaction->Content.size + sizeof(error)
  };
  
  xTxAdd(((xEventCommandT*)event)->Rx->Tx, &info, sizeof(info));
  xTxAdd(((xEventCommandT*)event)->Rx->Tx, &error, sizeof(error));
  xTxAdd(((xEventCommandT*)event)->Rx->Tx, event->Transaction->Content.obj, event->Transaction->Content.size);
}
//==============================================================================
void Bootloader_RequestAction(xEventBaseT* event, uint8_t* object, uint16_t size)
{
  uint16_t action_error = ACCEPT;
  
  xTransactionT* transaction = xTransactionIdentify(event, (xTransactionT*)Bootloader_Requests, ((RequestInfoT*)object)->Key);
  
  if(transaction)
  {
    xTransactionT context;
    COPY_MEMORY(&context, transaction, sizeof(xTransactionT));
    
    event->Context = &context;
    
    object += sizeof(RequestInfoT);
    size -= sizeof(RequestInfoT);
    
    if(transaction->Action)
    {
      action_error = transaction->Action(event, object, size);
    }
    
    if(transaction->Response)
    {
      xTxAdd(((xEventCommandT*)event)->Rx->Tx, BOOT_RESPONSE_HEADER, SIZE_STRING(BOOT_RESPONSE_HEADER));
      transaction->Response(event, object, size, action_error);
      xTxAdd(((xEventCommandT*)event)->Rx->Tx, BOOT_RESPONSE_END, SIZE_STRING(BOOT_RESPONSE_END));
    }
  }
}
//==============================================================================
const xTransactionT Bootloader_Requests[] =
{
  NEW_TRANSACTION0(BOOT_GET_INFO, Bootloader_RESPONSE_GET, 0, Bootloader.FirmwareInfo),
  NEW_TRANSACTION0(BOOT_GET_APP_INFO, Bootloader_RESPONSE_GET, 0, Bootloader.AppInfo),
  NEW_TRANSACTION0(BOOT_GET_FIRMWARE, Bootloader_RESPONSE_GET, 0, BOOT_FIRMWARE_VERSION),
  NEW_TRANSACTION0(BOOT_GET_STATUS, Bootloader_RESPONSE_GET, 0, xFlash.Status),
  
  NEW_TRANSACTION0(BOOT_SET_FLASH_LOCK_STATE, Bootloader_RESPONSE_DEFAULT, Bootloader_SetLockState, xFlash.Status),
  
  NEW_TRANSACTION0(BOOT_TRY_WRITE, Bootloader_RESPONSE_DEFAULT, Bootloader_TryWrite, xFlash.Status),
  NEW_TRANSACTION0(BOOT_TRY_ERASE, Bootloader_RESPONSE_DEFAULT, Bootloader_TryErase, xFlash.Status),
  NEW_TRANSACTION1(BOOT_TRY_READ, Bootloader_RESPONSE_DEFAULT, Bootloader_TryRead),
  
  NEW_TRANSACTION0(BOOT_TRY_JUMP_TO_MAIN, Bootloader_RESPONSE_DEFAULT, Bootloader_TryJumpToMain, Bootloader.AppInfo),
  NEW_TRANSACTION0(BOOT_TRY_JUMP_TO_BOOT, Bootloader_RESPONSE_DEFAULT, Bootloader_TryJumpToBoot, Bootloader.AppInfo),
  
  NEW_TRANSACTION0(BOOT_TRY_RESET, Bootloader_RESPONSE_DEFAULT, Bootloader_TryReset, Bootloader.AppInfo),
  NEW_TRANSACTION0(BOOT_TRY_UPDATE_INFO, Bootloader_RESPONSE_DEFAULT, Bootloader_TryUpdateInfo, Bootloader.FirmwareInfo),
  //NEW_TRANSACTION(TRY_READ_CRC, Response_REQUEST_DEFAULT, ActionReadInfo, Bootloader.Info),
  { 0 }
};
//==============================================================================
