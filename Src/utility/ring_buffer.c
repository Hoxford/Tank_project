/******************************************************************************
*
* ring_buffer.c - ring buffer utility source file. This ring buffer
*   implementation is dynamic and does not need a static buffer to create
* Copyright (c) notice
*
******************************************************************************/
#ifndef __RING_BUFFER_C__
#define __RING_BUFFER_C__
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
  /* Project specific include files here */

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  #include "utils_inc/ring_buffer.h"
  /* Utility include files here */

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

typedef enum RING_BUff_TYPE
{
  RB_TYPE_NONE,
  RB_TYPE_MANAGED,
  RB_TYPE_APP,
  RB_TYPE_LIMIT,
}RING_BUff_TYPE, RB_TYPE, * pRB_TYPE;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct Ring_Buffer_Descriptor
{
  RB_TYPE   eType;
  uint32_t  ui32NumOfBuffers;
  uint32_t  ui32NumBuffersFilled;
  uint32_t  ui32BuffSize;
  uint8_t * pFirstBuffer;
  uint8_t * pLastBuffer;
  uint8_t * pHead;
  uint8_t * pTail;
}Ring_Buffer_Descriptor_t, RB_Desc_t, * pRB_Desc;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//static ERROR_CODE eRegister_Ring_Buff(RB_HANDLE Handle);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
ERROR_CODE eRingBuff_Create(RB_HANDLE * pHandle, uint32_t ui32NumOfBuffers, uint32_t ui32BuffSize)
{
  ERROR_CODE eEC = ER_FAIL;
  uint32_t   ui32RingBuffDataSize = 0;
  uint32_t   ui32RingBuffTotalSize = 0;
  RB_HANDLE  Handle = 0;
  RB_Desc_t  Desc;
  uint8_t *  pCheck;

  if(pHandle == NULL)
    eEC = ER_PARAM1;
  else if(ui32NumOfBuffers == 0)
    eEC = ER_PARAM2;
  else if(ui32BuffSize == 0)
    eEC = ER_PARAM3;
  else
  {
    ui32RingBuffDataSize = ui32NumOfBuffers * ui32BuffSize;
    ui32RingBuffTotalSize = ui32RingBuffDataSize + sizeof(Ring_Buffer_Descriptor_t);
    Handle = (uint32_t)calloc(ui32RingBuffTotalSize, sizeof(uint8_t));
    if(Handle == 0)
    {
      eEC = ER_NOMEM;
    }
    else
    {
      Desc.eType = RB_TYPE_MANAGED;
      Desc.ui32NumOfBuffers = ui32NumOfBuffers;
      Desc.ui32NumBuffersFilled = 0;
      Desc.ui32BuffSize = ui32BuffSize;
      Desc.pHead = Desc.pTail = Desc.pFirstBuffer = (uint8_t *)(Handle + sizeof(Ring_Buffer_Descriptor_t));
      Desc.pLastBuffer = (uint8_t *)(Desc.pFirstBuffer + ui32BuffSize * (ui32NumOfBuffers - 1));
      pCheck = memcpy((uint8_t *)Handle, &Desc, sizeof(Ring_Buffer_Descriptor_t));
      if(Handle == (RB_HANDLE)pCheck)
      {
        *pHandle = Handle;
        eEC = ER_OK;
      }
    }
  }
  return eEC;
}

ERROR_CODE eRingBuff_Reset_All(RB_HANDLE Handle)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", false);
  return eEC;
}

ERROR_CODE eRingBuff_Destroy(RB_HANDLE Handle)
{
  ERROR_CODE eEC = ER_FAIL;
  pRB_Desc   pDesc;
  uint32_t   ui32RingBuffSize = 0;
  uint8_t *  pCheck;

  if(Handle == 0)
    eEC = ER_PARAM;
  else
  {
    pDesc = (pRB_Desc)Handle;
    ui32RingBuffSize = (pDesc->ui32BuffSize * pDesc->ui32NumOfBuffers) + sizeof(Ring_Buffer_Descriptor_t);
    pCheck = memset(pDesc, 0x00, ui32RingBuffSize);
    if(pCheck == (uint8_t *)pDesc)
    {
      free(pDesc);
      eEC = ER_OK;
    }
  }
  return eEC;
}

