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
//eExample_enum description
//typedef enum
//{
//    enum_1,
//    enum_2,
//    enum_3....
//}eExample_enum;

//eExample_enum eMy_enum; //short eMy_enum description

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//tExample_struct description
//typedef struct
//{
//    int var1;
//    ....
//}tExample_struct;
//
//tExample_struct tMy_struct;  //short tMy_struct description

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//extern int external_example_function(param); //short external function description
/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//int example_function(param_1, param_2); //short function declaration description

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
void vUSB_Task(void * pvParameters)
{

  while(1)
  {

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
    pRequest->pCommander_Task_Param->pName           = cUsb_Task_Name;
    pRequest->pCommander_Task_Param->pParameters     = NULL;
    pRequest->pCommander_Task_Param->pTaskFcn        = vUSB_Task;
    pRequest->pCommander_Task_Param->uiStack_Size    = 2048;
    pRequest->pCommander_Task_Param->uiTask_Priority = TASK_USB_PRIORITY;
  }

  return eEC;
}

#endif //__USB_C__
