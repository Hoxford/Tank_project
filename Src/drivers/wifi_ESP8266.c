/******************************************************************************
*
* wifi_ESP8266.c - Wifi module ESP8266 specific driver code.
* Copyright (c) notice
*
******************************************************************************/
#ifndef __WIFI_ESP8266_C__
#define __WIFI_ESP8266_C__
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
#if(PROJ_CONFIG_PLATFORM_WIFI == PROJ_CONFIG_PLATFORM_WIFI_ESP8266)
  /* Project specific include files here */

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  /* Utility include files here */

//Driver includes
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

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: [put name here]
* description:
*   [put description here]
* parameters:
*   type - value: value description (in order from left to right)
*   example:
*   bool - true: do action when set to true
*        - false: do action when set to false
*   Example_Struct - pStruct:
*     (int)->iFoo: Structure member description
*     (uint32_t *)->pFoo: Structure member description
*     (eEXAMPLE_ENUM)->eFOO: Structure member description
*
* return value description:
*   type - value: value description
*   examples:
*   bool - true: did function action and result is true
*        - false: did function action and result is false
*   uint32_t - : integer value after function action
*   Example_Struct - *: address of the created object
*                  - NULL: created object fail
******************************************************************************/


/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#endif //(PROJ_CONFIG_PLATFORM_WIFI == PROJ_CONFIG_PLATFORM_WIFI_ESP8266)
#endif //__WIFI_ESP8266_C__