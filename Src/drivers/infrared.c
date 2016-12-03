/******************************************************************************
*
* infrared.c - Infrared driver source code.
* Copyright (c) notice
*
******************************************************************************/
#ifndef __INFRARED_C__
#define __INFRARED_C__
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
  #include "drivers_inc/infrared.h"
  /* Driver include files here */

//Application includes
  #include "app_inc/commander.h"
  /* Application include files here */

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
typedef enum IR_MSG_ID
{
  IR_MSG_NONE,
  IR_MSG_LIMIT,
}IR_MSG_ID, * pIR_MSG_ID;

typedef enum IR_STATE
{
  IR_STATE_NONE,
  IR_STATE_LIMIT,
}IR_STATE, * pIR_STATE;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
typedef struct IR_Activity_State
{
  IR_STATE  eState;
}IR_Activity_State_t, * pIR_Activity_State;

IR_Activity_State_t IR_AS_t =
{
  .eState =
};

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
extern int external_example_function(param); //short external function description
/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
bool           bExample_PrivateFunction(param_1, param_2); //short function declaration description
int            iExample_PrivateFunction(param_1, param_2); //short function declaration description
Example_Struct pExample_PrivateFunction(param_1, param_2); //short function declaration description

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
bool bExample_PrivateFunction(param_1, param_2)
{
  bool bFoo = false;
  return bFoo;
}
/**/
int iExample_PrivateFunction(param_1, param_2)
{
  int iFoo = 0;
  return iFoo;
}
/**/
Example_Struct pExample_PrivateFunction(param_1, param_2)
{
  Example_Struct pStruct = NULL;
  pStruct = malloc(sizeof(tExample_Struct, ));
  return pStruct;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

bool bFilename_or_abreviation_funciton(param_1, param_2)
{
  bool bFoo = false;
  return bFoo;
}
/**/
int iFilename_or_abreviation_funciton(param_1, param_2)
{
  int iFoo = 0;
  return iFoo;
}
/**/
Example_Struct pFilename_or_abreviation_funciton(param_1, param_2)
{
  Example_Struct pStruct = NULL;
  pStruct = malloc(sizeof(tExample_Struct, ));
  return pStruct;
}

#endif //PROJ_CONFIG_USE_DRVR_IR_DETECTION
#endif //__FILE_NAME_C__
