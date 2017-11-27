/******************************************************************************
*
* commander.c - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __COMMANDER_C__
#define __COMMANDER_C__
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
#if (PROJ_CONFIG_USE_APP_COMMANDER == 0)
  //commander not implemented
#else

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  /* Utility include files here */

//Third party includes
  #include "ThirdParty_inc/osal.h"
  /* Third party include files here */

//Driver includes
  #include "drivers_inc/wifi.h"
  #include "drivers_inc/bluetooth.h"
  #include "drivers_inc/bluetooth_HC-05.h"
  #include "drivers_inc/nvram.h"
  /* Driver include files here */

//Application includes
  #include "app_inc/commander.h"
  #include "app_inc/packet_router.h"
  /* Application include files here */

//Platform includes
  #include "board.h"
  /* Platform include files here */

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
// TASK mappings
  #define TASK_COMMANDER_PRIORITY  6
  #define TASK_COMMANDER_STACK_SIZE 2048/sizeof(uint32_t) //size in bytes divided by RTOS stack type, portSTACK_TYPE
//End TASK mappings

//module persistent settings
  #define CMNDR_PERSISTANT_SETTINGS_ID  0x122D2C15
//End module persistent settings

//dev defines
  #define ECHO_PKT
//End dev defines

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
char cCommand_Task_Name[] = "COMMAND";

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//Commander message ID's
typedef enum COMMAND_MSG_ID
{
  COMMAND_MSG_NONE,
  COMMAND_MSG_OFF_TO_UNCONNECTED,
  COMMAND_MSG_UNCONNECTED_TO_CONNECTED,
  COMMAND_MSG_CONNECTED_TO_PLAY,
  COMMAND_MSG_PROVISION,
  COMMAND_MSG_TEST_DATA_MUTEX,
  COMMAND_MSG_INTERFACE_CONNECT,
  COMMAND_MSG_PARSE_PACKET,
  COMMAND_MSG_INTERFACE_SEND,
  COMMAND_MSG_LIMIT,
}COMMAND_MSG_ID, * pCOMMAND_MSG_ID;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//tCommand activity state for command, control and status of the task
typedef struct Command_Activity_State_t
{
  bool bIs_Command_Task_Ready;
  bool bIs_Command_Provisioned;
  bool bAuto_Interface_Connect;
  char cAP_ID[128];
  char cAP_PW[64];
  uint32_t uiCommaner_NVID;
  pQ_Handle hQ_Cmdr;
}Command_Activity_State_t, * pCommand_Activity_State;

Command_Activity_State_t Command_AS_t =
{
  .bIs_Command_Task_Ready  = false,
  .bIs_Command_Provisioned = false,
  .bAuto_Interface_Connect = false,
  .cAP_ID = {
              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            },
  .cAP_PW = {
              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            },
  .hQ_Cmdr = NULL,
  .uiCommaner_NVID = CMNDR_PERSISTANT_SETTINGS_ID,
};

typedef struct Commander_Message_Struct_t
{
    COMMAND_MSG_ID eMSG;
    uint8_t *      pBuff;
    uint32_t       u32BuffLen;
    bool           bBuffAllocated;
}Commander_Message_Struct_t, *pCommander_Message_Struct;

typedef struct Commander_Mutex_Data
{
  int32_t i32Data;
  int16_t i16DataA;
  int16_t i16DataB;
  uint8_t uiDataArray[32];
}Commander_Mutex_Data_t, * pCommander_Mutex_Data;
/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static void       vCommanderPacketReceive(uint8_t * pPacket, uint32_t u32Packet_Len);
static ERROR_CODE eCommander_Pkt_Parse(uint8_t * pBuff, uint32_t ui32Len);
static ERROR_CODE eProcess_Config_Pkt(pPacket pPkt);
static ERROR_CODE eProcess_GC_Pkt(pPacket pPkt);
static ERROR_CODE eProcess_Movement_Pkt(pPacket pPkt);
static ERROR_CODE eProcess_Vid_Pkt(pPacket pPkt);
static ERROR_CODE eProcess_Data_Pkt(pPacket pPkt);
static ERROR_CODE eProcess_Sen_Pkt(pPacket pPkt);
static void       vCommander_Task(void * pvParameters);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static void vCommanderPacketReceive(uint8_t * pPacket, uint32_t u32Packet_Len)
{
  Commander_Message_Struct_t Msg_t;
  Msg_t.eMSG = COMMAND_MSG_PARSE_PACKET;
  Msg_t.u32BuffLen = u32Packet_Len;
  Msg_t.pBuff = calloc(u32Packet_Len, sizeof(uint8_t));
  memcpy(Msg_t.pBuff, pPacket, u32Packet_Len);
  Msg_t.bBuffAllocated = true;
  eOSAL_Queue_Post_msg(Command_AS_t.hQ_Cmdr, &Msg_t);
  return;
}

