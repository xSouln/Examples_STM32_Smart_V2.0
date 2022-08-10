//==============================================================================
#ifndef X_TASK_H
#define X_TASK_H
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
//==============================================================================
typedef struct{ uint16_t size; uint8_t *data; } xTaskCallbackT;
//==============================================================================
typedef struct TaskManagerType{
  uint32_t MaxTic;
  xEvt Func;
  xObject Object;
}xTaskT;
//==============================================================================
typedef struct xTaskOptionType{
  uint8_t LastIndex;
  uint8_t MaxCount;
  uint8_t Count;
}xTaskOptionT;
//==============================================================================
typedef union {
  struct{
    uint8_t Pause : 1;
  };
  uint8_t Value;
}xTaskStateT;
//==============================================================================
typedef struct TaskLineType{
  xTaskStateT State;
  xTaskOptionT Option;
  xTaskT *Tasks;
}xTaskLineT;
//==============================================================================
void xTaskUpdate(xTaskLineT *TaskLine);
int8_t xTaskAdd(xTaskLineT *TaskLine, xEvt Func, xObject Object, uint32_t Time);
void xTaskRemove(xTaskLineT *TaskLine, xEvt Func);
int8_t xTaskPresense(xTaskLineT *TaskLine, xEvt Func);
//==============================================================================
#endif
