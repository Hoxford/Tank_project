/******************************************************************************
*
* file_name.h - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __CAMERA_H__
#define __CAMERA_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

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
}CAMERA_REQUEST_ID;

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct tCamera_Request
{
  CAMERA_REQUEST_ID eRequestID;
  tOSAL_Task_Parameters * pCamera_Task_Param;
}tCamera_Request;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eCamera_Request_Param_Init(tCamera_Request * pParam);
ERROR_CODE eCamera_Request(tCamera_Request * pRequest);
#endif //__CAMERA_H__