static ERROR_CODE eCommander_Pkt_Parse(uint8_t * pBuff, uint32_t ui32Len)
{
  ERROR_CODE eEC = ER_FAIL;
//  pCmnd_Req pReq;

  if(sizeof(Packet_t) != ui32Len)
    eEC = ER_LEN;
  else if(NULL == pBuff)
    eEC = ER_NO_BUFF;
  else
  {
#if defined(ECHO_PKT)
    Bluetooth_Request_t Bt_Req_t;
    Bt_Req_t.eRequestID = BLUETOOTH_REQUEST_SEND;
    Bt_Req_t.pBuff = pBuff;
    Bt_Req_t.ui32Len = ui32Len;
    eEC = eBluetooth_Request(&Bt_Req_t);
#else
    pPacket pPkt;
    pPkt = (pPacket)pBuff;
    switch(pPkt->id)
    {
      case PKT_ID_CONFIG:
        eProcess_Config_Pkt(pPkt);
        break;
      case PKT_ID_GENERAL_CONTROL:
        eProcess_GC_Pkt(pPkt);
        break;
      case PKT_ID_MOVEMENT_CONTROL:
        eProcess_Movement_Pkt(pPkt);
        break;
      case PKT_ID_VIDEO_CONTROL:
        eProcess_Vid_Pkt(pPkt);
        break;
      case PKT_ID_DATA:
        eProcess_Data_Pkt(pPkt);
        break;
      case PKT_ID_SENSOR_CONTROL:
        eProcess_Sen_Pkt(pPkt);
        break;
      default:
        break;
    }
#endif
  }

  return eEC;
}

static ERROR_CODE eProcess_Config_Pkt(pPacket pPkt)
{
  ERROR_CODE eEC = ER_FAIL;
  switch(pPkt->command)
  {
    case PKT_CMD_DEVICE_NAME:
      break;
    case PKT_CMD_DEVICE_TIME:
      break;
    case PKT_CMD_DEVICE_SOFTWARE:
      switch(pPkt->sub_command)
      {
        case 2:
        case 3:
        case 4:
        default:
          break;
      }
      break;
    default:
      break;
  }
  return eEC;
}

static ERROR_CODE eProcess_GC_Pkt(pPacket pPkt)
{
  ERROR_CODE eEC = ER_FAIL;
  return eEC;
}

static ERROR_CODE eProcess_Movement_Pkt(pPacket pPkt)
{
  ERROR_CODE eEC = ER_FAIL;
  return eEC;
}

static ERROR_CODE eProcess_Vid_Pkt(pPacket pPkt)
{
  ERROR_CODE eEC = ER_FAIL;
  return eEC;
}

static ERROR_CODE eProcess_Data_Pkt(pPacket pPkt)
{
  ERROR_CODE eEC = ER_FAIL;
  return eEC;
}

static ERROR_CODE eProcess_Sen_Pkt(pPacket pPkt)
{
  ERROR_CODE eEC = ER_FAIL;
  return eEC;
}