ERROR_CODE eRingBuff_Fill_Next_Free(RB_HANDLE Handle, uint8_t * pBuffer, uint32_t ui32Len)
{
  ERROR_CODE eEC = ER_FAIL;
  pRB_Desc   pDesc;

  //Check the parameters
  //
  if(Handle == 0)
    eEC = ER_PARAM1;
  else if(pBuffer == NULL)
    eEC = ER_PARAM2;
  else
  {
    pDesc = (pRB_Desc)Handle;
    if((ui32Len == 0) | (ui32Len > pDesc->ui32BuffSize))
      eEC = ER_PARAM3;
    else if(pDesc->eType != RB_TYPE_MANAGED)
      eEC = ER_INVALID;
    else
    {
      if(pDesc->ui32NumBuffersFilled == pDesc->ui32NumOfBuffers)
        eEC = ER_FULL;
      else
      {
        memcpy(pDesc->pHead, pBuffer, ui32Len);
        pDesc->ui32NumBuffersFilled++;
        //Check if the number of buffers is still less then max and it is okay
        //to move the head pointer.
        if(pDesc->ui32NumBuffersFilled < pDesc->ui32NumOfBuffers)
        {
          if(pDesc->pHead == pDesc->pLastBuffer)
          {
            pDesc->pHead = pDesc->pFirstBuffer;
          }
          else
          {
            pDesc->pHead += pDesc->ui32BuffSize;
          }
        }
        eEC = ER_OK;
      }
//      //check corner cases to make sure the ring buffer can accept new data
//      //
//      if((pDesc->pHead == pDesc->pLastBuffer) &
//         (pDesc->pTail == pDesc->pFirstBuffer))
//      {
//        eEC = ER_FULL;
//      }
//      else if((pDesc->pHead != pDesc->pLastBuffer) &
//             ((pDesc->pHead + pDesc->ui32BuffSize) == pDesc->pTail))
//      {
//        eEC = ER_FULL;
//      }
//      else
//      {
//        memcpy(pDesc->pHead, pBuffer, ui32Len);
//        if(pDesc->pHead == pDesc->pLastBuffer)
//          pDesc->pHead = pDesc->pFirstBuffer;
//        else
//          pDesc->pHead += pDesc->ui32BuffSize;
//        eEC = ER_OK;
//      }
    }
  }
  return eEC;
}

ERROR_CODE eRingBuff_Get_Next_Full(RB_HANDLE Handle, uint8_t * pBuffer)
{
  ERROR_CODE eEC = ER_FAIL;
  pRB_Desc   pDesc;

  //Check the parameters
  //
  if(Handle == 0)
    eEC = ER_PARAM1;
  else if(pBuffer == NULL)
    eEC = ER_PARAM2;
  else
  {
    pDesc = (pRB_Desc)Handle;
    if(pDesc->ui32NumBuffersFilled == 0)
      eEC = ER_NO_BUFF;
    else
    {
      memcpy(pBuffer, pDesc->pTail, pDesc->ui32BuffSize);
      memset(pDesc->pTail, 0x00, pDesc->ui32BuffSize);
      //Check corner case where the entire ring buffer is filled and if the
      //head needs to change position.
      if(pDesc->ui32NumBuffersFilled == pDesc->ui32NumOfBuffers)
      {
        if(pDesc->pHead == pDesc->pLastBuffer)
        {
          pDesc->pHead = pDesc->pFirstBuffer;
        }
        else
        {
          pDesc->pHead += pDesc->ui32BuffSize;
        }
      }
      pDesc->ui32NumBuffersFilled--;

      if(pDesc->pTail == pDesc->pLastBuffer)
        pDesc->pTail = pDesc->pFirstBuffer;
      else
        pDesc->pTail += pDesc->ui32BuffSize;

      eEC = ER_OK;
    }
  }
  return eEC;
}

