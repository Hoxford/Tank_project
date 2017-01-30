/******************************************************************************
*
* bluetooth.c - Bluetooth driver source code.
* Copyright (c) notice
*
******************************************************************************/
#ifndef __BLUETOOTH_C__
#define __BLUETOOTH_C__
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
#if (PROJ_CONFIG_USE_DRVR_BLUETOOTH == 0)
#else

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  /* Utility include files here */

//Third party includes
  #include "ThirdParty_inc/osal.h"
  /* Third party include files here */

//Driver includes
  #include "drivers_inc/bluetooth.h"
  #include "drivers_inc/bluetooth_HC-05.h"
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

#define TASK_BT_PRIORITY       2
#define TASK_BT_STACK_SIZE 4096/sizeof(uint32_t) //size in bytes divided by RTOS stack type, portSTACK_TYPE

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

const char cBluetooth_Task_Name[] = "Bluetooth";

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//Bluetooth message identifiers
typedef enum BLUETOOTH_MSG_ID
{
  BLUETOOTH_MSG_NONE,
  BLUETOOTH_MSG_LIMIT,
}BLUETOOTH_MSG_ID, * pBLUETOOTH_MSG_ID;

typedef enum BLUETOOTH_STATE
{
  BLUETOOTH_STATE_NONE,
  BLUETOOTH_STATE_LIMIT,
}BLUETOOTH_STATE, * pBLUETOOTH_STATE;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//tExample_struct description
typedef struct Bluetooth_Activity_State
{
  BLUETOOTH_STATE eState;
}Bluetooth_Activity_State_t, * pBluetooth_Activity_State;

Bluetooth_Activity_State_t BT_AS_t =
{
  .eState = BLUETOOTH_STATE_NONE,
};

typedef struct Bluetooth_Message_Struct
{
  BLUETOOTH_MSG_ID eMSG;
}Bluetooth_Message_Struct_t, *pBluetooth_Message_Struct;

typedef struct Bluetooth_Send
{
  uint8_t * pBuff;
  uint32_t len;
}Bluetooth_Send_t, pBluetooth_Send;

typedef struct Bluetooth_Receive
{
  uint8_t * pBuff;
  uint32_t len;
}Bluetooth_Receive_t, *pBluetooth_Receive;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

void vBluetooth_Driver_Task(void * pvParameters)
{
  ERROR_CODE eEC = ER_FAIL;
  OSAL_Queue_Parameters_t Bluetooth_Queue_Param_t;
  pOSAL_Queue_Handle      pBluetooth_Queue_Handle;
  Bluetooth_Message_Struct_t Msg_t;
//  Bluetooth_Send_t    Send_t;
//  Bluetooth_Receive_t Rcv_t;

  eOSAL_Queue_Params_Init(&Bluetooth_Queue_Param_t);
  Bluetooth_Queue_Param_t.uiNum_Of_Queue_Elements = 3;
  Bluetooth_Queue_Param_t.uiSize_Of_Queue_Element = sizeof(Bluetooth_Message_Struct_t);
  Bluetooth_Queue_Param_t.pMsgBuff                = &Msg_t;
  Bluetooth_Queue_Param_t.iTimeout                = OSAL_QUEUE_TIMEOUT_WAITFOREVER;

  eEC = eOSAL_Queue_Create(&Bluetooth_Queue_Param_t, &pBluetooth_Queue_Handle);
  vDEBUG_ASSERT("vBluetooth_Driver_Task queue create fail", eEC == ER_OK);

  eEC = eBluetooth_HC05_setup();
  vDEBUG_ASSERT("Bluetooth setup failure", eEC == ER_OK);

  while(1)
  {
    if(eOSAL_Queue_Get_msg(pBluetooth_Queue_Handle, &Msg_t) == ER_OK)
    {
      switch(Msg_t.eMSG)
      {
        default:
          break;
      }
    }
  }

  return;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
ERROR_CODE eBluetooth_Request_Param_Init(pBluetooth_Request pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  if(pParam == NULL)
  {
    eEC = ER_PARAM;
  }
  else
  {
    memset(pParam, 0x00, sizeof(Bluetooth_Request_t));
    eEC = ER_OK;
  }
  return eEC;
}

ERROR_CODE eBluetooth_Request(pBluetooth_Request pRequest)
{
  ERROR_CODE eEC = ER_FAIL;

  vDEBUG_ASSERT("BT request out of limit", ((pRequest->eRequestID > BLUETOOTH_REQUEST_NONE)|
                                            (pRequest->eRequestID < BLUETOOTH_REQUEST_LIMIT)));

  if(pRequest->eRequestID == BLUETOOTH_REQUEST_TASK_PARAMETERS)
  {
    pRequest->pBluetooth_Task_Param->pTaskFcn        = &vBluetooth_Driver_Task;
    pRequest->pBluetooth_Task_Param->pName           = cBluetooth_Task_Name;
    pRequest->pBluetooth_Task_Param->uiStack_Size    = TASK_BT_STACK_SIZE;
    pRequest->pBluetooth_Task_Param->pParameters     = NULL;
    pRequest->pBluetooth_Task_Param->uiTask_Priority = TASK_BT_PRIORITY;

    eEC = ER_OK;
  }
  else
  {
    switch(pRequest->eRequestID)
    {
      case BLUETOOTH_REQUEST_PROVISION:
        break;
      default:
        break;
    }
  }
  return eEC;
}

#endif //PROJ_CONFIG_USE_DRVR_BLUETOOTH
#endif //__BLUETOOTH_C__
