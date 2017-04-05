/******************************************************************************
* camera_OV7670.c
*
*  Created on: Mar 24, 2017
*      Author: Matt Buonadonna
*      Copyright (c) notice
*
******************************************************************************/
#ifndef __CAMERA_OV7670_C__
#define __CAMERA_OV7670_C__
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
  /* Project specific include files here */

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  /* Utility include files here */

//Third party includes
  #include "ThirdParty_inc/osal.h"
  /* Third party include files here */

//Driver includes
  #include "drivers_inc/camera.h"
  #include "drivers_inc/camera_OV7670.h"
  #include "drivers_inc/camera_OV7670_reg.h"
  /* Driver include files here */

//Platform includes
  #include "board.h"
  /* Platform include files here */


/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
static ERROR_CODE eOV7670_Setup(pCamera_App_API pApi);
static ERROR_CODE eOV7670_Take_Pic(uint8_t * pBuff, uint32_t u32BuffLen);
static ERROR_CODE eOV7670_Stream_Pic(uint8_t * pBuff, uint32_t u32BuffLen);

Camera_Device_API_t Cam_Dev_API_t =
{
  .eCamDeviceSetup   = &eOV7670_Setup,
  .eCamTakePicture   = &eOV7670_Take_Pic,
  .eCamStreamPicture = &eOV7670_Stream_Pic,
};
/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static ERROR_CODE eOV7670_Setup(pCamera_App_API pApi)
{
  ERROR_CODE eEC = ER_OK;
  vDEBUG_ASSERT("", eEC == ER_OK);
  return eEC;
}

static ERROR_CODE eOV7670_Take_Pic(uint8_t * pBuff, uint32_t u32BuffLen)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", eEC == ER_OK);
  return eEC;
}

static ERROR_CODE eOV7670_Stream_Pic(uint8_t * pBuff, uint32_t u32BuffLen)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", eEC == ER_OK);
  return eEC;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#endif //PROJ_CONFIG_USE_DRVR_CAMERA
#endif //__CAMERA_OV7670_C__
