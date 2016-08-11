/******************************************************************************
*
* main_proj.h - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __MAIN_PROJ_H__
#define __MAIN_PROJ_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//Project application configuration defines BEGIN
  #define PROJ_CONFIG_USE_APP_COMMANDER               0
//Project application configuration defines END

//Project driver configuration defines BEGIN
  #define PROJ_CONFIG_USE_DRVR_PERSISTENT_SETTINGS    1
  #define PROJ_CONFIG_USE_DRVR_WIFI                   0
  #define PROJ_CONFIG_USE_DRVR_USB                    0
  #define PROJ_CONFIG_USE_DRVR_CAMERA                 0
//Project driver configuration defines END


//Project utility configuration defines BEGIN
  #define PROJ_CONFIG_USE_UTIL_DEBUG    1
  #define PROJ_CONFIG_USE_UTIL_OSAL     1

  #define PROJ_CONFIG_OS_TIRTOS      1
  #define PROJ_CONFIG_OS_FREERTOS    2
  #define PROJ_CONFIG_OS_CHIBIOS     3
  #define PROJ_CONFIG_OS_THREADX     4
  /*add more PROJ_CONFIG_OS_... here*/
    #define PROJ_CONFIG_OS     PROJ_CONFIG_OS_FREERTOS
//Project utility configuration defines END


//Project platform configuration defines BEGIN
  #define PROJ_CONFIG_PLATFORM_STM32F4DISC    1
  /*add more PROJ_CONFG_PLAFTORM_... here*/
    #define PROJ_CONFIG_PLATFORM  PROJ_CONFIG_PLATFORM_STM32F4DISC
//Project platform configuration defines END

/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
void main_proj(void);  //Project source code start function

#endif //__MAIN_PROJ_H__