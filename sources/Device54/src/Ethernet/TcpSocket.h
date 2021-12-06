#pragma once
#include "defines.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Ethernet
 *  @{
 *  @defgroup TcpSocket
 *  @{
 */
 
 #define SOCKET_SEND(buffer, size) SocketTCP::Send(buffer, size)

static const int    DEFAULT_PORT = 7,
                    POLICY_PORT = 843;

class SocketTCP
{
public:
    static bool Init(void (*funcConnect)(), void (*funcReceiver)(const char *buffer, uint length));
    
    static bool Send(const char *buffer, int length);
};

void ETH_SendFormatString(char *format, ...);

#define LAN_IS_CONNECTED gEthIsConnected

extern bool gEthIsConnected;

/** @}  @}
 */
