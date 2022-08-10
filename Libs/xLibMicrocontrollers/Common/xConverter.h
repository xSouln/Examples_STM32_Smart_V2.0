//==============================================================================
#ifndef X_CONVERTER_H
#define X_CONVERTER_H
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
//==============================================================================
#include "xTx.h"
//==============================================================================
#define X_STRING_LIST_MAX_SIZE 10
#define X_VALUE_LIST_MAX_SIZE 10
//==============================================================================
typedef struct{
  int32_t *Values;
  uint16_t Count;
}ValueListT;
//==============================================================================
typedef struct{
  uint8_t *ptr;
  uint16_t size;
}StringT;
//==============================================================================
typedef struct{
  StringT *Str;
  uint16_t Count;
}StringListT;
//==============================================================================
ValueListT xConverterStrGetValues(uint8_t *data, uint8_t size, char separator);
StringListT xConverterSeparate(uint8_t *ptr, uint16_t size, char* str2);

uint16_t xConverterStrRemoveStr(uint8_t *ptr, uint16_t size, char* str2);
uint32_t xConverterHexStrToUInt(uint8_t *data, uint8_t len);

uint8_t xConverterHex4(uint8_t byte);
//==============================================================================
//==============================================================================
#endif
