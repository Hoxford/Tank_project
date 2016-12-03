/******************************************************************************
*
* infrared.h - infrared driver header file
* Copyright (c) notice
*
******************************************************************************/
#ifndef __INFRARED_H__
#define __INFRARED_H__
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
#if (PROJ_CONFIG_USE_DRVR_IR_DETECTION == 0)
  #warning "IR driver not implemented"
#else
/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//#def EXAMPLE_DEF  value  //def description

/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//int iExample_var;  //variable description

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//extern int iExample_extern_var;  //external variable description

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//eExample_enum description
typedef enum IR_REQUEST_ID
{
  IR_REQUEST_NONE = 0,
  IR_REQUEST_LIMIT,
}IR_REQUEST_ID, * pIR_REQUEST_ID;

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//tExample_struct description
typedef struct IR_Request
{
  IR_REQUEST_ID eRequestID;;
}IR_Request_t, * pIR_Request_t;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
ERROR_CODE eIR_Request_Param_Init(pIR_Request_t pParam);
ERROR_CODE eIR_Request(pIR_Request_t pRequest);

#endif //PROJ_CONFIG_USE_DRVR_USB
#endif //__USB_H__
