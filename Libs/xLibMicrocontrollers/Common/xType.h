/*
 * xType.h
 *
 * Created: 16.05.2019 14:29:38
 *  Author: rekuts
 */ 
//==============================================================================
#ifndef XTYPE_H_
#define XTYPE_H_
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
//==============================================================================
#include "Patterns/xPatterns.h"
//==============================================================================
typedef union{
  struct{
    uint64_t ExponentSign : 1;
    uint64_t Exponent : 11;
    uint64_t Mantisa : 52;
  };
  double Value;
	
} xDoubleT;
//==============================================================================
enum RESULTS
{
  RESULT_ACCEPT = 0,
  RESULT_ERROR_DATA,
  RESULT_ERROR_CONTENT_SIZE,
  RESULT_ERROR_REQUEST,
  RESULT_ERROR_RESOLUTION,
  RESULT_UNKNOWN_COMMAND,
  RESULT_BUSY,
  RESULT_OUTSIDE,
  RESULT_ERROR_ACTION
};
//==============================================================================
typedef void* xObject;
//==============================================================================
typedef xObject (*xEvt)(xObject Obj);
typedef struct { xObject obj; uint16_t key; } xContext;
typedef struct { xObject obj; uint16_t size; xObject next; } xContent;
//==============================================================================
//------------------------------------------------------------------------------
typedef union
{
	struct
	{
		uint32_t StartKey : 8; //default '#'
		uint32_t Description : 16; //description of the purpose of the package - request, response, event, etc.
		uint32_t EndKey : 8; //default ':'
	};
	
	uint32_t Value;
	
} PacketIdentificatorT; //aligned for uint32_t
//------------------------------------------------------------------------------
typedef struct
{
	PacketIdentificatorT Identificator; //identifier for recognizing the package
	uint32_t DeviceKey; //unique key of the device, module. 0 - system commands
	
} PacketHeaderT; //aligned for uint32_t
//------------------------------------------------------------------------------
typedef struct
{
	uint32_t RequestId; //generated key - when receiving a response, must match the request key
	
	uint16_t ActionKey; //action(command) key
	uint16_t ContentSize; //size of nested data after packet info
	
} PacketInfoT; //aligned for uint32_t
//------------------------------------------------------------------------------
typedef struct
{
	PacketHeaderT Header; // format: [#][Description][:][DeviceKey]
	PacketInfoT Info; // format: [RequestId][ActionKey][ContentSize]
	//uint8_t Content[Info.ContentSize]
	//uint8_t EndPacketSymbol default('\r')
	
} PacketT; //array: [#][Description][:][DeviceKey][RequestId][ActionKey][ContentSize][uint8_t Content[ContentSize]][\r]
//==============================================================================
//------------------------------------------------------------------------------
#define PACKET_HEADER_IDENTIFICATOR_MASK 0xFF0000FFU
#define PACKET_HEADER_IDENTIFICATOR 0x2300003AU // format: "#{Description}:"
//------------------------------------------------------------------------------
#define PACKET_HEADER_DESCRIPTION_MASK 0x00FFFF00U // format: "#{Description}:"
#define PACKET_HEADER_DESCRIPTION_REQUEST 0x5251U
#define PACKET_HEADER_DESCRIPTION_RESPONSE 0x5253U
#define PACKET_HEADER_DESCRIPTION_EVENT 0x4554U
//==============================================================================
//------------------------------------------------------------------------------
#ifndef xResult
#define xResult int8_t
#endif
//------------------------------------------------------------------------------
#ifndef xResultAccept
#define xResultAccept 0
#endif
//------------------------------------------------------------------------------
#ifndef xResultError
#define xResultError -1
#endif
//==============================================================================
//------------------------------------------------------------------------------
#define RESPONSE_END_SIZE (sizeof(RESPONSE_END) / sizeof(RESPONSE_END[0]) - 1)
#define SIZE_STRING(str)(sizeof(str) - 1)
#define SIZE_ARRAY(array)(sizeof(array) / sizeof(array[0]))
//------------------------------------------------------------------------------
#define sizeof_str(str)(sizeof(str) / sizeof(str[0]) - 1)
#define sizeof_array(array)(sizeof(array) / sizeof(array[0]))
//==============================================================================
//------------------------------------------------------------------------------
#define OBJECT_DESCRIPTION\
  char* Description
//------------------------------------------------------------------------------
#define OBJECT_HEADER\
  OBJECT_DESCRIPTION;\
  void* Parent
//------------------------------------------------------------------------------
#define EVENT_TYPEDEF(Name, TAttachment, TContext)\
typedef struct{\
  OBJECT_ATTACHMENT(TAttachment);\
  TContext Context;\
}xEvent##Name##T
//------------------------------------------------------------------------------
#define EVENT_PATTERN(Name, TAttachment, TContext)\
typedef struct{\
  OBJECT_ATTACHMENT_PATTERN(TAttachment);\
  TContext;\
}xEvent##Name##T
//------------------------------------------------------------------------------
#define OBJECT_DEF(Name, TAttachment)\
typedef struct{\
  OBJECT_ATTACHMENT(TAttachment);\
  xObject Context;\
}xObject##Name##T
//==============================================================================
//------------------------------------------------------------------------------
static inline bool IS(xObject total, xObject request, uint16_t size)
{
  while(size)
  {
    size--;
    if(((uint8_t*)total)[size] != ((uint8_t*)request)[size])
		{
			return false;
		}
  }
  return true;
}
//==============================================================================
#endif /* XTYPE_H_ */
