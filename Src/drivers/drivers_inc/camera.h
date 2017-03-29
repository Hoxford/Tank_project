/******************************************************************************
*
* camera.h - Driver code for the project camera
* Copyright (c) notice
*
******************************************************************************/
#ifndef __CAMERA_H__
#define __CAMERA_H__
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
  #warning "Camera driver not implemented"

#define eCamera_Request_Param_Init(p)   ER_NOT_ENABLED
#define eCamera_Request(p)              ER_NOT_ENABLED
#define Camera_Request_t                void *
#else

/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef enum CAMERA_REQUEST_ID
{
  CAMERA_REQUEST_NONE = 0,
  CAMERA_REQUEST_TASK_PARAM,
  CAMERA_REQUEST_REGISTER_CALLBACK,
  CAMERA_REQUEST_TAKE_SINGLE_IMAGE,
  CAMERA_REQUEST_START_CONTINUOUS_IMAGE_CAPTURE,
  CAMERA_REQUEST_STOP_CONTINUOUS_IMAGE_CAPTURE,
  CAMERA_REQUEST_LIMIT,
}CAMERA_REQUEST_ID, * pCAMERA_REQUEST_ID;

typedef enum CAMERA_DEV_STATUS
{
  CAM_DEV_STATE_NONE,
  CAM_DEV_STATE_OK,
  CAM_DEV_STATE_ERROR,
  CEM_DEV_STATE_STREAMING,
  CAM_DEV_STATE_LIMIT,
}CAMERA_DEV_STATUS, * pCAMERA_DEV_STATUS;

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct Camera_Request
{
  CAMERA_REQUEST_ID eRequestID;
  pOSAL_Task_Parameters pCamera_Task_Param;
}Camera_Request_t, *pCamera_Request;

typedef struct Camera_App_API
{
  ERROR_CODE (* eCamAppRcvCallBack)(uint8_t * pBuff, uint32_t u32BuffLen);
  ERROR_CODE (* eCamAppRcvPictureCallback)(uint8_t * pBuff, uint32_t u32BuffLen);
  ERROR_CODE (* eCamAppRcvStreamCallback)(uint8_t * pBuff, uint32_t u32BuffLen);
  ERROR_CODE (* eCamAppDevStatusCallBack)(CAMERA_DEV_STATUS eSTATUS);
}Camera_App_API_t, * pCamera_App_API;

typedef struct Camera_Device_API
{
  ERROR_CODE (* eCamDeviceSetup)(pCamera_App_API pApi);
  ERROR_CODE (* eCamTakePicture)(uint8_t * pBuff, uint32_t u32BuffLen);
  ERROR_CODE (* eCamStreamPicture)(uint8_t * pBuff, uint32_t u32BuffLen);
}Camera_Device_API_t, * pCamera_Device_API;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eCamera_Request_Param_Init(pCamera_Request pParam);
ERROR_CODE eCamera_Request(pCamera_Request pRequest);

#endif //PROJ_CONFIG_USE_DRVR_CAMERA
#endif //__CAMERA_H__
