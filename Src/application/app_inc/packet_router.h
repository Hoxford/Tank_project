/******************************************************************************
*
* packet_router.h - include file for hardware interface data packet routing.
* Copyright (c) notice
*
******************************************************************************/
#ifndef __PACKET_ROUTER_H__
#define __PACKET_ROUTER_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#include "proj_inc/packet_router_config.h"
/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* Name: ePacket_Client_Register
* Description:
*   This function registers a client interested in hardware interface packets
*   with the packet routing table. The client can call this function at any
*   time that it wants to be routed a packet of a certain packet ID. The
*   client only needs to call this function once, ideally during setup, but the
*   function is reentrant. If this function finds that the calling client is
*   already in the routing table the request to register is ignored. A client
*   can also register multiple packet ID's to the same function or vice versa
*   depending on the application.
*
* Parameters:
*   (uint8_t) u8PacketId: The 8 bit packet ID that the client wants to be
*     notified of.
*   void (* vPacketClientReceive)(uint8_t * pPacket, uint32_t u32Packet_Len):
*     pointer to the client call back function to be called when a packet ID is
*     a match.
*
* Returns:
*   (ERROR_CODE): enum type error code.
*     = ER_FAIL: Failed to register the client with the packet router.
*     = ER_OK: Successfully registered the client with the packet router.
*
* Example:
* static const uint8_t u8FooPktID  0x46
* static const uint8_t u8OofPktID  0x4F
*
* void foo_callback(uint8_t * pPacket, uint32_t u32Packet_Len);
* void oof_callback(uint8_t * pPacket, uint32_t u32Packet_Len);
* void foof_callback(uint8_t * pPacket, uint32_t u32Packet_Len);
*
* void foo_callback(uint8_t * pPacket, uint32_t u32Packet_Len)
* {
*   //code
* }
*
* void oof_callback(uint8_t * pPacket, uint32_t u32Packet_Len)
* {
*   //code
* }
*
* void foof_callback(uint8_t * pPacket, uint32_t u32Packet_Len)
* {
*   //code
* }
*
* void foo(void)
* {
*   //register a single packet id to a callback function
*   ePacket_Client_Register(u8FooPktID, &foo_callback);
*   ePacket_Client_Register(u8OofPktID, &oof_callback);
*
*   //register multiple packet id's to the same callback function
*   ePacket_Client_Register(u8FooPktID, &foof_callback);
*   ePacket_Client_Register(u8OofPktID, &foof_callback);
* }
******************************************************************************/
ERROR_CODE ePacket_Client_Register(uint8_t u8PacketId, void (* vPacketClientReceive)(uint8_t * pPacket, uint32_t u32Packet_Len));

/******************************************************************************
*todo: Name: [put name here]
*todo: Description:
*   [put description here]
*
*todo: Parameters:
*   (type) name: description (in order from left to right)
*     = value(optional): description if value set
*     -or-
*     ->(type) name: description if member of struct
*       = value(optional): description if member of struct value set
*   example:
*   (bool) bVar: do action option
*     = false: do action when set to false
*     = true: do other action when set to true
*   (pPub_Example_Struct) pParam: pointer to the param structure
*     ->(int)iVar1: description
*     ->(int *)pVar2: description
*     ->(bool)bVar3: description
*     ->(PUB_EXAMPLE_ENUM)eENUM: description
*       = PUB_ENUM_A: do action when set to _A
*       = PUB_ENUM_B: do action when set to _B
*       = PUB_ENUM_C: do action when set to _C
*     ->void (* pCallback_Fcn)(void * param): Function pointer for callback.
*
*todo: Returns:
*   (type): description
*     = value (optional): value description
*   examples:
*   (bool):
*     = true: did function action and result is true
*     = false: did function action and result is false
*   (int): integer value description after function action
*   (pPub_Example_Struct):
*     = (uint32_t *): address of the created object
*     =             - NULL: created object fail
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE ePacket_Client_Unregister(uint8_t u8PacketId);

/******************************************************************************
*todo: Name: [put name here]
*todo: Description:
*   [put description here]
*
*todo: Parameters:
*   (type) name: description (in order from left to right)
*     = value(optional): description if value set
*     -or-
*     ->(type) name: description if member of struct
*       = value(optional): description if member of struct value set
*   example:
*   (bool) bVar: do action option
*     = false: do action when set to false
*     = true: do other action when set to true
*   (pPub_Example_Struct) pParam: pointer to the param structure
*     ->(int)iVar1: description
*     ->(int *)pVar2: description
*     ->(bool)bVar3: description
*     ->(PUB_EXAMPLE_ENUM)eENUM: description
*       = PUB_ENUM_A: do action when set to _A
*       = PUB_ENUM_B: do action when set to _B
*       = PUB_ENUM_C: do action when set to _C
*     ->void (* pCallback_Fcn)(void * param): Function pointer for callback.
*
*todo: Returns:
*   (type): description
*     = value (optional): value description
*   examples:
*   (bool):
*     = true: did function action and result is true
*     = false: did function action and result is false
*   (int): integer value description after function action
*   (pPub_Example_Struct):
*     = (uint32_t *): address of the created object
*     =             - NULL: created object fail
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE ePacket_Route(uint8_t * pPacket, uint32_t u32Packet_Len);

#endif //__PACKET_ROUTER_H__
