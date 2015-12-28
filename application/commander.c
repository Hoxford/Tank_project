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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "utils_inc/error_codes.h"
#include "utils_inc/osal.h"
#include "utils_inc/proj_debug.h"

#include "drivers_inc/wifi.h"
#include "drivers_inc/nvram.h"
#include "app_inc/commander.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
// TASK mappings
  #define TASK_COMMANDER_PRIORITY  6
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
  COMMAND_MSG_INTERFACE_CONNECT,
  COMMAND_MSG_LIMIT,
}COMMAND_MSG_ID;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//tCommand activity state for command, control and status of the task
typedef struct tCommand_Activity_State
{
  bool bIs_Command_Task_Ready;
  bool bAuto_Interface_Connect;
  char cAP_ID[128];
  char cAP_PW[64];
  uint32_t uiCommaner_NVID;
}tCommand_Activity_State;

tCommand_Activity_State tCommand_AS =
{
  false, //bool bIs_Command_Task_Ready;
  true, //todo: default setting is false, //bool bAuto_Interface_Connect;
  //char cAP_ID[128];
  {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

  },
  //char cAP_PW[64];
  {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  },
  CMNDR_PERSISTANT_SETTINGS_ID,//uint32_t uiCommaner_NVID;
};

typedef struct tCommander_Message_Struct
{
    COMMAND_MSG_ID eMSG;
}tCommander_Message_Struct;

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
  tOSAL_Queue_Parameters tCommander_Queue_Param;
  tOSAL_Queue_Handle * pCommander_Queue_Handle;
  tCommander_Message_Struct tMsg;
  tWifi_Request tWifiReq;
  tNvram_Request tNVReq;

  eEC = eOSAL_Queue_Params_Init(&tCommander_Queue_Param);
  vDEBUG_ASSERT("Commander Queue params init fail", eEC == ER_OK);
  tCommander_Queue_Param.uiNum_Of_Queue_Elements = 3;
  tCommander_Queue_Param.uiSize_Of_Queue_Element = sizeof(tCommander_Message_Struct);
  tCommander_Queue_Param.pMsgBuff = &tMsg;
  tCommander_Queue_Param.iTimeout = OSAL_QUEUE_TIMEOUT_WAITFOREVER;

  eEC = eOSAL_Queue_Create(&tCommander_Queue_Param, &pCommander_Queue_Handle);
  vDEBUG_ASSERT("vCommander_Task queue create fail", eEC == ER_OK);

  eEC = eNvram_Request_Param_Init(&tNVReq);
  vDEBUG_ASSERT("eNvram_Request_Param_Init fail", eEC == ER_OK);
  tNVReq.eRequestID = NVRAM_REQUEST_REGISTER_ID;
  tNVReq.pBuff = (uint8_t)&tCommand_AS;
  tNVReq.uiSize = sizeof(tCommand_Activity_State);
  //eNvram_Request


  if(tCommand_AS.bAuto_Interface_Connect == true)
  {
    tMsg.eMSG = COMMAND_MSG_INTERFACE_CONNECT;
    eEC = eOSAL_Queue_Post_msg(pCommander_Queue_Handle, &tMsg);
    vDEBUG_ASSERT("vCommander_Task msg post fail", eEC == ER_OK);
  }

  tCommand_AS.bIs_Command_Task_Ready = true;

  while(1)
  {
    eOSAL_delay(1000, NULL);
    if(eOSAL_Queue_Get_msg(pCommander_Queue_Handle, &tMsg) == ER_OK)
    {
      switch(tMsg.eMSG)
      {
        case COMMAND_MSG_INTERFACE_CONNECT:
          eWifi_Request_Param_Init(&tWifiReq);
          tWifiReq.eRequestID = WIFI_REQUEST_CONNECT;
          tWifiReq.cAP_ID = &tCommand_AS.cAP_ID;
          tWifiReq.cAP_PW = &tCommand_AS.cAP_PW;
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
      pRequest->pCommander_Task_Param->uiStack_Size = 2048;
      pRequest->pCommander_Task_Param->pParameters = NULL;
      pRequest->pCommander_Task_Param->uiTask_Priority = TASK_COMMANDER_PRIORITY;
      eEC = ER_OK;
      break;
    default:
      eEC = ER_FAIL;
  }

    return eEC;
}

#endif //__COMMANDER_C__
