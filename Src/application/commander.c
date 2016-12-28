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
  COMMAND_MSG_INTERFACE_CONNECT,
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
  .uiCommaner_NVID = CMNDR_PERSISTANT_SETTINGS_ID,
};

typedef struct Commander_Message_Struct_t
{
    COMMAND_MSG_ID eMSG;
}Commander_Message_Struct_t, *pCommander_Message_Struct;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eCommander_Cmnd_Parse(void);
void       vCommander_Task(void * pvParameters);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eCommander_Cmnd_Parse(void)
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
void vCommander_Task(void * pvParameters)
{
  ERROR_CODE eEC = ER_FAIL;
  OSAL_Queue_Parameters_t tCommander_Queue_Param;
  OSAL_Queue_Handle_t * pCommander_Queue_Handle;
  Commander_Message_Struct_t Msg_t;
  Bluetooth_Request_t BT_Req_t;
#if (PROJ_CONFIG_USE_DRVR_WIFI >= 1)
  Wifi_Request_t WifiReq_t;
#endif //PROJ_CONFIG_USE_DRVR_WIFI

  //create the commanders message queue
  eEC = eOSAL_Queue_Params_Init(&tCommander_Queue_Param);
  vDEBUG_ASSERT("Commander Queue params init fail", eEC == ER_OK);
  tCommander_Queue_Param.uiNum_Of_Queue_Elements = 3;
  tCommander_Queue_Param.uiSize_Of_Queue_Element = sizeof(Commander_Message_Struct_t);
  tCommander_Queue_Param.pMsgBuff = &Msg_t;
  tCommander_Queue_Param.iTimeout = OSAL_QUEUE_TIMEOUT_WAITFOREVER;
  eEC = eOSAL_Queue_Create(&tCommander_Queue_Param, &pCommander_Queue_Handle);
  vDEBUG_ASSERT("vCommander_Task queue create fail", eEC == ER_OK);

#if (PROJ_CONFIG_USE_DRVR_NVRAM >= 1)
  tNvram_Request tNVReq;
  pCommand_Activity_State pActivity_State;

  //check nv ram for persistent settings
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

  //check if auto connect is enabled
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

  //just before the task loop mark commander task ready
  Command_AS_t.bIs_Command_Task_Ready = true;

  while(1)
  {
    if(eOSAL_Queue_Get_msg(pCommander_Queue_Handle, &Msg_t) == ER_OK)
    {
      switch(Msg_t.eMSG)
      {
        case COMMAND_MSG_PROVISION:
          BT_Req_t.eRequestID = BLUETOOTH_REQUEST_PROVISION;
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
ERROR_CODE eCommand_Param_Init(tCommand_Request * pRequest)
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
ERROR_CODE eCommand_Request(tCommand_Request * pRequest)
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
    default:
      eEC = ER_FAIL;
  }

    return eEC;
}

#endif //PROJ_CONFIG_USE_APP_COMMANDER
#endif //__COMMANDER_C__
