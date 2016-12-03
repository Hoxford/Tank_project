/******************************************************************************
*
* project_config.h - project configuration file
* Copyright (c) notice
*
******************************************************************************/
#ifndef __PROJECT_CONFIG_H__
#define __PROJECT_CONFIG_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//Project application configuration defines BEGIN
  #define PROJ_CONFIG_USE_APP_COMMANDER         1
//Project application configuration defines END

//Project driver configuration defines BEGIN
  #define PROJ_CONFIG_USE_DRVR_NVRAM            0
  #define PROJ_CONFIG_USE_DRVR_WIFI             0
  #define PROJ_CONFIG_USE_DRVR_BLUETOOTH        0
  #define PROJ_CONFIG_USE_DRVR_USB              0
  #define PROJ_CONFIG_USE_DRVR_IR_DETECTION     0
  #define PROJ_CONFIG_USE_DRVR_CAMERA           0
//Project driver configuration defines END


//Project utility configuration defines BEGIN
  #define PROJ_CONFIG_USE_UTIL_DEBUG            1
  #define PROJ_CONFIG_USE_UTIL_OSAL             1

  #define PROJ_CONFIG_OS_NONE                   0
  #define PROJ_CONFIG_OS_TIRTOS                 1
  #define PROJ_CONFIG_OS_FREERTOS               2
  #define PROJ_CONFIG_OS_CHIBIOS                3
  #define PROJ_CONFIG_OS_THREADX                4
  /*add more PROJ_CONFIG_OS_... here*/
    #define PROJ_CONFIG_OS     PROJ_CONFIG_OS_FREERTOS
//Project utility configuration defines END


//Project platform configuration defines BEGIN
  //PCB platform
    #define PROJ_CONFIG_PLATFORM_STM32F4DISC    1
    #define PROJ_CONFIG_PLATFORM_NUCLEOF401RE   2
    #define PROJ_CONFIG_PLATFORM_NUCLEOF412ZG   3
    /*add more PROJ_CONFG_PLAFTORM_... here*/
      #define PROJ_CONFIG_PLATFORM  PROJ_CONFIG_PLATFORM_NUCLEOF412ZG

  //Wifi module
    #define PROJ_CONFIG_PLATFORM_WIFI_WINC3400  1
    /*add more PROJ_CONFIG_PLATFORM_WIFI_... here*/
      #define PROJ_CONFIG_PLATFORM_WIFI PROJ_CONFIG_PLATFORM_WIFI_WINC3400

  //Bluetooth module
    #define PROJ_CONFIG_PLATFORM_BT_WINC3400    1
    /*add more PROJ_CONFIG_PLATFORM_BT_... here*/
      #define PROJ_CONFIG_PLATFORM_BT PROJ_CONFIG_PLATFORM_BT_WINC3400


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


#endif //__PROJECT_CONFIG_H__
