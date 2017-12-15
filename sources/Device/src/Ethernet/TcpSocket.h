#pragma once
#include "defines.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Ethernet
 *  @{
 *  @defgroup TcpSocket
 *  @{
 */

static const int    DEFAULT_PORT = 7,
                    POLICY_PORT = 843;

class SocketTCP
{
public:
    static bool Init(void (*funcConnect)(), void (*funcReceiver)(const char *buffer, uint length));
    
    static bool Send(const char *buffer, int length);
    
    static void SendFormatString(char *format, ...);
};

void ETH_SendFormatString(char *format, ...);

extern bool gEthIsConnected;

/** @}  @}
 */
