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

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  #include "utils_inc/clock.h"
  #include "utils_inc/ring_buffer.h"
  /* Utility include files here */

//Third party includes
  #include "ThirdParty_inc/osal.h"

//Driver includes
  #include "drivers_inc/wifi.h"
  #include "drivers_inc/bluetooth.h"
  #include "drivers_inc/camera.h"

//Application includes
  #include "app_inc/commander.h"

//Platform includes
  #include "board.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define TASK_CAMERA_PRIORITY 4
#define TASK_CAMERA_STACK_SIZE 4096/sizeof(uint32_t) //size in bytes divided by RTOS stack type, portSTACK_TYPE

#define vCamera_GPIO_EXTI_Callback  HAL_GPIO_EXTI_Callback

#define CAMERA_PIC_WIDTH             174
#define CAMERA_PIC_HEIGHT            144
#define CAMERA_PIC_BYTES_PER_PIXEL   2
#define CAMERA_PIC_BUFF_SIZE         (CAMERA_PIC_WIDTH * CAMERA_PIC_HEIGHT * CAMERA_PIC_BYTES_PER_PIXEL)
#define CAMERA_PIC_NUM_BUFFERS       2

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

const  char        cCamera_Task_Name[] = "CAMERA";
static RB_HANDLE   hRingBuffPic = 0;

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
typedef struct Camera_Activity_State
{
  bool      bIs_Camera_Ready;
  bool      bIs_Camera_In_Continuouis_Mode;
  uint8_t * pPicBuff;
}Camera_Activity_State_t, * pCamera_Activity_State;

Camera_Activity_State_t Camera_AS_t =
{
  .bIs_Camera_Ready               = false,
  .bIs_Camera_In_Continuouis_Mode = false,
  .pPicBuff                       = NULL,
};

typedef struct Camera_Message_Struct
{
  CAMERA_MSG_ID eMSG;
}Camera_Message_Struct_t, * pCamera_Message_Struct;

extern Camera_Device_API_t Cam_Dev_API_t;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static ERROR_CODE eCamera_Receive_CB(uint8_t * pBuff, uint32_t u32BuffLen);
static ERROR_CODE eCamera_Picture_Receive_CB(uint8_t * pBuff, uint32_t u32BuffLen);
static ERROR_CODE eCamera_Stream_Receive_CB(uint8_t * pBuff, uint32_t u32BuffLen);
static ERROR_CODE eCamera_App_Dev_Status_CB(CAMERA_DEV_STATUS eStatus);
static void vCamera_Task(void * pvParameters);

Camera_App_API_t Cam_App_API_t =
{
  .eCamAppRcvCallBack        = &eCamera_Receive_CB,
  .eCamAppRcvPictureCallback = &eCamera_Picture_Receive_CB,
  .eCamAppRcvStreamCallback  = &eCamera_Stream_Receive_CB,
  .eCamAppDevStatusCallBack  = &eCamera_App_Dev_Status_CB,
};

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static ERROR_CODE eCamera_Receive_CB(uint8_t * pBuff, uint32_t u32BuffLen)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", eEC = ER_OK);
  return eEC;
}

static ERROR_CODE eCamera_Picture_Receive_CB(uint8_t * pBuff, uint32_t u32BuffLen)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", eEC = ER_OK);
  return eEC;
}

static ERROR_CODE eCamera_Stream_Receive_CB(uint8_t * pBuff, uint32_t u32BuffLen)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", eEC = ER_OK);
  return eEC;
}

static ERROR_CODE eCamera_App_Dev_Status_CB(CAMERA_DEV_STATUS eStatus)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", eEC = ER_OK);
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
  OSAL_Queue_Parameters_t  Camera_Queue_Param_t;
  pOSAL_Queue_Handle       pCamera_Queue_Handle;
  Camera_Message_Struct_t  Msg_t;

  eOSAL_Queue_Params_Init(&Camera_Queue_Param_t);
  Camera_Queue_Param_t.uiNum_Of_Queue_Elements = 3;
  Camera_Queue_Param_t.uiSize_Of_Queue_Element = sizeof(Camera_Message_Struct_t);
  Camera_Queue_Param_t.pMsgBuff                = &Msg_t;
  Camera_Queue_Param_t.iTimeout                = OSAL_QUEUE_TIMEOUT_WAITFOREVER;

  eEC = eOSAL_Queue_Create(&Camera_Queue_Param_t, &pCamera_Queue_Handle);
  vDEBUG_ASSERT("vCamera_Task queue create fail", eEC == ER_OK);

  if(eEC == ER_OK)
  {
    //Perform camera device setup
    //
    eEC = Cam_Dev_API_t.eCamDeviceSetup(&Cam_App_API_t);
    vDEBUG_ASSERT("vCamera_Task camera setup fail", eEC == ER_OK);
  }

  if(eEC == ER_OK)
  {
    //set up the camera image ring buffer
    //
    eEC = eRingBuff_App_Create(&hRingBuffPic, CAMERA_PIC_NUM_BUFFERS, CAMERA_PIC_BUFF_SIZE);
    vDEBUG_ASSERT("Cam pic ring buff create fail", eEC == ER_OK);
    eRingBuff_App_Get_Buff(hRingBuffPic, &Camera_AS_t.pPicBuff);
  }

  Camera_AS_t.bIs_Camera_Ready = true;

  while(1)
  {
    if(eOSAL_Queue_Get_msg(pCamera_Queue_Handle, &Msg_t) == ER_OK)
    {
      switch(Msg_t.eMSG)
      {
        default:
          vDEBUG_ASSERT("vCamera_Task invalid msg ID", Msg_t.eMSG < CAMERA_MSG_LIMIT);
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
ERROR_CODE eCamera_Request_Param_Init(pCamera_Request pParam)
{
  ERROR_CODE eEC = ER_OK;

  memset(pParam, 0x00, sizeof(Camera_Request_t));

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
ERROR_CODE eCamera_Request(pCamera_Request pRequest)
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
