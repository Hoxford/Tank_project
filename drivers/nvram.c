/******************************************************************************
*
* nvram.c - driver code and api to interact with the nvram
* Copyright (c) notice
*
******************************************************************************/
#ifndef __NVRAM_C__
#define __NVRAM_C__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "utils_inc/error_codes.h"
#include "utils_inc/proj_debug.h"

#include "drivers_inc/nvram.h"

#include "board.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

// NVRAM index mappings////////////////////////////////
  #define NVRAM_TABLE_INDEX_MEMBERS  8  //max number of modules that can register with NVram
// END NVRAM index mappings////////////////////////////

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
typedef struct tNvram_Index
{
  uint32_t uiID;
  uint32_t uiStartAddress;
  uint32_t uiEndAddress;
  uint32_t uiSize;
}tNvram_Index;

typedef struct tNvram_Activity_State
{
  bool bIs_Nvram_Ready;
  tNvram_Index tNvram_Table_Index[NVRAM_TABLE_INDEX_MEMBERS];
}tNvram_Activity_State;

tNvram_Activity_State tNvram_AS = //nvram activity state
{
  false, //bool bIs_Nvram_Ready;
  //tNvram_Index tNvram_Table_Index[NVRAM_TABLE_INDEX_MEMBERS];
  {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
  },
};

typedef struct tNvram_Write
{
  uint32_t   uiSize;
  uint8_t * pBuff;
}tNvram_Write;

typedef struct tNvram_Read
{
  uint32_t   uiSize;
  uint8_t * pBuff;
}tNvram_Read;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
ERROR_CODE eNvram_Init(void);
ERROR_CODE eNvram_Write(tNvram_Write * pWrite);
ERROR_CODE eNvram_Read(tNvram_Read * pRead);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eNvram_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;
  tBSP_Flash_Read tRead;

  tRead.pBuff = (uint8_t)&tNvram_AS.tNvram_Table_Index[0];
  tRead.uiBuff_Len = sizeof(tNvram_AS.tNvram_Table_Index);
  tRead.uiStart_Addr = 0;

  return eEC;
}

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eNvram_Write(tNvram_Write * pWrite)
{
  ERROR_CODE eEC = ER_FAIL;
  tBSP_Flash_Write tParam;

  tParam.pBuff = pWrite->pBuff;
  tParam.uiBuff_Len = pWrite->uiSize;

  eBSP_FLASH_WRITE(&tParam);

  return eEC;
}

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eNvram_Read(tNvram_Read * pRead)
{
  ERROR_CODE eEC = ER_FAIL;
  tBSP_Flash_Read tParam;

  tParam.pBuff = pRead->pBuff;
  tParam.uiBuff_Len = pRead->uiSize;

  eBSP_FLASH_READ(&tParam);

  return eEC;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eNvram_Request_Param_Init(tNvram_Request * pRequest)
{
  ERROR_CODE eEC = ER_FAIL;

  return eEC;
}

/******************************************************************************
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eNvram_Request(tNvram_Request * pRequest)
{
  ERROR_CODE eEC = ER_FAIL;

  switch(pRequest->eRequestID)
  {
    case NVRAM_INIT:
      if(tNvram_AS.bIs_Nvram_Ready == false)
      {
        eNvram_Init();
      }
      break;
    case NVRAM_REQUEST_READ:
    case NVRAM_REQUEST_WRITE:
    case NVRAM_REQUEST_REGISTER_ID:
    default:
      break;
  }
  return eEC;
}

#endif //__NVRAM_C__
