/******************************************************************************
*
* packet_router.c - routes data packets from hardware interfaces to registered
*   members.
* Created on: Feb 13, 2017
* Copyright (c) notice
*
******************************************************************************/
#ifndef __PACKET_ROUTER_C__
#define __PACKET_ROUTER_C__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//Standard includes
  #include <stdint.h>
  #include <stdbool.h>
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>

//Project specific includes
  #include "proj_inc/project_config.h"
  #include "proj_inc/packet_router_config.h"
  /* Project specific include files here */

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  /* Utility include files here */

//Third party includes
  #include "ThirdParty_inc/osal.h"
  /* Third party include files here */

//Application includes
  #include "app_inc/packet_router.h"
  /* Application include files here */

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

uint8_t u8PacketIDTable[] = PKT_ID_TABLE;

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct PacketRouteClient
{
  uint8_t                    u8PacketId;
  void                    (* vPacketClientReceive)(uint8_t * pPacket, uint32_t u32Packet_Len);
  struct PacketRouteClient * pPrevClient;
  struct PacketRouteClient * pNextClient;
}PacketRouteClient_t, * pPacketRouteClient;

pPacketRouteClient pPacketClientRouteList = NULL;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
ERROR_CODE ePacket_Client_Register(uint8_t u8PacketId, void (* vPacketClientReceive)(uint8_t * pPacket, uint32_t u32Packet_Len))
{
  ERROR_CODE eEC = ER_FAIL;
  pPacketRouteClient pClient = pPacketClientRouteList;

  //First check if the client route list has 0 members.
  if(pClient == NULL)
  {
    //The list has zero members, begin by allocating the first member of the list
    pPacketClientRouteList = calloc(sizeof(PacketRouteClient_t), sizeof(uint8_t));
    if(pPacketClientRouteList != NULL)
    {
      pClient = pPacketClientRouteList;
      pClient->u8PacketId = u8PacketId;
      pClient->vPacketClientReceive = vPacketClientReceive;
    }
    else
    {
      eEC = ER_NOMEM;
    }

  }
  else
  {
    //Walk the client register list to determine that the client doesn't already exist in the list else add it to the list
    while(1)
    {
      //check if client & client parameters already exist in the client register list
      if((pClient->u8PacketId == u8PacketId) & (pClient->vPacketClientReceive == vPacketClientReceive))
      {
        //client & param already exists, break and return
        eEC = ER_OK;
        break;
      }
      else
      {
        //check if the last member of the list was located
        if(pClient->pNextClient == NULL)
        {
          //the end of the list was found, allocate a new member of the list
          pClient->pNextClient = calloc(sizeof(PacketRouteClient_t), sizeof(uint8_t));
          if(pClient->pNextClient != NULL)
          {
            pClient = pClient->pNextClient;
            pClient->u8PacketId = u8PacketId;
            pClient->vPacketClientReceive = vPacketClientReceive;
          }
          else
          {
            eEC = ER_NOMEM;
          }

          break;
        }
        else
        {
          pClient = pClient->pNextClient;
        }
      }
    }
  }

  eEC= ER_OK;

  return eEC;
}

ERROR_CODE ePacket_Route(uint8_t * pPacket, uint32_t u32Packet_Len)
{
  ERROR_CODE         eEC = ER_FAIL;
  uint16_t           u16RouteTableSize;
  uint16_t           u16Index = 0;
  uint8_t            u8PacketID = pPacket[0];
  pPacketRouteClient pClient = pPacketClientRouteList;

  //check if any clients are currently registered with the packet router
  if(pPacketClientRouteList != NULL)
  {
    u16RouteTableSize = sizeof(u8PacketIDTable);

    while(u16Index < u16RouteTableSize)
    {
      //check the packet ID table to determine if the received packet is to be accepted.
      if(u8PacketIDTable[u16Index] == u8PacketID)
      {
        while(1)
        {
          //Packet ID found, find a client that is interested in this packet.
          if(pClient->u8PacketId == u8PacketID)
          {
            pClient->vPacketClientReceive(pPacket, u32Packet_Len);
          }

          //check if there is another client in the list
          if(pClient->pNextClient != NULL)
          {
            pClient = pClient->pNextClient;
          }
          else
          {
            //set the client searching pointer back to the beginning of the client list.
            pClient = pPacketClientRouteList;
            //break from searching the client list
            break;
          }
        }
      }

      u16Index ++;
    }
  }
  else
  {
    //No clients registered with the packet router, packet router is not ready to route packets.
    eEC = ER_NOT_READY;
  }

  return eEC;
}

#endif //__PACKET_ROUTER_C__
