//=================================================================================================================================
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//=================================================================================================================================
#include "xConverter.h"
//=================================================================================================================================
const uint8_t ByteCountStrUint32 = 10;
//=================================================================================================================================
const char *HexTable[] = {
"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f", "10", "11",
"12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f", "20", "21", "22", "23",
"24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f", "30", "31", "32", "33", "34", "35",
"36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f", "40", "41", "42", "43", "44", "45", "46", "47",
"48", "49", "4a", "4b", "4c", "4d", "4e", "4f", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
"5a", "5b", "5c", "5d", "5e", "5f", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b",
"6c", "6d", "6e", "6f", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d",
"7e", "7f", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f", "a0", "a1",
"a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af", "b0", "b1", "b2", "b3",
"b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf", "c0", "c1", "c2", "c3", "c4", "c5",
"c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
"d8", "d9", "da", "db", "dc", "dd", "de", "df", "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9",
"ea", "eb", "ec", "ed", "ee", "ef", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb",
"fc", "fd", "fe", "ff"
};
//=================================================================================================================================
//=================================================================================================================================
StringT xstring[X_STRING_LIST_MAX_SIZE];
StringListT xstring_list = { .Str = xstring };

int32_t xvalues[X_VALUE_LIST_MAX_SIZE];
ValueListT xvalue_list = { .Values = xvalues };
//=================================================================================================================================
//=================================================================================================================================
int32_t xConverterStrToInt32(uint8_t *data, uint8_t data_leng){
  int32_t volume = 0;
  uint8_t byts = 4;
  uint8_t byte_number = 0;
  
  while(byts > 0 && byte_number < data_leng){
    volume <<= 8;
    volume |= data[byte_number];
    byte_number++;
    byts--;
  }
  return volume;
}
//=================================================================================================================================
ValueListT xConverterStrGetValues(uint8_t *data, uint8_t size, char separator){
  xvalue_list.Count = 0;
  uint8_t last_index = 0;
  
  for(uint16_t i = 0; i < size; i++){
    if(data[i] == separator){
      data[i] = '\0';
      xvalue_list.Values[xvalue_list.Count] = atoi((char*)(data + last_index));
      
      last_index = i + 1;
      if(++xvalue_list.Count == X_VALUE_LIST_MAX_SIZE) { return xvalue_list; }
    }
  }
  
  data[size] = '\0';
  xvalue_list.Values[xvalue_list.Count] = atoi((char*)(data + last_index));
  xvalue_list.Count++;
  
  return xvalue_list;
}
//=================================================================================================================================
void MasPutVolume4Byte(uint8_t *MasPtr, uint64_t Vol){
  MasPtr[0] = (Vol & 0xff000000) >> 24;
  MasPtr[1] = (Vol & 0x00ff0000) >> 16;
  MasPtr[2] = (Vol & 0x0000ff00) >> 8;
  MasPtr[3] = (Vol & 0x000000ff);
}
//=================================================================================================================================
void MasPutVolume4ByteStrHex(uint8_t *MasPtr, uint64_t Vol)
{
  MasPtr[0] = (Vol & 0xff000000) >> 24;
  MasPtr[1] = (Vol & 0x00ff0000) >> 16;
  MasPtr[2] = (Vol & 0x0000ff00) >> 8;
  MasPtr[3] = (Vol & 0x000000ff);
}
//=================================================================================================================================
uint8_t xConverterHex4(uint8_t byte)
{
  byte &= 0x0F;
  if(byte < 10) { byte += 0x30; }
  else { byte += 0x37; }
  return byte;
}
//=================================================================================================================================
//=================================================================================================================================
uint32_t xConverterHexStrToUInt(uint8_t *data, uint8_t len)
{
  uint32_t result = 0;
  uint8_t byte;
  
  if(len >= 2 && data[0] == '0')
  {
    if(data[1] == 'x' || data[1] == 'X') { data += 2; len -= 2; }
  }
  
  for(uint8_t i = 0; i < len; i++)
  {
    byte = data[i];
    if (byte >= '0' && byte <= '9') { byte = byte - '0'; }
    else if (byte >= 'a' && byte <='f') { byte = byte - 'a' + 10; }
    else if (byte >= 'A' && byte <='F') { byte = byte - 'A' + 10; }
    result = (result << 4) | (byte & 0xF);
  }
  return result;
}
//=================================================================================================================================
uint16_t xConverterStrRemoveStr(uint8_t *ptr, uint16_t size, char* str2){
  uint8_t SequencMaxCount = sizeof(str2);
  uint8_t SequencCount;
  uint16_t LastIndex = 0;  
  uint16_t i = 0;
  while(i < size){
    if(ptr[i] == str2[SequencCount]){
      if(++SequencCount == SequencMaxCount){
        SequencCount = 0;
        LastIndex = i - (SequencMaxCount - 1);
        for(int j = LastIndex + SequencMaxCount; j < size; j++){
          ptr[j - SequencMaxCount] = ptr[j];
        }
        i = 0;
        size -= SequencMaxCount;
      }
      else i++;
    }
    else {
      if(SequencCount) SequencCount = 0;
      i++;
    }    
  }
  return size;
}
//=================================================================================================================================
StringListT xConverterSeparate(uint8_t *ptr, uint16_t size, char* str2){  
  uint8_t SequencMaxCount = strlen(str2);
  uint8_t SequencCount = 0;
  
  xstring_list.Str[0].ptr = ptr;
  xstring_list.Str[0].size = 0;
  
  for(int i = 0; i < size; i++){
    xstring_list.Str[xstring_list.Count].size++;    
    if(ptr[i] == str2[SequencCount]){
      if(++SequencCount == SequencMaxCount){
        SequencCount = 0;      
        if(xstring_list.Str[xstring_list.Count].size > SequencMaxCount){
          xstring_list.Str[xstring_list.Count].size -= SequencMaxCount;        
          xstring_list.Count++;
          
          xstring_list.Str[xstring_list.Count].size = 0;
          if(i + 1 < size) xstring_list.Str[xstring_list.Count].ptr = ptr + i + 1;
        }
        else {
          xstring_list.Str[xstring_list.Count].ptr += SequencMaxCount;
          xstring_list.Str[xstring_list.Count].size = 0;
        }
      }
    }
    else if(SequencCount) SequencCount = 0;
  }  
  if(xstring_list.Str[xstring_list.Count].size) xstring_list.Count++;

  return xstring_list;
}
//=================================================================================================================================
