//==============================================================================
#ifndef _X_FLASH_H
#define _X_FLASH_H
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
#include "Bootloader_Types.h"
//==============================================================================
#define XFLASH_LOCK true
#define XFLASH_UNLOCK (XFLASH_LOCK ^ true)
//==============================================================================
static uint32_t FLASH_PAGES[] =
{
  0x08000000,
  0x08004000,
  0x08008000,
  0x0800C000,
  0x08010000,
  0x08020000,
  0x08040000,
  0x08060000,
  0x08080000,
  0x080A0000,
  0x080C0000,
  0x080E0000,
  0x080FFFFF
};
//==============================================================================
enum xFlashOperationT
{
  FlashOperationFree,
  FlashOperationErase,
  FlashOperationWrite,
  FlashOperationRead,
  FlashOperationLock
};
//==============================================================================
typedef struct xFlashStatusT
{
  union
  {
    struct
    {
      uint32_t Write : 1;
      uint32_t Read : 1;
      uint32_t Erase : 1;
      uint32_t FlashUnlocked : 1;
      
      uint32_t Busy : 1;
    };
    uint32_t Value;
  };
  
  uint32_t WriteAddress;
  uint32_t EraseAddress;
  uint32_t ReadAddress;
  
  uint16_t Operation;
  uint16_t OperationTime;
  
  uint16_t OperationResult;
} xFlashStatusT;
//==============================================================================
typedef struct
{
  xFlashStatusT Status;
  volatile uint32_t OperationTimeout;
}xFlashT;
//==============================================================================
typedef struct
{
  uint16_t Key;
  uint16_t crc;
}xFlashNoteT;
//==============================================================================
extern xFlashT xFlash;
//==============================================================================
int8_t xFlashErasePages(uint32_t start_address, uint32_t end_address, uint32_t timeout);
int8_t xFlashSetLock(bool state);
uint16_t xFlashGetCrc(uint32_t start_address, uint32_t end_address);

int8_t xFlashWrite(uint32_t address, xObject data, uint16_t len, uint32_t timeout);
int8_t xFlashRead(uint32_t address, volatile xObject data, uint16_t len);

int8_t xFlashSaveObject(uint32_t address, xObject object, uint16_t size, uint32_t timeout);
//==============================================================================
#endif
