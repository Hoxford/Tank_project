/******************************************************************************
*
* usb.c - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __USB_C__
#define __USB_C__
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
#if (PROJ_CONFIG_USE_DRVR_USB == 0)
#else

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  /* Utility include files here */

//Third party includes
  #include "ThirdParty_inc/osal.h"
  /* Third party include files here */

//Driver includes
  #include "drivers_inc/usb.h"
  /* Driver include files here */

//Application includes
  #include "app_inc/commander.h"
  /* Application include files here */

//Platform includes
  #include "board.h"
  /* Platform include files here */

#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#ifdef USE_USB_CDC
  #include "usbd_cdc.h"
  #include "usbd_cdc_interface.h"
#elif USE_USB_HID
  #include "usbd_hid.h"
#else
  #warning NO USB CLASS DEFINED
#endif
#include "usbd_cdc_interface.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define TASK_USB_PRIORITY    3
#define TASK_USB_STACK_SIZE 2048/sizeof(uint32_t) //size in bytes divided by RTOS stack type, portSTACK_TYPE

// Define group ID mappings IE:
// RADIO interface mappings////////////////////////////////
//  #def RADIO_EXAMPLE_DEF  value  //def description

// RADIO sub group mappings
//  #def RADIO_EXAMPLE_DEF_AGAIN value //def description

// END RADIO interface mappings////////////////////////////

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
char cUsb_Task_Name[] = "USB";

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//extern int iExample_extern_var;  //external variable description

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
typedef enum USB_MSG_ID
{
  USB_MSG_NONE,
  USB_MSG_CONNECT,
  USB_MSG_DISCONNECT,
  USB_MSG_SEND,
  USB_MSG_RCV,
  USB_MSG_LIMIT,
}USB_MSG_ID;

typedef enum CDC_ApplicationTypeDef
{
  APPLICATION_IDLE = 0,
  APPLICATION_START,
  APPLICATION_RUNNING,
}CDC_ApplicationTypeDef;

CDC_ApplicationTypeDef AppliState = APPLICATION_IDLE;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
typedef struct tUSB_Send
{
  bool bIs_Send_Free;
  uint8_t uiBuff[1024];
}tUSB_Send;

typedef struct tUSB_Activity_State
{
  bool bIs_USB_Ready;
  tUSB_Send tUSB_Send_Buff;
  tOSAL_Queue_Handle * pUSB_Queue_Handle;
}tUSB_Activity_State;

tUSB_Activity_State tUSB_AS =
{
  false,//  bool bIs_USB_Ready;
  //tUSB_Send tUSB_Send_Buff;
  {
    false, //bool bIs_Send_Free;
    //uint8_t uiBuff[1024];
    {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    },
  },
  NULL, //tOSAL_Queue_Handle * pUSB_Queue_Handle;
};

USBD_HandleTypeDef  USBD_Device; /* USB Device handle */

