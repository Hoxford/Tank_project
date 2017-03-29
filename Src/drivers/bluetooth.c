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
  #include "utils_inc/ring_buffer.h"
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
  #include "app_inc/packet_router.h"
  /* Application include files here */

//Platform includes
  #include "board.h"
  /* Platform include files here */


/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define TASK_BT_PRIORITY       2
#define TASK_BT_STACK_SIZE 4096/sizeof(uint32_t) //size in bytes divided by RTOS stack type, portSTACK_TYPE

#define TASK_BT_RCV_BUFF_SIZE   64
#define TASK_BT_SEND_BUFF_SIZE  64

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

const  char                 cBluetooth_Task_Name[] = "Bluetooth";
static pOSAL_Queue_Handle   pBluetooth_Queue = NULL;
static RB_HANDLE            hRingBuffCmndRcv = 0;
static RB_HANDLE            hRingBuffDataRcv = 0;
static RB_HANDLE            hRingBuffDataSend = 0;

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//Bluetooth message identifiers
typedef enum _BLUETOOTH_MSG_ID
{
  BLUETOOTH_MSG_NONE,
  BLUETOOTH_RECEIVE_COMMAND_FRAME,
  BLUETOOTH_RECEIVE_DATA_FRAME,
  BLUETOOTH_SEND_DATA_FRAME,
  BLUETOOTH_MSG_LIMIT,
}BLUETOOTH_MSG_ID, * pBLUETOOTH_MSG_ID;

typedef enum _BLUETOOTH_STATE
{
  BLUETOOTH_STATE_NONE,
  BLUETOOTH_STATE_LIMIT,
}BLUETOOTH_STATE, * pBLUETOOTH_STATE;

typedef enum _RECEVING_BT_TYPE
{
  BT_RCV_IDLE,
  BT_RCV_CMD,
  BT_RCV_DATA,
}RECEVING_BT_TYPE, * pRECEVING_BT_TYPE;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//Bluetooth activity state struct
typedef struct Bluetooth_Activity_State
{
  BLUETOOTH_STATE       eState;
  RECEVING_BT_TYPE      eReceiving_State;
  uint32_t              u32RcvIndex;
  uint8_t *             pRcvBuff;
  uint8_t *             pSendBuff;
  Bluetooth_App_API_t   api_t;
}Bluetooth_Activity_State_t, * pBluetooth_Activity_State;

typedef struct Bluetooth_Message_Struct
{
  BLUETOOTH_MSG_ID eMSG;
}BT_msg_t, *pBT_msg;

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

typedef struct Bluetooth_Command_Packet
{
  uint8_t   u8ID;
  uint8_t   u8Sequence;
  uint8_t   u8Command;
  uint8_t   u8SubCommand;
  uint8_t   u8Payload[2];
  uint8_t   u8Tbd[2];
}BT_Cmd_Pkt_t, * pBT_Cmd_Pkt;

extern Bluetooth_Device_API_t BT_BSP_API_t;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static ERROR_CODE eBluetooth_Receive_Frame_CB(uint8_t * pBuff, uint32_t u32BuffLen);
static ERROR_CODE eBluetooth_Conn_Status_CB(BLUETOOTH_CONN_STATE eState);
static void       vBluetooth_Driver_Task(void * pvParameters);

Bluetooth_Activity_State_t BT_AS_t =
{
  .eState = BLUETOOTH_STATE_NONE,
  .eReceiving_State = BT_RCV_IDLE,
  .u32RcvIndex = 0,
  .pRcvBuff = NULL,
  .pSendBuff = NULL,
  .api_t.eBTAppRcvCallBack = &eBluetooth_Receive_Frame_CB,
  .api_t.eBTAppConnStatusCallBack = &eBluetooth_Conn_Status_CB,
};

Bluetooth_App_API_t App_API_t =
{
  .eBTAppRcvCallBack        = &eBluetooth_Receive_Frame_CB,
  .eBTAppConnStatusCallBack = &eBluetooth_Conn_Status_CB,
};

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: eBluetooth_Receive_Frame_CB
******************************************************************************/
static ERROR_CODE eBluetooth_Receive_Frame_CB(uint8_t * pBuff, uint32_t u32BuffLen)
{
  ERROR_CODE eEC = ER_FAIL;
//  pBT_Cmd_Pkt pPkt;
  BT_msg_t Msg_t;

  if(BT_AS_t.pRcvBuff[0] == 0x10) //todo: need to add a command protocol file
  {
    eEC = eRingBuff_App_Get_Buff(hRingBuffCmndRcv, &BT_AS_t.pRcvBuff);
    vDEBUG_ASSERT("Ring buff fail", eEC == ER_OK);
    eEC = BT_BSP_API_t.eBTDeviceRcv(BT_AS_t.pRcvBuff, 8);

    Msg_t.eMSG = BLUETOOTH_RECEIVE_COMMAND_FRAME;
    eOSAL_Queue_Post_msg_ISR(pBluetooth_Queue, &Msg_t);
  }
  else if(BT_AS_t.pRcvBuff[0] == 0x1F)
  {
    eEC = eRingBuff_Fill_Next_Free(hRingBuffCmndRcv, pBuff, 64);
    vDEBUG_ASSERT("Ring buff fail", eEC == ER_OK);
  }

  return eEC;
}

