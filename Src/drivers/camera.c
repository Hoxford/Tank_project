/******************************************************************************
*
* camera.c - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __CAMERA_C__
#define __CAMERA_C__
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
#if (PROJ_CONFIG_USE_DRVR_CAMERA == 0)
  //Camera driver not implemented
#else
  #include "proj_inc/project_main.h"

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  #include "utils_inc/clock.h"

//Driver includes
  #include "drivers_inc/wifi.h"
  #include "drivers_inc/bluetooth.h"
  #include "drivers_inc/camera.h"

//Application includes
  #include "app_inc/commander.h"

//Third party includes
  #include "ThirdParty_inc/osal.h"

//Platform includes
  #include "board.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define TASK_CAMERA_PRIORITY 4
#define TASK_CAMERA_STACK_SIZE 4096/sizeof(uint32_t) //size in bytes divided by RTOS stack type, portSTACK_TYPE

#define vCamera_GPIO_EXTI_Callback  HAL_GPIO_EXTI_Callback

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

char cCamera_Task_Name[] = "CAMERA";

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef enum CAMERA_MSG_ID
{
  CAMERA_MSG_NONE,
  CAMERA_MSG_LIMIT,
}CAMERA_MSG_ID;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//Camera activity state struct
typedef struct tCamera_Activity_State
{
  bool bIs_Camera_Ready;
  bool bIs_Camera_In_Continuouis_Mode;
}tCamera_Activity_State;

tCamera_Activity_State tCamera_AS =
{
  false, //bool bIs_Camera_Ready;

};

typedef struct tCamera_Message_Struct
{
  CAMERA_MSG_ID eMSG;
}tCamera_Message_Struct;
/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
ERROR_CODE eCamera_Setup(void);
void vCamera_Task(void * pvParameters);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
/******************************************************************************
* todo: DESCRIPTION
* name: eCamera_Setup
* description:
* param description: none
* return value description: ERROR_CODE - ER_OK: Camera properly setup and ready to take pictures
*                                        ER_FAIL: Camera setup failure
******************************************************************************/
ERROR_CODE eCamera_Setup(void)
{
  ERROR_CODE eEC = ER_FAIL;

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
void vCamera_Task(void * pvParameters)
{
  ERROR_CODE eEC = ER_FAIL;
  tOSAL_Queue_Parameters  tCamera_Queue_Param;
  tOSAL_Queue_Handle    * pCamera_Queue_Handle;
  tCamera_Message_Struct  tMsg;

  eOSAL_Queue_Params_Init(&tCamera_Queue_Param);
  tCamera_Queue_Param.uiNum_Of_Queue_Elements = 3;
  tCamera_Queue_Param.uiSize_Of_Queue_Element = sizeof(tCamera_Message_Struct);
  tCamera_Queue_Param.pMsgBuff                = &tMsg;
  tCamera_Queue_Param.iTimeout                = OSAL_QUEUE_TIMEOUT_WAITFOREVER;

  eEC = eOSAL_Queue_Create(&tCamera_Queue_Param, &pCamera_Queue_Handle);
  vDEBUG_ASSERT("vCamera_Task queue create fail", eEC == ER_OK);

  tCamera_AS.bIs_Camera_Ready = true;

  while(1)
  {
    if(eOSAL_Queue_Get_msg(pCamera_Queue_Handle, &tMsg) == ER_OK)
    {
      switch(tMsg.eMSG)
      {
        default:
          vDEBUG_ASSERT("vCamera_Task invalid msg ID", tMsg.eMSG < CAMERA_MSG_LIMIT);
      }
    }
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void vCamera_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_12)
  {
    //todo VSYNC
  }
  else if(GPIO_Pin == GPIO_PIN_13)
  {
    //todo HREF
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
ERROR_CODE eCamera_Request_Param_Init(tCamera_Request * pParam)
{
  ERROR_CODE eEC = ER_OK;

  memset(pParam, 0x00, sizeof(tCamera_Request));

  pParam->eRequestID = CAMERA_REQUEST_NONE;

  return eEC;
}

/******************************************************************************
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eCamera_Request(tCamera_Request * pRequest)
{
  ERROR_CODE eEC = ER_OK;

  if(pRequest->eRequestID == CAMERA_REQUEST_TASK_PARAM)
  {
    pRequest->pCamera_Task_Param->pTaskFcn        = &vCamera_Task;
    pRequest->pCamera_Task_Param->pName           = cCamera_Task_Name;
    pRequest->pCamera_Task_Param->uiStack_Size    = TASK_CAMERA_STACK_SIZE;
    pRequest->pCamera_Task_Param->pParameters     = NULL;
    pRequest->pCamera_Task_Param->uiTask_Priority = TASK_CAMERA_PRIORITY;

    eEC = ER_OK;
  }
  else
  {
    switch(pRequest->eRequestID)
    {
      case CAMERA_REQUEST_REGISTER_CALLBACK:
      case CAMERA_REQUEST_TAKE_SINGLE_IMAGE:
      case CAMERA_REQUEST_START_CONTINUOUS_IMAGE_CAPTURE:
      case CAMERA_REQUEST_STOP_CONTINUOUS_IMAGE_CAPTURE:
      default:
        eEC = ER_FAIL;
    }
  }

  return eEC;
}

#endif //PROJ_CONFIG_USE_DRVR_CAMERA
#endif //__CAMERA_C__
