//==============================================================================
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//==============================================================================
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_flash.h"
#include "xFlash.h"
#include "Bootloader.h"
//==============================================================================
uint32_t xflash_start_address;
uint32_t xflash_end_address;
uint32_t xflash_sector;
//==============================================================================
int8_t xFlashErasePages(uint32_t start_address, uint32_t end_address, uint32_t timeout)
{
  uint32_t sector = 0;
  xFlash.Status.Operation = FlashOperationErase;
  xFlash.OperationTimeout = timeout;
  xFlash.Status.OperationTime = 0;
  xFlash.Status.OperationResult = BOOT_ACCEPT;
  xFlash.Status.Erase = true;
  
  xflash_start_address = start_address;
  xflash_end_address = end_address;
  
  if(!xFlash.Status.FlashUnlocked) { xFlash.Status.OperationResult = BOOT_LOCKED; goto end; }
  
  while(FLASH_PAGES[sector] < start_address && sector < SIZE_ARRAY(FLASH_PAGES) - 2)
  {
    sector++;
  }
  
  xflash_sector = sector;
  
  while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
  {
    if(!xFlash.OperationTimeout){ xFlash.Status.OperationResult = BOOT_TIMEOUT; goto end; }
  }
  
  if(READ_BIT(FLASH->SR, FLASH_SR_EOP)) { WRITE_REG(FLASH->SR, FLASH_SR_EOP); }
  
  while(start_address < end_address && sector < SIZE_ARRAY(FLASH_PAGES) - 1)
  {
    CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
    FLASH->CR |= FLASH_PSIZE_WORD;
    CLEAR_BIT(FLASH->CR, FLASH_CR_SNB);
    FLASH->CR |= FLASH_CR_SER | (sector << FLASH_CR_SNB_Pos);
    FLASH->CR |= FLASH_CR_STRT;
    
    while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
    {
      if(!xFlash.OperationTimeout){ xFlash.Status.OperationResult = BOOT_TIMEOUT; goto end; }
    }
    
    if(READ_BIT(FLASH->SR, FLASH_SR_EOP)) { WRITE_REG(FLASH->SR, FLASH_SR_EOP); }
    
    CLEAR_BIT(FLASH->CR, (FLASH_CR_SER | FLASH_CR_SNB));
    
    FLASH_FlushCaches();
    
    xFlash.Status.EraseAddress = start_address;
    start_address += (FLASH_PAGES[sector + 1] - FLASH_PAGES[sector]);
    sector++;
  }
  
  end:;
  CLEAR_BIT(FLASH->CR, (FLASH_CR_SER | FLASH_CR_SNB));
  FLASH_FlushCaches();
  xFlash.Status.OperationTime = timeout - xFlash.OperationTimeout;
  xFlash.Status.Erase = false;
  return xFlash.Status.OperationResult;
}
//==============================================================================
int8_t xFlashWrite(uint32_t address, xObject data, uint16_t len, uint32_t timeout)
{
  uint16_t i = 0;
  uint8_t* ptr = (uint8_t*)data;
  xFlash.Status.Operation = FlashOperationWrite;
  xFlash.OperationTimeout = timeout;
  xFlash.Status.OperationTime = 0;
  xFlash.Status.OperationResult = BOOT_ACCEPT;
  xFlash.Status.Write = true;
  
  if(!xFlash.Status.FlashUnlocked) { xFlash.Status.OperationResult = BOOT_LOCKED; goto end; }
  
  while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
  {
    if(!xFlash.OperationTimeout){ xFlash.Status.OperationResult = BOOT_TIMEOUT; goto end; }
  }
  
  if(READ_BIT(FLASH->SR, FLASH_SR_EOP)) { WRITE_REG(FLASH->SR, FLASH_SR_EOP); }
  
  CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
  FLASH->CR |= FLASH_PSIZE_WORD;
  FLASH->CR |= FLASH_CR_PG;
  
  while(i < len && address < APP_END_ADDRESS)
  {
    uint32_t value;
    
    value = ptr[i + 3];
    value <<= 8;
    
    value |= ptr[i + 2];
    value <<= 8;
    
    value |= ptr[i + 1];
    value <<= 8;
    
    value |= ptr[i];
    
    xFlash.Status.WriteAddress = address;
    *(volatile uint32_t*)(address) = value;
    
    address += sizeof(value);
    i += sizeof(value);
    
    while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
    {
      if(!xFlash.OperationTimeout){ xFlash.Status.OperationResult = BOOT_TIMEOUT; goto end; }
    }
    WRITE_REG(FLASH->SR, FLASH_SR_EOP);
  }
  
  end:;
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);  
  xFlash.Status.OperationTime = timeout - xFlash.OperationTimeout;
  xFlash.Status.Write = false;
  return xFlash.Status.OperationResult;
}
//==============================================================================
int8_t xFlashRead(uint32_t address, volatile xObject data, uint16_t len)
{
  volatile uint8_t* mem = (volatile uint8_t*)address;
  volatile uint8_t* out = (volatile uint8_t*)data;
  uint16_t i = 0;
  xFlash.Status.Read = true;
  while(i < len && address < APP_END_ADDRESS)
  {
    out[i] = mem[i];
    address++;
    i++;
  }
  xFlash.Status.Read = false;
  return BOOT_ACCEPT;
}
//==============================================================================
uint16_t xFlashGetCrc(uint32_t start_address, uint32_t end_address)
{
  uint16_t crc = 0;
  xFlash.Status.Read = true;
  while(start_address < end_address && start_address < APP_END_ADDRESS)
  {
    crc += *(volatile uint8_t*)start_address;
    start_address++;
  }
  xFlash.Status.Read = false;
  return crc;
}
//==============================================================================
int8_t xFlashSetLock(bool state)
{
  xFlash.Status.FlashUnlocked = state == XFLASH_UNLOCK;
  xFlash.Status.Operation = FlashOperationLock;
  xFlash.Status.OperationTime = 0;
  xFlash.Status.OperationResult = BOOT_ACCEPT;
  
  if(state == XFLASH_LOCK)
  {
    SET_BIT(FLASH->CR, FLASH_CR_LOCK);
  }
  else if(SET_BIT(FLASH->CR, FLASH_CR_LOCK))
  {
    WRITE_REG(FLASH->KEYR, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR, FLASH_KEY2);
  }
  
  return xFlash.Status.OperationResult;
}
//==============================================================================
int8_t xFlashSaveObject(uint32_t address, xObject object, uint16_t size, uint32_t timeout)
{
  int8_t result = xFlashSetLock(XFLASH_UNLOCK);
  if(result != BOOT_ACCEPT) { goto end; }
  
  result = xFlashErasePages(address, address + 1, timeout);
  if(result != BOOT_ACCEPT) { goto end; }
  
  result = xFlashWrite(address, object, size, timeout);
  if(result != BOOT_ACCEPT) { goto end; }
  
  end:;
  xFlashSetLock(XFLASH_LOCK);
  return result;
}
//==============================================================================
xFlashT xFlash;
//==============================================================================
