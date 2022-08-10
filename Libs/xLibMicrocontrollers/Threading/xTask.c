//=================================================================================================
#include <string.h>
#include "xTask.h"
//=================================================================================================
void xTaskUpdate(xTaskLineT *TaskLine){
  if(TaskLine->Option.Count && !TaskLine->State.Pause){
    uint8_t i = 0;
    while(i < TaskLine->Option.MaxCount && TaskLine->Option.Count){
      if(TaskLine->Tasks[i].Func){
        if(!TaskLine->Tasks[i].MaxTic){
          TaskLine->Tasks[i].Func(TaskLine->Tasks[i].Object);
          TaskLine->Tasks[i].Func = 0;
          TaskLine->Option.Count--;
        }
        else{ TaskLine->Tasks[i].MaxTic--; }
      }
      i++;
    }
  }
}
//===================================================================================================
int8_t xTaskAdd(xTaskLineT *TaskLine, xEvt Func, xObject Object, uint32_t Time){
  if(TaskLine->Option.Count < TaskLine->Option.MaxCount && Time && Func) {
    xTaskT Task = { .MaxTic = Time, .Func = (xEvt)Func, .Object = Object };
    
    for(int i = 0; i < TaskLine->Option.MaxCount; i++){
      if(!TaskLine->Tasks[i].Func){
        TaskLine->Tasks[i] = Task;
        TaskLine->Option.LastIndex = i;
        TaskLine->Option.Count++;
        return i;
      }
    }
  }
  return -1;
}
//===================================================================================================
void xTaskRemove(xTaskLineT *TaskLine, xEvt Func){
  if(Func){
    for(uint8_t i = 0; i < TaskLine->Option.MaxCount; i++){
      if(TaskLine->Tasks[i].Func == Func){
        TaskLine->State.Pause = true;
        memset(&TaskLine->Tasks[i], 0, sizeof(xTaskT));
        if(TaskLine->Option.Count) { TaskLine->Option.Count--; }
        TaskLine->State.Pause = false;
        return;
      }
    }
  }
}
//===================================================================================================
int8_t xTaskPresense(xTaskLineT *TaskLine, xEvt Func){
  for(uint8_t i = 0; i < TaskLine->Option.MaxCount; i++) { if(TaskLine->Tasks[i].Func == Func) return i; }
  return -1;
}
//===================================================================================================
