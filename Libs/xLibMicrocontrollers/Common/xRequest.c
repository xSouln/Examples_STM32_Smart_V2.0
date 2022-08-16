//==============================================================================
#include <string.h>
#include "xRequest.h"
//==============================================================================
xRequestT* xRequestIdentify(xRxT* rx, xRequestManagerT* manager, uint8_t data[], uint16_t data_size)
{
  uint8_t i = 0;
  while(manager->Requests[i].Header)
  {
    if(data_size >= manager->Requests[i].HeaderLength)
    {
      if(memcmp(data, manager->Requests[i].Header, manager->Requests[i].HeaderLength) == 0)
      {
        data += manager->Requests[i].HeaderLength;
        data_size -= manager->Requests[i].HeaderLength;
        
        if (manager->Requests[i].Action)
        {
					manager->FoundRequest = &manager->Requests[i];
					manager->Rx = rx;
					
          manager->Requests[i].Action(manager, data, data_size);
					
					manager->FoundRequest = 0;
					manager->Rx = 0;
        }
        return &manager->Requests[i];
      }
    }
    i++;
  }
  return 0;
}
//==============================================================================
xResult xRequestManagerInit(xRequestManagerT* manager, void* parent, void* device, xRequestT* requests)
{
	if (manager && device && requests)
	{
		manager->Description = "xRequestManagerT";
		manager->Parent = parent;
		manager->Device = device;
		manager->Requests = requests;
		
		return RESULT_ACCEPT;
	}
	
	return RESULT_ERROR;
}
//==============================================================================
