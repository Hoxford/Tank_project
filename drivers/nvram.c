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
  #define NVRAM_TABLE_INDEX_MEMBERS  16 //max number of modules that can register with NVram including the NVram module
  #define NVRAM_TABLE_INDEX_MEMBER_SIZE 0x2000 //max size in bytes of each table member
  #define NVRAM_DEFAULT_INDEX        0 //Default index to determine if NVram has been pre-set
  #define NVRAM_DEFAULT_ID           0x49BBD06A //NV ram ID
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
typedef struct tNvram_Table_ID
{
  uint32_t uiID;
  uint32_t uiStartAddress;
  uint32_t uiEndAddress;
  uint32_t uiSize;
  uint32_t uiChecksum;
  uint32_t uiID2;
}tNvram_Table_ID;

typedef struct tNvram_Activity_State
{
  bool bIs_Nvram_Ready;
  tNvram_Table_ID tNvram_Table_ID_Index[NVRAM_TABLE_INDEX_MEMBERS];
}tNvram_Activity_State;

tNvram_Activity_State tNvram_AS = //nvram activity state
{
  false, //bool bIs_Nvram_Ready;
  //tNvram_Table_ID tNvram_Table_ID_Index[NVRAM_TABLE_INDEX_MEMBERS];
  {
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
    {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
  },
};

typedef struct tNvram_Write
{
  uint32_t   uiSize;
  uint8_t *  pBuff;
}tNvram_Write;

typedef struct tNvram_Read
{
  uint32_t   uiSize;
  uint8_t *  pBuff;
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
* todo: NAME, DESCRIPTION, PARAM, RETURN
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
  tBSP_Flash_Write tWrite;
  tNvram_Table_ID tDefaultID;
  uint32_t uiStartAddr = 0;

  if(tNvram_AS.bIs_Nvram_Ready == false)
  {
    tRead.pBuff = (uint8_t *)&tDefaultID;
    tRead.uiBuff_Len = sizeof(tNvram_Table_ID);
    tRead.uiStart_Addr = FLASH_SETTINGS_INDEX;
    eBSP_FLASH_READ(&tRead);

    //determine if persistent settings default index is present
    if((tDefaultID.uiID == NVRAM_DEFAULT_ID) &
       (tDefaultID.uiID2 == NVRAM_DEFAULT_ID))
    {
      //validate checksum
      //todo: validate check sum
      if(1)
      {
        tRead.pBuff = (uint8_t *)tNvram_AS.tNvram_Table_ID_Index;
        tRead.uiBuff_Len = sizeof(tNvram_AS.tNvram_Table_ID_Index);
        tRead.uiStart_Addr = FLASH_SETTINGS_INDEX;
        eBSP_FLASH_READ(&tRead);
        tNvram_AS.bIs_Nvram_Ready = true;
      }
      else
      {
        tNvram_AS.bIs_Nvram_Ready = false;
      }
    }
    else
    {
      tNvram_AS.bIs_Nvram_Ready = false;
    }

    if(tNvram_AS.bIs_Nvram_Ready == false)
    {
      eBSP_FLASH_ERASE();

      eBSP_FLASH_GET_START_ADDR(&uiStartAddr);

      tNvram_AS.tNvram_Table_ID_Index[NVRAM_DEFAULT_INDEX].uiID = NVRAM_DEFAULT_ID;
      tNvram_AS.tNvram_Table_ID_Index[NVRAM_DEFAULT_INDEX].uiStartAddress = uiStartAddr;
      tNvram_AS.tNvram_Table_ID_Index[NVRAM_DEFAULT_INDEX].uiEndAddress = uiStartAddr + sizeof(tNvram_AS.tNvram_Table_ID_Index);
      tNvram_AS.tNvram_Table_ID_Index[NVRAM_DEFAULT_INDEX].uiSize = sizeof(tNvram_AS.tNvram_Table_ID_Index);
      tNvram_AS.tNvram_Table_ID_Index[NVRAM_DEFAULT_INDEX].uiChecksum = 0;
      tNvram_AS.tNvram_Table_ID_Index[NVRAM_DEFAULT_INDEX].uiID2 = NVRAM_DEFAULT_ID;

      tWrite.pBuff = (uint8_t *)tNvram_AS.tNvram_Table_ID_Index;
      tWrite.uiBuff_Len = sizeof(tNvram_AS.tNvram_Table_ID_Index);

      eBSP_FLASH_WRITE(&tWrite);

      //todo: read back flash and verify default was set
      if(1)
      {
        tNvram_AS.bIs_Nvram_Ready = true;
      }
      else
      {
        tNvram_AS.bIs_Nvram_Ready = false;
        eEC = ER_NOT_SET;
      }
    }
  }

  if(tNvram_AS.bIs_Nvram_Ready == true)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
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
* todo: NAME, DESCRIPTION, PARAM, RETURN
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
  int i;
  void * pRtn;

  if(pRequest == NULL)
  {
    eEC = ER_PARAM;
  }
  else
  {
    i = sizeof(tNvram_Request);
    pRtn = memset(pRequest, 0x00, i);
    if(pRtn == pRequest)
    {
      eEC = ER_OK;
    }
    else
    {
      eEC = ER_FAIL;
    }
  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
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
    case NVRAM_REQUEST_INIT:
      if(tNvram_AS.bIs_Nvram_Ready == false)
      {
        eEC = eNvram_Init();
      }
      else
      {
        eEC = ER_OK;
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
