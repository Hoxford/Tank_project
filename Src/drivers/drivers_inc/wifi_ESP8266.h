/******************************************************************************
*
* file_name.h - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __FILE_NAME_H__
#define __FILE_NAME_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#include "file.h"

/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#def EXAMPLE_DEF  value  //def description

/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
int iExample_var;  //variable description

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
extern int iExample_extern_var;  //external variable description

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//PUB_EXAMPLE_ENUM description
typedef enum PUB_EXAMPLE_ENUM
{
    PUB_ENUM_A,
    PUB_ENUM_B,
    PUB_ENUM_C,
}PUB_EXAMPLE_ENUM, * pPUB_EXAMPLE_ENUM;

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//Pub_Example_Struct_t description
typedef struct Pub_Example_Struct_t
{
  int              iVar1;
  int *            pVar2;
  bool             bVar3;
  PUB_EXAMPLE_ENUM eENUM;
  void (* pCallback_Fcn)(void * param);
}Pub_Example_Struct_t, * pPub_Example_Struct;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
extern int external_example_function(int param); //short external function description

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*todo: Name: [put name here]
*todo: Description:
*   [put description here]
*
*todo: Parameters:
*   (type) name: description (in order from left to right)
*     = value(optional): description if value set
*     -or-
*     ->(type) name: description if member of struct
*       = value(optional): description if member of struct value set
*   example:
*   (bool) bVar: do action option
*     = false: do action when set to false
*     = true: do other action when set to true
*   (pPub_Example_Struct) pParam: pointer to the param structure
*     ->(int)iVar1: description
*     ->(int *)pVar2: description
*     ->(bool)bVar3: description
*     ->(PUB_EXAMPLE_ENUM)eENUM: description
*       = PUB_ENUM_A: do action when set to _A
*       = PUB_ENUM_B: do action when set to _B
*       = PUB_ENUM_C: do action when set to _C
*     ->void (* pCallback_Fcn)(void * param): Function pointer for callback.
*
*todo: Returns:
*   (type): description
*     = value (optional): value description
*   examples:
*   (bool):
*     = true: did function action and result is true
*     = false: did function action and result is false
*   (int): integer value description after function action
*   (pPub_Example_Struct):
*     = (uint32_t *): address of the created object
*     =             - NULL: created object fail
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
int iFilename_or_abreviation_funciton(bool bVar, pPub_Example_Struct pParam);  //breif function description

#endif //__FILE_NAME_H__