typedef struct tUSB_Message_Struct
{
  USB_MSG_ID eMSG;
  uint8_t *  pBuff;
  uint16_t   uiBuff_Len;
}tUSB_Message_Struct;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//extern int external_example_function(param); //short external function description
/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
ERROR_CODE eUSB_setup(void);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name:eUSB_setup
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eUSB_setup(void)
{
  ERROR_CODE eEC = ER_FAIL;
  USBD_StatusTypeDef eUSBD_Status = USBD_FAIL;

  if(tUSB_AS.bIs_USB_Ready == false)
  {
#if defined(USE_USB_CDC)
    /* Init Device Library */
    eUSBD_Status = USBD_Init(&USBD_Device, &VCP_Desc, 0);

    if(eUSBD_Status == USBD_OK)
    {
      /* Add Supported Class */
      eUSBD_Status = USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
    }

    if(eUSBD_Status == USBD_OK)
    {
      /* Add CDC Interface Class */
      eUSBD_Status = USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
    }

    if(eUSBD_Status == USBD_OK)
    {
      /* Start Device Process */
      eUSBD_Status = USBD_Start(&USBD_Device);
    }
#elif defined(USE_USB_HID)
    /* Init Device Library */
    eUSBD_Status = USBD_Init(&USBD_Device, &HID_Desc, 0);

    if(eUSBD_Status == USBD_OK)
    {
      /* Add Supported Class */
      eUSBD_Status = USBD_RegisterClass(&USBD_Device, USBD_HID_CLASS);
    }

    if(eUSBD_Status == USBD_OK)
    {
      /* Start Device Process */
      eUSBD_Status = USBD_Start(&USBD_Device);
    }
#else
  #warning NO USB CLASS DEFINED
#endif
  }

  if(eUSBD_Status == USBD_OK)
  {
    tUSB_AS.bIs_USB_Ready = true;
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* name: vUSB_Task
* description: USB task function. Handles task related interfacing to the USB API.
* param description: void * - pvParameters: parameter passed to the function from its instantiation.
* return value description: none
******************************************************************************/
void vUSB_Task(void * pvParameters)
{
  ERROR_CODE eEC = ER_FAIL;
  USBD_StatusTypeDef eUSBD_Status = USBD_FAIL;
  tOSAL_Queue_Parameters tUSB_Queue_Param;
  tUSB_Message_Struct tMsg;

  //create the USB message queue
  eEC = eOSAL_Queue_Params_Init(&tUSB_Queue_Param);
  vDEBUG_ASSERT("USB Queue params init fail", eEC == ER_OK);
  tUSB_Queue_Param.uiNum_Of_Queue_Elements = 3;
  tUSB_Queue_Param.uiSize_Of_Queue_Element = sizeof(tUSB_Message_Struct);
  tUSB_Queue_Param.pMsgBuff = &tMsg;
  tUSB_Queue_Param.iTimeout = OSAL_QUEUE_TIMEOUT_WAITFOREVER;
  eEC = eOSAL_Queue_Create(&tUSB_Queue_Param, &tUSB_AS.pUSB_Queue_Handle);
  vDEBUG_ASSERT("vUSB_Task queue create fail", eEC == ER_OK);

  //check persistent settings
  //todo: persistent settings code

  //set up the USB low level driver
  eEC = eUSB_setup();
  vDEBUG_ASSERT("vUSB_Task driver setup fail", eEC == ER_OK);

  while(1)
  {
    if(eOSAL_Queue_Get_msg(tUSB_AS.pUSB_Queue_Handle, &tMsg) == ER_OK)
    {
      switch(tMsg.eMSG)
      {
        case USB_MSG_CONNECT:
          break;
        case USB_MSG_DISCONNECT:
          break;
        case USB_MSG_SEND:
        {
#if defined(USE_USB_CDC)
          eUSBD_Status = USBD_CDC_fops.Control(CDC_RAW_SEND, tMsg.pBuff, tMsg.uiBuff_Len);
          if(eUSBD_Status == USBD_OK)
          {
            //TODO: call callbacks
          }
#else
#warning NO USB SEND DEFINED
#endif //USB SEND
          break;
        }
        case USB_MSG_RCV:
          break;
        default:
          break;
      }
    }
    else
    {
      vDEBUG_ASSERT("vUSB_Task eOSAL_Queue_Get_msg fail", 0);
    }
  }
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name: eUSB_Request_Param_Init
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eUSB_Request_Param_Init(tUsb_Request * pRequest)
{
  ERROR_CODE eEC = ER_FAIL;
  void * pRC = NULL;

  pRC = memset(pRequest, 0x00, sizeof(tUsb_Request));

  if(pRC != NULL)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name: eUSB_Request
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eUSB_Request(tUsb_Request * pRequest)
{
  ERROR_CODE eEC = ER_FAIL;
  USBD_StatusTypeDef eUSBD_Status = USBD_FAIL;
  tUSB_Message_Struct tMsg;

  if(pRequest->eRequestID == USB_REQUEST_TASK_PARAM)
  {
    pRequest->pUsb_Task_Param->pName           = cUsb_Task_Name;
    pRequest->pUsb_Task_Param->pParameters     = NULL;
    pRequest->pUsb_Task_Param->pTaskFcn        = vUSB_Task;
    pRequest->pUsb_Task_Param->uiStack_Size    = TASK_USB_STACK_SIZE;
    pRequest->pUsb_Task_Param->uiTask_Priority = TASK_USB_PRIORITY;

    eEC = ER_OK;
  }
  else if(pRequest->eRequestID == USB_REQUEST_INIT)
  {

  }
  else
  {
    switch(pRequest->eRequestID)
    {
      case USB_REQUEST_REGISTER_CALLBACK:
        break;
      case USB_REQUEST_SEND:
        tMsg.eMSG = USB_MSG_SEND;
        memcpy(tUSB_AS.tUSB_Send_Buff.uiBuff, pRequest->pBuff, pRequest->uiSize);
        eEC = eOSAL_Queue_Post_msg(tUSB_AS.pUSB_Queue_Handle, &tMsg);
        break;
      case USB_REQUEST_SEND_IMMEDIATE:
        eUSBD_Status = USBD_CDC_fops.Control(CDC_RAW_SEND, pRequest->pBuff, pRequest->uiSize);
        if(eUSBD_Status == USBD_OK)
        {
          eEC = ER_OK;
        }
        break;
      case USB_REQUEST_RECEIVE:
        break;
      default:
        break;
    }

  }

  return eEC;
}

#endif //#if (PROJ_CONFIG_USE_DRVR_USB >= 1)

#endif //__USB_C__
