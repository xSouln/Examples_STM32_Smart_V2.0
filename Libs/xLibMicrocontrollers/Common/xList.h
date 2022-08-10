//=================================================================================================================================
#ifndef X_LIST_H
#define X_LIST_H
//=================================================================================================================================
#include <stdlib.h>
#include "xType.h"
//=================================================================================================================================
typedef struct xListElementT
{
  xObject Value;
  xObject Next;
}xListElementT;

typedef struct xListT
{
  xListElementT* Head;
  xListElementT* Tail;
  uint16_t Count;
}xListT;

xListElementT* xListAdd(xListT *list, xObject object);
xListElementT* xListInsert(xListT *list, uint16_t index, xObject object);
xListElementT* xListRemoveElement(xListT *list, xListElementT *element);
xListElementT* xListRemoveAt(xListT *list, uint16_t index);

xListElementT* xListGetElement(xListT *list, uint16_t index);
//=================================================================================================================================
#endif