/******************************************************************************
* name: eBluetooth_Conn_Status_CB
******************************************************************************/
static ERROR_CODE eBluetooth_Conn_Status_CB(BLUETOOTH_CONN_STATE eState)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", false);
  return eEC;
}

/******************************************************************************
* name: vBluetooth_Driver_Task
******************************************************************************/
static void vBluetooth_Driver_Task(void * pvParameters)
{
  ERROR_CODE eEC = ER_FAIL;
  OSAL_Queue_Parameters_t Bluetooth_Queue_Param_t;
  BT_msg_t Msg_t;
//  uint8_t * pBuff;
  uint8_t u8CommandBuff[8];
//  Bluetooth_Send_t    Send_t;
//  Bluetooth_Receive_t Rcv_t;

  eOSAL_Queue_Params_Init(&Bluetooth_Queue_Param_t);
  Bluetooth_Queue_Param_t.uiNum_Of_Queue_Elements = 20;
  Bluetooth_Queue_Param_t.uiSize_Of_Queue_Element = sizeof(BT_msg_t);
  Bluetooth_Queue_Param_t.pMsgBuff                = &Msg_t;
  Bluetooth_Queue_Param_t.iTimeout                = OSAL_QUEUE_TIMEOUT_WAITFOREVER;

  eEC = eOSAL_Queue_Create(&Bluetooth_Queue_Param_t, &pBluetooth_Queue);
  vDEBUG_ASSERT("vBluetooth_Driver_Task queue create fail", eEC == ER_OK);

  if(eEC == ER_OK)
  {
    eEC = BT_BSP_API_t.eBTDeviceSetup(&App_API_t);
    vDEBUG_ASSERT("Bluetooth setup failure", eEC == ER_OK);
  }

  if(eEC == ER_OK)
  {
    //set up the bluetooth command receive ring buffer
    //
    eEC = eRingBuff_App_Create(&hRingBuffCmndRcv, 8, 8);
    vDEBUG_ASSERT("BT rcv cmnd ring buff create fail", eEC == ER_OK);
    eRingBuff_App_Get_Buff(hRingBuffCmndRcv, &BT_AS_t.pRcvBuff);

    //set up the bluetooth data receive ring buffer
    //
    eEC = eRingBuff_App_Create(&hRingBuffDataRcv, 4, 64);
    vDEBUG_ASSERT("BT rcv data ring buff create fail", eEC == ER_OK);

    //set up the bluetooth send ring buffer
    //
    eEC = eRingBuff_Create(&hRingBuffDataSend, 8, 64);
    vDEBUG_ASSERT("BT send data ring buff create fail", eEC == ER_OK);
  }

  if(eEC == ER_OK)
  {
    //Kick off data reception from the bluetooth device
    //
    eEC = BT_BSP_API_t.eBTDeviceRcv(BT_AS_t.pRcvBuff, 8);
    vDEBUG_ASSERT("Bluetooth receiving failure", eEC == ER_OK);
  }

  //Check to make sure the task initialized properly before starting loop
  //
  if(eEC != ER_OK)
  {
    vDEBUG("vBluetooth_Driver_Task failed to init");
  }
  else
  {
    while(1)
    {
      if(eOSAL_Queue_Get_msg(pBluetooth_Queue, &Msg_t) == ER_OK)
      {
        switch(Msg_t.eMSG)
        {
          case BLUETOOTH_RECEIVE_COMMAND_FRAME:
          {
            while(eRingBuff_App_Read_Buff(hRingBuffCmndRcv, u8CommandBuff) == ER_OK)
            {
              ePacket_Route(u8CommandBuff, 8);
            }
            break;
          }
          case BLUETOOTH_RECEIVE_DATA_FRAME:
          {
            break;
          }
          case BLUETOOTH_SEND_DATA_FRAME:
          {
            break;
          }
          default:
            break;
        }
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
