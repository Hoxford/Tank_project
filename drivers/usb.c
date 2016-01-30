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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "utils_inc/error_codes.h"
#include "utils_inc/osal.h"
#include "utils_inc/proj_debug.h"

#include "drivers_inc/usb.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#ifdef USE_USB_CDC
  #include "usbd_cdc.h"
#elif USE_USB_HID
  #include "usbd_hid.h"
#else
  #warning NO USB CLASS DEFINED
#endif
#include "usbd_cdc_interface.h"
#include "board.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#define TASK_USB_PRIORITY    3
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
USBD_HandleTypeDef  USBD_Device; /* USB Device handle */
USBD_HandleTypeDef  hUSBDDevice;

typedef struct tUSB_Message_Struct
{
  USB_MSG_ID eMSG;
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
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eUSB_setup(void)
{
  ERROR_CODE eEC = ER_FAIL;
  USBD_StatusTypeDef eUSBD_Status = USBD_FAIL;

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
  eUSBD_Status = USBD_Init(&hUSBDDevice, &HID_Desc, 0);

  if(eUSBD_Status == USBD_OK)
  {
    /* Add Supported Class */
    eUSBD_Status = USBD_RegisterClass(&hUSBDDevice, USBD_HID_CLASS);
  }

  if(eUSBD_Status == USBD_OK)
  {
    /* Start Device Process */
    eUSBD_Status = USBD_Start(&hUSBDDevice);
  }
#else
  #warning NO USB CLASS DEFINED
#endif

  if(eUSBD_Status == USBD_OK)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
void vUSB_Task(void * pvParameters)
{
  ERROR_CODE eEC = ER_FAIL;
  tOSAL_Queue_Parameters tUSB_Queue_Param;
  tOSAL_Queue_Handle * pUSB_Queue_Handle;
  tUSB_Message_Struct tMsg;

  //create the USB message queue
  eEC = eOSAL_Queue_Params_Init(&tUSB_Queue_Param);
  vDEBUG_ASSERT("USB Queue params init fail", eEC == ER_OK);
  tUSB_Queue_Param.uiNum_Of_Queue_Elements = 3;
  tUSB_Queue_Param.uiSize_Of_Queue_Element = sizeof(tUSB_Message_Struct);
  tUSB_Queue_Param.pMsgBuff = &tMsg;
  tUSB_Queue_Param.iTimeout = 10;
  eEC = eOSAL_Queue_Create(&tUSB_Queue_Param, &pUSB_Queue_Handle);
  vDEBUG_ASSERT("vUSB_Task queue create fail", eEC == ER_OK);

  //check persistent settings
  //todo: persistent settings code

  //set up the USB low level driver
  eEC = eUSB_setup();
  vDEBUG_ASSERT("vUSB_Task driver setup fail", eEC == ER_OK);

  while(1)
  {
    if(eOSAL_Queue_Get_msg(pUSB_Queue_Handle, &tMsg) == ER_OK)
    {
      switch(tMsg.eMSG)
      {
        case USB_MSG_CONNECT:
        case USB_MSG_DISCONNECT:
        case USB_MSG_SEND:
        case USB_MSG_RCV:
        default:
          break;
      }
    }
    else
    {

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

  if(pRequest->eRequestID == USB_REQUEST_TASK_PARAM)
  {
    pRequest->pUsb_Task_Param->pName           = cUsb_Task_Name;
    pRequest->pUsb_Task_Param->pParameters     = NULL;
    pRequest->pUsb_Task_Param->pTaskFcn        = vUSB_Task;
    pRequest->pUsb_Task_Param->uiStack_Size    = 2048;
    pRequest->pUsb_Task_Param->uiTask_Priority = TASK_USB_PRIORITY;

    eEC = ER_OK;
  }
  else if(pRequest->eRequestID == USB_REQUEST_INIT)
  {

  }

  return eEC;
}

#endif //__USB_C__
