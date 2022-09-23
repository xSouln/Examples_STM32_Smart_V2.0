//==============================================================================
#include <string.h>
#include "xRxTransaction.h"
//==============================================================================
xRxTransactionT* xRxTransactionIdentify(xRxTransactionT* transaction, uint16_t key)
{
  while(transaction && transaction->Id != (uint16_t)-1)
  {
    if(transaction->Id == key)
    {
      return transaction;
    }
    transaction++;
  }
  
  return 0;
}
//==============================================================================
