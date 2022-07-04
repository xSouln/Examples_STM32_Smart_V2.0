//==============================================================================
#ifndef __FLOW_DIRECTOR_H
#define __FLOW_DIRECTOR_H
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
#include "MotorDriver/MotorDriver.h"
//==============================================================================
//#ifdef FLOW_DIRECTOR_USER_CONFIG
#include "FlowDirectorConfig.h" // override default macroses
//#endif
//------------------------------------------------------------------------------
#ifndef FlowDirectorResult
#define FlowDirectorResult int8_t
#endif
//------------------------------------------------------------------------------
#ifndef FlowDirectorResultAccept
#define FlowDirectorResultAccept 0
#endif
//------------------------------------------------------------------------------
#ifndef FlowDirectorResultError
#define FlowDirectorResultError -1
#endif
//------------------------------------------------------------------------------
#ifndef FlowDirectorResultBusy
#define FlowDirectorResultBusy -2
#endif
//------------------------------------------------------------------------------
#ifndef FlowDirectorResultNullPointer
#define FlowDirectorResultNullPointer -3
#endif
//------------------------------------------------------------------------------
#ifndef FlowDirectorResultInvalidParameter
#define FlowDirectorResultInvalidParameter -4
#endif
//==============================================================================
typedef struct
{
	char* Description;
	void* Parent;
	
	MotorDriverT Driver;
	
} FlowDirectorT;
//==============================================================================
FlowDirectorResult FlowDirectorInit(FlowDirectorT* adapter, void* parent);
//==============================================================================
#ifdef __cplusplus
}
#endif
//==============================================================================
#endif /* __L298_ADAPTER_H */
