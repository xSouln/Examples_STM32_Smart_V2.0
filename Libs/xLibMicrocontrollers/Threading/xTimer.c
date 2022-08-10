/*
 * Timers.c
 *
 * Created: 12.03.2020 18:33:54
 *  Author: rekuts
 */
//==============================================================================
#include "xTimer.h"
//==============================================================================
TimerT Timer;
//==============================================================================
inline void xTimerTick()
{
  Timer.Events.Time1ms = true;
  if(++Timer.Counters.Time10ms == 10){ Timer.Counters.Time10ms = 0; Timer.Events.Time10ms = true; }
	if(++Timer.Counters.Time100ms == 100){ Timer.Counters.Time100ms = 0; Timer.Events.Time100ms = true; }
	if(++Timer.Counters.Time1000ms == 1000){ Timer.Counters.Time1000ms = 0; Timer.Events.Time1000ms = true; }
}
//==============================================================================
inline void xTimerDecrement(xTimerT* timer)
{
  xListElementT* element = timer->Tasks.Head;
  //timer->Handler.Decrement = true;
  
  while(element)
  {
    xTimerTaskT* task = element->Value;    
    if(task->Retention && task->State.Enable){ task->Retention--; }
    element = element->Next;
  }
  
  //timer->Handler.Decrement = false;
}
//==============================================================================
inline void xTimer(xTimerT* timer)
{
  xListElementT* element = timer->Tasks.Head;
  //timer->Handler.Update = true;
  
  while(element)
  {
    xTimerTaskT* task = element->Value;
    
    if(task->Action && !task->Retention)
    {
      task->Action(timer, task);
      
      if(task->Period)
      {
        task->Retention = task->Period;
      }      
      else
      {
        element = xListRemoveElement(&timer->Tasks, element);
        free(task);
        goto end_while;
      }
    }
    element = element->Next;
  end_while:;
  }
}
//==============================================================================
inline void xTimerDispose(xTimerT* timer)
{
  xListElementT* element = timer->Tasks.Head;
  xListElementT* next;
  
  while(element)
  {
    next = element->Next;
    free(element->Value);
    free(element);
    element = next;
    timer->Tasks.Count--;
  }
}
//==============================================================================
xTimerTaskT* xTimerAdd(xTimerT* timer, xTimerAction action, uint32_t retention, uint32_t period){
  //timer->Handler.Add = true;
  
  xTimerTaskT* task = calloc(1, sizeof(xTimerTaskT));
  if(task)
  {
    task->Retention = retention;
    task->Period = period;
    task->Action = action;
    xListAdd(&timer->Tasks, task);
  }
  //timer->Handler.Add = false;
  return task;
}
//==============================================================================