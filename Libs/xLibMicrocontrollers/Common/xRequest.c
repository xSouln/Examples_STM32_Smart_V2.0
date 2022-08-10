//==============================================================================
#include <string.h>
#include "xRequest.h"
//==============================================================================
xRequestT* xRequestIdentify(xRxT* rx, xRequestT request[], uint8_t data[], uint16_t len)
{
  uint8_t i = 0;
  while(request[i].Header)
  {
    if(len >= request[i].HeaderLength)
    {
      if(memcmp(data, request[i].Header, request[i].HeaderLength) == 0)
      {
        data += request[i].HeaderLength;
        len -= request[i].HeaderLength;
        
        if (request[i].Action)
        {
          request[i].Action(rx, &request[i], data, len);
        }
        return &request[i];
      }
    }
    i++;
  }
  return 0;
}
//==============================================================================
