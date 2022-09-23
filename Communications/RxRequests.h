/*
 *
 */
#ifndef RX_REQUESTS_H
#define RX_REQUESTS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "Common/xTx.h"
#include "Common/xRxTransaction.h"
#include "Common/xRxReceiver.h"
//==============================================================================
static const char RECEIVER_REQUEST_GET_FIRMWARE[] = "get firmware";

extern const xRxRequestT RxRequests[];
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* RECEIVER_REQUESTS_H */
