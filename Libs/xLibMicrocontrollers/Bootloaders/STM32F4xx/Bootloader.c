/*
 * Bootloader.c
 *
 *  Created on: Dec 17, 2020
 *      Author: rekuts
 */
//==============================================================================
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "Bootloader.h"
#include "xTransaction.h"
//==============================================================================
uint8_t bootloader_read_buffer[BOOT_READ_BUFFER_SIZE];

EVENT_PATTERN(Response,
               xRxT* Rx,
               xTransactionT* Transaction);
//==============================================================================
int16_t Bootloader_TryWrite(xObject context, RequestWriteT* request, uint16_t object_size)
{
  if(request->StartAddress < BOOT_END_ADDRESS){ return BOOT_OUTSIDE; }
  if((request->StartAddress + request->DataSize) > APP_END_ADDRESS){ return BOOT_OUTSIDE; }
  if(object_size - sizeof(RequestWriteT) < request->DataSize){ return BOOT_ERROR_REQUEST; }
  
  uint8_t* data = (uint8_t*)request;
  data += sizeof(RequestWriteT);
  
  return xFlashWrite(request->StartAddress, data, request->DataSize, 100);
}
//==============================================================================
int16_t Bootloader_TryRead(xEventBaseT* event, RequestReadT* request, uint16_t object_size)
{
  ((xTransactionT*)event->Context)->Content.size = 0;
   
  if(request->DataSize > BOOT_READ_BUFFER_SIZE){ return BOOT_OUTSIDE; }
  if((request->StartAddress + request->DataSize) > APP_END_ADDRESS){ return BOOT_OUTSIDE; }
  
  xFlashRead(request->StartAddress, bootloader_read_buffer, request->DataSize);
  
  ((xTransactionT*)event->Context)->Content.obj = bootloader_read_buffer;
  ((xTransactionT*)event->Context)->Content.size = request->DataSize;
  
  return BOOT_ACCEPT;
}
//==============================================================================
int16_t Bootloader_TryErase(xObject context, RequestEraseT* request, uint16_t object_size)
{
  if(request->StartAddress < BOOT_END_ADDRESS){ return BOOT_OUTSIDE; }
  if(request->StartAddress > request->EndAddress){ return BOOT_ERROR_ACTION; }
  return xFlashErasePages(request->StartAddress, request->EndAddress, 2000);
}
//==============================================================================
int16_t Bootloader_SetLockState(xObject context, uint8_t* request)
{  
  return xFlashSetLock(*request > 0);
}
//==============================================================================
int16_t Bootloader_TryJumpToMain(xObject context)
{
  if(!Bootloader.AppInfo.Status.BootIsEnable){ return BOOT_ERROR_RESOLUTION; }  
  Bootloader.AppInfo.Status.JumpToMain = true;
  
  return BOOT_ACCEPT;
}
//==============================================================================
int16_t Bootloader_TryUpdateInfo(xObject context)
{
  xFlashRead(BOOT_END_ADDRESS, &Bootloader.FirmwareInfo, sizeof(Bootloader.FirmwareInfo));
  
  Bootloader.AppInfo.Crc = xFlashGetCrc(Bootloader.FirmwareInfo.StartAddress, Bootloader.FirmwareInfo.EndAddress);
  Bootloader.AppInfo.Status.AppCrcError = Bootloader.AppInfo.Crc != Bootloader.FirmwareInfo.Crc;
  Bootloader.AppInfo.AppStartAddress = Bootloader.FirmwareInfo.StartAddress;
  Bootloader.AppInfo.AppEndAddress = Bootloader.FirmwareInfo.EndAddress;

  Bootloader.AppInfo.Status.AppErrorDiscrepancyAddress = APP_START_ADDRESS != Bootloader.FirmwareInfo.StartAddress;
  Bootloader.AppInfo.Status.AppErrorStartAddress = Bootloader.FirmwareInfo.StartAddress == -1;
  
  if(Bootloader.AppInfo.Status.AppCrcError) { return BOOT_ERROR_CRC; }
  if(Bootloader.AppInfo.AppStartAddress != APP_START_ADDRESS) { return BOOT_ERROR_ADDRESS; }
  return BOOT_ACCEPT;
}
//==============================================================================
int16_t Bootloader_ReadCrc(xObject context, RequestEraseT* request, uint16_t object_size)
{
  if(request->StartAddress > APP_END_ADDRESS){ return BOOT_OUTSIDE; }
  if(request->StartAddress > request->EndAddress){ return BOOT_ERROR_ACTION; }
  
  uint16_t crc = xFlashGetCrc(request->StartAddress, request->EndAddress);
  
  return BOOT_ACCEPT;
}
//==============================================================================
int16_t Bootloader_TryReset(xObject context)
{
  Bootloader.AppInfo.Status.Reset = true;
  return BOOT_ACCEPT;
}
//==============================================================================
int16_t Bootloader_TryJumpToBoot(xObject context)
{
  if(Bootloader.AppInfo.Status.BootIsEnable){ return BOOT_ERROR_RESOLUTION; }
  Bootloader.AppInfo.Status.JumpToBoot = true;
  
  xFlashSetLock(false);
  
  FirmwareInfoT FirmwareInfo;
  xFlashRead(BOOT_END_ADDRESS, &FirmwareInfo, sizeof(FirmwareInfo));
  
  FirmwareInfo.Requests.BootEnable = true;  
  
  xFlashErasePages(BOOT_END_ADDRESS, BOOT_END_ADDRESS + 1, 1000);
  xFlashWrite(BOOT_END_ADDRESS, &FirmwareInfo, sizeof(FirmwareInfo), 100);
  
  return BOOT_ACCEPT;
}
//==============================================================================
int16_t Bootloader_SetFirmwareInfo(FirmwareInfoT* info)
{
  if(!Bootloader.AppInfo.Status.BootIsEnable){ return BOOT_ERROR_RESOLUTION; }
  
  xFlashSetLock(false);
  xFlashErasePages(BOOT_END_ADDRESS, BOOT_END_ADDRESS + 1, 1000);
  xFlashWrite(BOOT_END_ADDRESS, info, sizeof(FirmwareInfoT), 100);
  xFlashSetLock(true);
  
  return BOOT_ACCEPT;
}
//==============================================================================
inline void Bootloader_MoveVector(uint32_t address)
{
  __set_PRIMASK(1);
  SCB->VTOR = (address);
  __set_PRIMASK(0);
}
//==============================================================================
inline void Bootloader_AppMainStart(uint32_t address)
{
  uint32_t app_jump_address = *(uint32_t*)(address + 4);  
  AppFuncT app_main = (AppFuncT)app_jump_address;
  
  __disable_irq();
  HAL_DeInit();
  
  __set_MSP(*(volatile uint32_t*)address);
  app_main();
  
  NVIC_SystemReset();
}
//==============================================================================
inline void Bootloader_Handler()
{
  if(Bootloader.AppInfo.Status.Reset)
  {
    Bootloader.AppInfo.Status.Reset = false;
    NVIC_SystemReset();
  }
  
  if(Bootloader.AppInfo.Status.JumpToBoot)
  {
    Bootloader.AppInfo.Status.JumpToBoot = false;
    NVIC_SystemReset();
  }
  
  if(Bootloader.AppInfo.Status.JumpToMain)
  {
    Bootloader.AppInfo.Status.JumpToMain = false;
    NVIC_SystemReset();
  }
}
//==============================================================================
BootloaderT Bootloader =
{
  .AppInfo =
  {
    .BootStartAddress = BOOT_START_ADDRESS,
    .BootEndAddress = BOOT_END_ADDRESS,
    
    .AppStartAddress = APP_START_ADDRESS,
    .AppEndAddress = APP_END_ADDRESS
  },
};
//==============================================================================
