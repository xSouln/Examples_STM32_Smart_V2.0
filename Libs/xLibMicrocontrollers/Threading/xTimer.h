 /*
 * Timers.h
 *
 * Created: 12.03.2020 18:34:12
 *  Author: rekuts
 */ 
#ifndef _XTIMER_H_
#define _XTIMER_H_
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
#include "xList.h"
//==============================================================================
typedef void (*xTimerAction)(xObject context, xObject task);
//==============================================================================
typedef union
{
  struct
  {
    uint16_t Enable : 1;
  };
  uint16_t Value;
	
} xTimerTaskStateT;
//==============================================================================
typedef struct
{
  xTimerAction Action;
  uint32_t Retention;
  uint32_t Period;
  
  xObject Object;
  
  uint16_t Key;  
  
  xTimerTaskStateT State;
	
} xTimerTaskT;
//==============================================================================
typedef union
{
  struct
  {
    uint16_t Add : 1;
    uint16_t Update : 1;
    uint16_t Decrement : 1;
    uint16_t Pause : 1;
  };
  uint16_t Value;
	
} xTimerHandlerT;
//==============================================================================
typedef struct
{
  uint8_t SizeMask;
  uint8_t TotalIndex;
  uint8_t HandlerIndex;
	
} xTimerStateT;
//==============================================================================
typedef struct
{
  volatile xTimerHandlerT Handler;
  uint16_t Id;
  
  xListT Tasks;
  //uint16_t RequestCount;
}xTimerT;
//==============================================================================
#define TIMER_INIT(name, id, count)\
xTimerTaskT Timer##name##Tasks[count];\
xTimerT Timer##name = {\
  .Id = id,\
  .RequestCount = count,\
  .Tasks = Timer##name##Tasks\
}
//==============================================================================
extern inline void xTimerDecrement(xTimerT* timer);
extern inline void xTimerDispose(xTimerT* timer);
extern inline void xTimer(xTimerT* timer);
xTimerTaskT* xTimerAdd(xTimerT* timer, xTimerAction action, uint32_t retention, uint32_t period);
extern inline void xTimerTick();
//==============================================================================
typedef struct
{
  uint8_t Time1ms : 1;
  uint8_t Time10ms : 1;
  uint8_t Time100ms : 1;
  uint8_t Time1000ms : 1;
  uint8_t Time1min : 1;
	
} TimerEventT;
//==============================================================================
typedef struct
{
  uint8_t Time10ms;
  uint8_t Time100ms;
  uint16_t Time1000ms;
  uint16_t Time1min;
	
} TimersCountersT;
//==============================================================================
typedef struct
{
  TimerEventT Events;
  TimersCountersT Counters;
	
} TimerT;
//==============================================================================
extern TimerT Timer;
//==============================================================================
#endif /* TIMERS_H_ */
