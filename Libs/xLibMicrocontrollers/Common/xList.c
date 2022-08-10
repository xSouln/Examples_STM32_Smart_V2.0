//=================================================================================================================================
#include <string.h>
#include "xList.h"
//=================================================================================================================================
xListElementT* xListAdd(xListT *list, xObject object)
{
  xListElementT *element = calloc(1, sizeof(xListElementT));
  if(element)
  {
    element->Value = object;
    
    if(!list->Head) { list->Head = element; }
    else { list->Tail->Next = element; }
    
    list->Tail = element;
    list->Count++;
  }
  return element;
}
//=================================================================================================================================
xListElementT* xListInsert(xListT *list, uint16_t index, xObject object)
{
  xListElementT* request = 0;
  if(index <= list->Count)
  {
    request = calloc(1, sizeof(xListElementT));
    
    xListElementT* previous = 0;
    xListElementT* total = list->Head;
    
    request->Value = object;
    
    while(index)
    {
      previous = total;
      total = total->Next;
      index--;
    }
    
    if(!previous){ list->Head = request; }
    else { previous->Next = request; }
    
    request->Next = total;
    if(!total) { list->Tail = request; }
    
    list->Count++;
  }
  return request;
}
//=================================================================================================================================
xListElementT* xListRemoveAt(xListT *list, uint16_t index){
  if(index < list->Count)
  {
    xListElementT* previous = 0;
    xListElementT* total = list->Head;
    xListElementT* result;
    
    while(index)
    {
      previous = total;
      total = total->Next;
      index--;
    }
    
    result = total->Next;
    
    if(!previous) { list->Head = result; }
    else { previous->Next = result; }
    
    if(!result){ list->Tail = previous; }
    
    free(total);
    list->Count--;
    
    return result;
  }
  return 0;
}
//=================================================================================================================================
xListElementT* xListRemoveElement(xListT *list, xListElementT *element)
{
  if(!list->Count){ return 0; }
  
  xListElementT* previous = 0;
  xListElementT* total = list->Head;  
  
  while(total && total != element)
  {
    previous = total;
    total = total->Next;
  }
  
  if(!total) { return 0; }
  
  if(!previous) { list->Head = total->Next; }
  else { previous->Next = total->Next; }
  
  if(!total->Next){ list->Tail = previous; }
  
  free(element);
  list->Count--;
    
  return total->Next;
}
//=================================================================================================================================
xListElementT* xListGetElement(xListT *list, uint16_t index)
{
  xListElementT *element = 0;
  if(index < list->Count)
  {
    element = list->Head;
    while(index)
    {
      element = element->Next;
      index--;
    }
  }
  return element;
}
//=================================================================================================================================
