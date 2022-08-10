/*
 * xThread.c
 *
 * Created: 29.05.2019 9:32:03
 *  Author: rekuts
 */ 
#include "xThread.h"
//==============================================================================
/*
void xThread(xThreadT *thread){    
  if(thread->State.HandlerIndex != thread->State.TotalIndex){
    thread->Handler.Update = true;
    
    if(thread->Requests[thread->State.HandlerIndex].Action != 0){
      thread->Requests[thread->State.HandlerIndex].Action(thread, &thread->Requests[thread->State.HandlerIndex]);
    }
    thread->State.HandlerIndex++;
    thread->State.HandlerIndex &= thread->State.SizeMask;
    
    thread->Handler.Update = false;
  }
}
//==============================================================================
int8_t xThreadAdd(xThreadT *thread, xThreadAction action, xObject object, uint16_t size, uint16_t key){
  thread->Handler.IsAdd = true;
  
  thread->Requests[thread->State.TotalIndex].Action = action;
  thread->Requests[thread->State.TotalIndex].Object = object;
  thread->Requests[thread->State.TotalIndex].ObjectSize = size;
  thread->Requests[thread->State.TotalIndex].ObjectKey = key;
  
  thread->State.TotalIndex++;
  thread->State.TotalIndex &= thread->State.SizeMask;
  
  thread->Handler.IsAdd = false;
  return 0;
}
*/
//==============================================================================
void xThread(xThreadT *thread)
{    
  //thread->Handler.Update = true;
    
  while(thread->Tasks.Count)
  {
    xThreadTaskT* task = thread->Tasks.Head->Value;
    xListRemoveAt(&thread->Tasks, 0);
    task->Action(thread, task);
    free(task);
  }
  
  //thread->Handler.Update = false;
}
//==============================================================================
void xThreadDispose(xThreadT *thread)
{    
  //thread->Handler.Update = true;
  xListElementT* element = thread->Tasks.Head;
  xListElementT* next;
  
  while(element)
  {
    next = element->Next;
    free(element->Value);
    free(element);
    element = next;
    thread->Tasks.Count--;
  }
  
  //thread->Handler.Update = false;
}
//==============================================================================
xThreadTaskT* xThreadAdd(xThreadT *thread, xThreadAction action, xObject object, uint16_t size, uint16_t key)
{
  thread->Handler.IsAdd = true;
  
  xThreadTaskT* request = calloc(1, sizeof(xThreadTaskT));
  
  if(request)
  {
    request->Action = action;
    request->Object = object;
    request->ObjectSize = size;
    request->ObjectKey = key;
    
    xListAdd(&thread->Tasks, request);
  }
  
  thread->Handler.IsAdd = false;
  return request;
}
//==============================================================================