/******************************************************************************
* name: vCommander_Task
* description: Commander task function
* param description: void - pwParameters: pointer to the task parameters
* return value description: void: should never return
******************************************************************************/
static void vCommander_Task(void * pvParameters)
{
  ERROR_CODE eEC = ER_FAIL;
  OSAL_Queue_Parameters_t tCommander_Queue_Param;
  OSAL_Queue_Handle_t * pCommander_Queue_Handle;
  OSAL_Data_Mutex_Parameters_t Commander_Data_Mutex_Param_t;
  pOSAL_Data_Mutex_Handle pCommander_Data_Mutex_Handle;
  Commander_Message_Struct_t Msg_t;
#if (PROJ_CONFIG_USE_DRVR_BLUETOOTH >= 1)
  Bluetooth_Request_t BT_Req_t;
#endif //PROJ_CONFIG_USE_DRVR_BLUETOOTH
#if (PROJ_CONFIG_USE_DRVR_WIFI >= 1)
  Wifi_Request_t WifiReq_t;
#endif //PROJ_CONFIG_USE_DRVR_WIFI

  //create the commanders data mutex
  //
  Commander_Data_Mutex_Param_t.uiObject_Size = sizeof(Commander_Mutex_Data_t);
  Commander_Data_Mutex_Param_t.uiTimeout = 1000;
  eEC = eOSAL_Data_Mutex_Create(&Commander_Data_Mutex_Param_t, &pCommander_Data_Mutex_Handle);
  vDEBUG_ASSERT("Commander data mutex create fail", eEC == ER_OK);

  //create the commanders message queue
  //
  eEC = eOSAL_Queue_Params_Init(&tCommander_Queue_Param);
  vDEBUG_ASSERT("Commander Queue params init fail", eEC == ER_OK);
  tCommander_Queue_Param.uiNum_Of_Queue_Elements = 3;
  tCommander_Queue_Param.uiSize_Of_Queue_Element = sizeof(Commander_Message_Struct_t);
  tCommander_Queue_Param.pMsgBuff = &Msg_t;
  tCommander_Queue_Param.iTimeout = OSAL_QUEUE_TIMEOUT_WAITFOREVER;
  eEC = eOSAL_Queue_Create(&tCommander_Queue_Param, &pCommander_Queue_Handle);
  vDEBUG_ASSERT("vCommander_Task queue create fail", eEC == ER_OK);
  Command_AS_t.hQ_Cmdr = pCommander_Queue_Handle;

#if (PROJ_CONFIG_USE_DRVR_NVRAM >= 1)
  tNvram_Request tNVReq;
  pCommand_Activity_State pActivity_State;

  //check nv ram for persistent settings
  //
  eEC = eNvram_Request_Param_Init(&tNVReq);
  vDEBUG_ASSERT("eNvram_Request_Param_Init fail", eEC == ER_OK);
  tNVReq.eRequestID = NVRAM_REQUEST_IS_ID_REGISTERED;
  tNVReq.uiSettings_ID = Command_AS_t.uiCommaner_NVID;
  eEC = eNvram_Request(&tNVReq);
  if(eEC == ER_OK)
  {
    pActivity_State = calloc(sizeof(tCommand_Activity_State), sizeof(uint8_t));
    eNvram_Request_Param_Init(&tNVReq);
    tNVReq.eRequestID = NVRAM_REQUEST_READ;
    tNVReq.uiSettings_ID = Command_AS_t.uiCommaner_NVID;
    tNVReq.pBuff = (uint8_t *)pActivity_State;
    tNVReq.uiSize = sizeof(tCommand_Activity_State);
    eNvram_Request(&tNVReq);
    memcpy(&Command_AS_t.bAuto_Interface_Connect, &pActivity_State->bAuto_Interface_Connect, (sizeof(Command_AS_t.bAuto_Interface_Connect) + sizeof(Command_AS_t.cAP_ID) + sizeof(Command_AS_t.cAP_PW)));
    free(pActivity_State);
  }
#else //PROJ_CONFIG_USE_DRVR_NVRAM == 0
  Command_AS_t.bIs_Command_Provisioned = false;
  Command_AS_t.bAuto_Interface_Connect = true;
#endif //PROJ_CONFIG_USE_DRVR_NVRAM

  //check auto connect settings
  //
  if(Command_AS_t.bIs_Command_Provisioned == true)
  {
    //
    Msg_t.eMSG = COMMAND_MSG_INTERFACE_CONNECT;
    eEC = eOSAL_Queue_Post_msg(pCommander_Queue_Handle, &Msg_t);
    vDEBUG_ASSERT("vCommander_Task msg post fail", eEC == ER_OK);
  }
  else
  {
    //clear stored wifi info
    Msg_t.eMSG = COMMAND_MSG_PROVISION;
    eEC = eOSAL_Queue_Post_msg(pCommander_Queue_Handle, &Msg_t);
  }

  //Register with the packet router
  //
  eEC = ePacket_Client_Register(PKT_ID_CONFIG, &vCommanderPacketReceive);
  eEC = ePacket_Client_Register(PKT_ID_GENERAL_CONTROL, &vCommanderPacketReceive);

  //just before the task loop mark commander task ready
  //
  Command_AS_t.bIs_Command_Task_Ready = true;

  while(1)
  {
    if(eOSAL_Queue_Get_msg(pCommander_Queue_Handle, &Msg_t) == ER_OK)
    {
      switch(Msg_t.eMSG)
      {
//        case COMMAND_MSG_TEST_DATA_MUTEX:
//          eOSAL_Data_Mutex_Get(pCommander_Data_Mutex_Handle, (void **)&pMutex_Data);
//          eOSAL_Data_Mutex_Return(pCommander_Data_Mutex_Handle, (void **)&pMutex_Data);
//          break;
        case COMMAND_MSG_PROVISION:
#if (PROJ_CONFIG_USE_DRVR_BLUETOOTH >= 1)
          BT_Req_t.eRequestID = BLUETOOTH_REQUEST_PROVISION;
          eBluetooth_Request(&BT_Req_t);
#endif //PROJ_CONFIG_USE_DRVR_BLUETOOTH
          break;
        case COMMAND_MSG_INTERFACE_CONNECT:
#if (PROJ_CONFIG_USE_DRVR_WIFI >= 1)
          eWifi_Request_Param_Init(&WifiReq_t);
          WifiReq_t.eRequestID = WIFI_REQUEST_CONNECT;
          WifiReq_t.cAP_ID = Command_AS_t.cAP_ID;
          WifiReq_t.cAP_PW = Command_AS_t.cAP_PW;
          eWifi_Request(&WifiReq_t);
#endif //PROJ_CONFIG_USE_DRVR_WIFI
          break;
        case COMMAND_MSG_PARSE_PACKET:
          eCommander_Pkt_Parse(Msg_t.pBuff, Msg_t.u32BuffLen);
          if(Msg_t.bBuffAllocated == true)
          {
            free(Msg_t.pBuff);
          }
          break;
        case COMMAND_MSG_INTERFACE_SEND:
#if (PROJ_CONFIG_USE_DRVR_BLUETOOTH >= 1)
          BT_Req_t.eRequestID = BLUETOOTH_REQUEST_SEND;
          eBluetooth_Request(&BT_Req_t);
#endif //PROJ_CONFIG_USE_DRVR_BLUETOOTH
          break;
        default:
          break;
      }
    }
  }
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name: eCommand_Param_Init
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eCommand_Param_Init(pCmnd_Req pRequest)
{
  ERROR_CODE eEC = ER_FAIL;
  if(pRequest == NULL)
  {
    eEC = ER_PARAM;
  }
  else
  {
    pRequest->eRequestID = CMND_REQUEST_NONE;
    pRequest->pCommander_Task_Param = NULL;

    if((pRequest->eRequestID            == CMND_REQUEST_NONE) &
       (pRequest->pCommander_Task_Param == NULL))
    {
      eEC = ER_OK;
    }
  }

  return eEC;
}

/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name: eCommand_Request
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eCommand_Request(pCmnd_Req pRequest)
{
  ERROR_CODE eEC = ER_OK;

  switch(pRequest->eRequestID)
  {
    case CMND_REQUEST_TASK_PARAMETERS:
      pRequest->pCommander_Task_Param->pTaskFcn = &vCommander_Task;
      pRequest->pCommander_Task_Param->pName = cCommand_Task_Name;
      pRequest->pCommander_Task_Param->uiStack_Size = TASK_COMMANDER_STACK_SIZE;
      pRequest->pCommander_Task_Param->pParameters = NULL;
      pRequest->pCommander_Task_Param->uiTask_Priority = TASK_COMMANDER_PRIORITY;
      eEC = ER_OK;
      break;
    case CMND_REQUEST_SEND:
      eEC = ER_OK;
      break;
    default:
      eEC = ER_FAIL;
      break;
  }

    return eEC;
}

#endif //PROJ_CONFIG_USE_APP_COMMANDER
#endif //__COMMANDER_C__
