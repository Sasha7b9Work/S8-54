#include "defines.h"
#include "Log.h"
#include "ethernetif.h"
#include "main.h"
#include "Ethernet/TcpEthernet.h"
#include "Ethernet/TcpSocket.h"
#include "SCPI/SCPI.h"
#include "globals.h"
#include <lwip/init.h>
#include <lwip/ip_addr.h>
#include <lwip/netif.h>
#include <lwip/timeouts.h>
#include <netif/etharp.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Netif_Config();

static struct netif gnetif;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void FuncConnect()
{
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncReceiver(const char *buffer, uint length)
{
    SCPI::AddNewData((uint8 *)buffer, length);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Ethernet::Init()
{
    // Initilaize the LwIP stack
    lwip_init();

    // Configure the Network interface
    Netif_Config();

    SocketTCP::Init(FuncConnect, FuncReceiver);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Ethernet::Update(uint timeMS)
{
    uint time = HAL_GetTick();

    do 
    {
        CABLE_LAN_IS_CONNECTED = (HAL_GetTick() - gEthTimeLastEthifInput <= 1500) ? 1U : 0U;

        ethernetif_input(&gnetif);
        sys_check_timeouts();
    } while (HAL_GetTick() - time < timeMS);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Netif_Config()
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    // IP address default setting
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

    // add the network interface
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    // Registers the default network interface
    netif_set_default(&gnetif);

    if (netif_is_link_up(&gnetif))
    {
        // When the netif is fully configured this function must be called
        netif_set_up(&gnetif);
    }
    else
    {
        // When the netif link is down this function must be called
        netif_set_down(&gnetif);
    }

    // Set the link callback function, this function is called on change of link status
    netif_set_link_callback(&gnetif, ethernetif_update_config);
}
