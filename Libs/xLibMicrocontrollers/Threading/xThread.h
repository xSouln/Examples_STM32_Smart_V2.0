/*
 * xThread.h
 *
 * Created: 29.05.2019 9:35:06
 *  Author: rekuts
 */ 
#ifndef XTHREAD_H_
#define XTHREAD_H_
//==============================================================================
#include <stdlib.h>
#include <string.h>
#include "xType.h"
#include "xList.h"
//==============================================================================
typedef struct {
  xObject Object;
  uint16_t Size;
  uint16_t Key;
} xThreadContentT;
//==============================================================================
typedef void (*xThreadAction)(xObject thread, xObject request);
//==============================================================================
typedef struct {
  xThreadAction Action;
  xObject Object;
  uint16_t ObjectSize;
  uint16_t ObjectKey;
} xThreadTaskT;
//==============================================================================
typedef struct{
  uint8_t SizeMask;
  uint8_t TotalIndex;
  uint8_t HandlerIndex;
}xThreadStateT;
//==============================================================================
typedef union{
  struct{
    uint8_t Update : 1;
    uint8_t Pause : 1;
    uint8_t IsAdd : 1;
  };
  uint8_t State;
}xThreadHandlerT;
//==============================================================================
typedef struct{
  volatile xThreadHandlerT Handler;
  //xThreadStateT State;
  uint16_t Id;
  xListT Tasks;
  
  //void (*Activate)();
  //xThreadRequestT *Requests;  
}xThreadT;
//==============================================================================
#define THREAD_INIT(name, id, size_mask, activate)\
xThreadTaskT Thread##name##Tasks[size_mask + 1];\
xThreadT Thread##name = {\
  .Id = id,\
  .State = { .SizeMask = size_mask },\
  .Tasks = Thread##name##Tasks,\
  .Activate = activate\
}
//==============================================================================
void xThread(xThreadT *thread);
void xThreadDispose(xThreadT *thread);
xThreadTaskT* xThreadAdd(xThreadT *thread, xThreadAction action, xObject object, uint16_t size, uint16_t key);
//==============================================================================
#endif /* XTHREAD_H_ */
