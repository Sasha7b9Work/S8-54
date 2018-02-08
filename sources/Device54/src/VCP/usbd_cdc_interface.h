#pragma once

#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

#include "usbd_cdc.h"

#pragma clang diagnostic warning "-Wpadded"
#pragma clang diagnostic warning "-Wc++98-compat-pedantic"



/* Periodically, the state of the buffer "UserTxBuffer" is checked.
   The period depends on CDC_POLLING_INTERVAL */
#define CDC_POLLING_INTERVAL             5 /* in ms. The max is 65 and the min is 1 */


extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;
