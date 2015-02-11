/******************************************************************************
*
* wifi.c - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __WIFI_C__
#define __WIFI_C__
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
#include "board.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define TASK_WIFI_PRIORITY  2

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

char cWifi_Task_Name[] = "WIFI";

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef enum WIFI_MSG_ID
{
  WIFI_MSG_NONE,
  WIFI_MSG_TEST,
  WIFI_MSG_LIMIT,
}WIFI_MSG_ID;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//tExample_struct description
typedef struct tWifi_Activity_State
{
  bool bIs_Wifi_Ready;
}tWifi_Activity_State;

tWifi_Activity_State tWifi_AS;  //short tMy_struct description

typedef struct tWifi_Message_Struct
{
    WIFI_MSG_ID eMSG;
}tWifi_Message_Struct;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

void vWifi_Driver_Task(void * pvParameters);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: vWifi_Driver_Task
* description: WIFI driver task function
* param description: void - pwParameters: pointer to the task parameters
* return value description: void: should never return
******************************************************************************/
void vWifi_Driver_Task(void * pvParameters)
{
  ERROR_CODE eEC = ER_FAIL;
  tOSAL_Queue_Parameters tWifi_Queue_Param;
  tOSAL_Queue_Handle * pWifi_Queue_Handle;
  tWifi_Message_Struct tMsg;

  eOSAL_Queue_Params_Init(&tWifi_Queue_Param);
  tWifi_Queue_Param.uiNum_Of_Queue_Elements = 5;
  tWifi_Queue_Param.uiSize_Of_Queue_Element = sizeof(tWifi_Message_Struct);
  tWifi_Queue_Param.pMsgBuff = &tMsg;
  tWifi_Queue_Param.iTimeout = OSAL_QUEUE_TIMEOUT_WAITFOREVER;

  eEC = eOSAL_Queue_Create(&tWifi_Queue_Param, &pWifi_Queue_Handle);
  vDEBUG_ASSERT("vWifi_Driver_Task queue create fail", eEC != ER_OK);

  tMsg.eMSG = WIFI_MSG_TEST;
  eEC = eOSAL_Queue_Post_msg(pWifi_Queue_Handle, &tMsg);
  vDEBUG_ASSERT("vWifi_Driver_Task msg post fail", eEC != ER_OK);

  memset(&tMsg, 0x00, sizeof(tWifi_Message_Struct));

  while(1)
  {
    if(eOSAL_Queue_Get_msg(pWifi_Queue_Handle, &tMsg) == ER_OK)
    {
      switch(tMsg.eMSG)
      {
        case WIFI_MSG_TEST:
        {
          eBSP_Wifi_Transmit(NULL);
          break;
        }
        default:
          vDEBUG_ASSERT("vWifi_Driver_Task invalid msg ID", tMsg.eMSG >= WIFI_MSG_LIMIT);
      }
    }
  }
}


/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eWifi_Request_Param_Init(tWifi_Request * pParam)
{
  ERROR_CODE eEC = ER_FAIL;

  pParam->eRequestID = WIFI_REQUEST_NONE;
  pParam->pWifi_Task_Param = NULL;
  return eEC;
}

/******************************************************************************
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eWifi_Request(tWifi_Request * pRequest)
{
  ERROR_CODE eEC = ER_FAIL;

  switch(pRequest->eRequestID)
  {
    case WIFI_REQUEST_TASK_PARAMETERS:
    {
      pRequest->pWifi_Task_Param->pTaskFcn = &vWifi_Driver_Task;
      pRequest->pWifi_Task_Param->pName = cWifi_Task_Name;
      pRequest->pWifi_Task_Param->uiStack_Size = 1024;
      pRequest->pWifi_Task_Param->pParameters = NULL;
      pRequest->pWifi_Task_Param->uiTask_Priority = TASK_WIFI_PRIORITY;
      eEC = ER_OK;

      break;
    }
    default:
      eEC = ER_FAIL;
  }

  return eEC;
}

#endif //__FILE_NAME_C__