ERROR_CODE eRingBuff_App_Create(RB_HANDLE * pHandle, uint32_t ui32NumOfBuffers, uint32_t ui32BuffSize)
{
  ERROR_CODE eEC = ER_FAIL;
  uint32_t   ui32RingBuffDataSize = 0;
  uint32_t   ui32RingBuffTotalSize = 0;
  RB_HANDLE  Handle = 0;
  RB_Desc_t  Desc;
  uint8_t *  pCheck;

  if(pHandle == NULL)
    eEC = ER_PARAM1;
  else if(ui32NumOfBuffers == 0)
    eEC = ER_PARAM2;
  else if(ui32BuffSize == 0)
    eEC = ER_PARAM3;
  else
  {
    ui32RingBuffDataSize = ui32NumOfBuffers * ui32BuffSize;
    ui32RingBuffTotalSize = ui32RingBuffDataSize + sizeof(Ring_Buffer_Descriptor_t);
    Handle = (uint32_t)calloc(ui32RingBuffTotalSize, sizeof(uint8_t));
    if(Handle == 0)
    {
      eEC = ER_NOMEM;
    }
    else
    {
      Desc.eType = RB_TYPE_APP;
      Desc.ui32NumOfBuffers = ui32NumOfBuffers;
      Desc.ui32NumBuffersFilled = 0;
      Desc.ui32BuffSize = ui32BuffSize;
      Desc.pHead = Desc.pTail = Desc.pFirstBuffer = (uint8_t *)(Handle + sizeof(Ring_Buffer_Descriptor_t));
      Desc.pLastBuffer = (uint8_t *)(Desc.pFirstBuffer + ui32BuffSize * (ui32NumOfBuffers - 1));
      pCheck = memcpy((uint8_t *)Handle, &Desc, sizeof(Ring_Buffer_Descriptor_t));
      if(Handle == (RB_HANDLE)pCheck)
      {
        *pHandle = Handle;
        eEC = ER_OK;
      }
    }
  }
  return eEC;
}

ERROR_CODE eRingBuff_App_Get_Buff(RB_HANDLE Handle, uint8_t ** ppBuffer)
{
  ERROR_CODE eEC = ER_FAIL;
  pRB_Desc   pDesc;

  //Check the parameters
  //
  if(Handle == 0)
    eEC = ER_PARAM1;
  else if(ppBuffer == NULL)
    eEC = ER_PARAM2;
  else
  {
    pDesc = (pRB_Desc)Handle;
    if(pDesc->eType != RB_TYPE_APP)
      eEC = ER_INVALID;
    else
    {
      if(pDesc->ui32NumBuffersFilled == pDesc->ui32NumOfBuffers)
      {
        *ppBuffer = NULL;
        eEC = ER_FULL;
      }
      else
      {
        *ppBuffer = pDesc->pHead;
        pDesc->ui32NumBuffersFilled++;
        //Check if the number of buffers is still less then max and it is okay
        //to move the head pointer.
        if(pDesc->ui32NumBuffersFilled < pDesc->ui32NumOfBuffers)
        {
          if(pDesc->pHead == pDesc->pLastBuffer)
          {
            pDesc->pHead = pDesc->pFirstBuffer;
          }
          else
          {
            pDesc->pHead += pDesc->ui32BuffSize;
          }
        }
        eEC = ER_OK;
      }
    }
  }
  return eEC;
}

ERROR_CODE eRingBuff_App_Read_Buff(RB_HANDLE Handle, uint8_t * pBuffer)
{
  ERROR_CODE eEC = ER_FAIL;
  pRB_Desc   pDesc;

  //Check the parameters
  //
  if(Handle == 0)
  {
    eEC = ER_PARAM1;
  }
  else if(pBuffer == NULL)
  {
    eEC = ER_PARAM2;
  }
  else
  {
    pDesc = (pRB_Desc)Handle;
    if(pDesc->eType != RB_TYPE_APP)
      eEC = ER_INVALID;
    else if(pDesc->ui32NumBuffersFilled == 0)
      eEC = ER_NO_BUFF;
    else
    {
      memcpy(pBuffer, pDesc->pTail, pDesc->ui32BuffSize);
      memset(pDesc->pTail, 0x00, pDesc->ui32BuffSize);
      //Check corner case where the entire ring buffer is filled and if the
      //head needs to change position.
      if(pDesc->ui32NumBuffersFilled == pDesc->ui32NumOfBuffers)
      {
        if(pDesc->pHead == pDesc->pLastBuffer)
        {
          pDesc->pHead = pDesc->pFirstBuffer;
        }
        else
        {
          pDesc->pHead += pDesc->ui32BuffSize;
        }
      }
      pDesc->ui32NumBuffersFilled--;

      if(pDesc->pTail == pDesc->pLastBuffer)
      {
        pDesc->pTail = pDesc->pFirstBuffer;
      }
      else
      {
        pDesc->pTail += pDesc->ui32BuffSize;
      }

      eEC = ER_OK;
    }
  }
  return eEC;
}

#endif //__RING_BUFFER_C__
