/******************************************************************************
*
 osal.c - operating system abstraction layer
* Copyright (c) notice
*
******************************************************************************/
#ifndef __OSAL_C__
#define __OSAL_C__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils_inc/error_codes.h"
#include "utils_inc/osal.h"

#if defined(USING_TIRTOS)
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#define OSAL_MAILBOX_DEFAULT_WAIT_FOREVER 0xFFFFFFFF
#define OSAL_MAILBOX_DEFAULT_BUFF_SIZE    256
#define OSAL_MAILBOX_DEFAULT_MAX_MESSAGES 3
/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
bool bIs_OS_running = false;
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
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: eOSAL_OS_start
* description: OSAL start abstraction function. For most operating systems
*              this function will not return once called. Do not rely on it
*              to return an error code.
* param description: none
* return value description: ERROR_CODE - ER_OK: Most OS's do not return once
*                                        their OS start function was called.
*                                        Do not rely on this return value.
******************************************************************************/
ERROR_CODE eOSAL_OS_start(void)
{
  ERROR_CODE eEC = ER_OK;

  bIs_OS_running = true;

  //Start the TI-RTOS, will not return
  //
  BIOS_start();

  return eEC;
}

ERROR_CODE eOSAL_Is_OS_Running(void)
{
  ERROR_CODE eEC = ER_FALSE;

  if(bIs_OS_running == true)
  {
    eEC = ER_TRUE;
  }
  else
  {
    eEC = ER_FALSE;
  }
  return eEC;
}

/******************************************************************************
* name: eOSAL_delay
* description: delays for <delay parameter> number of milliseconds. Will call
*              either a hardware or OS delay depending on if the OS is running
*              or not.
* param description: uint32_t : the number of milliseconds to delay
*                    uint32_t - pointer: pointer to variable that will contain
*                    the number of milliseconds delayed.
* return value description: ERROR_CODE - ER_OK: delay was successful
*                           ERROR_CODE - ER_FAIL: delay was not successful
******************************************************************************/
ERROR_CODE eOSAL_delay(uint32_t uiDelay, uint32_t * puiMS_Delayed)
{
  uint32_t i = 0;
  ERROR_CODE eEC = ER_FAIL;

  if(bIs_OS_running == true)
  {
    Task_sleep(uiDelay);
    i = uiDelay;
  }
  else
  {
    i = iHW_delay(uiDelay);
  }

  if(i == uiDelay)
  {
    eEC = ER_OK;
  }
  else
  {
    eEC = ER_FAIL;
  }

  if(puiMS_Delayed != NULL)
  {
    *puiMS_Delayed = i;
  }else{/*do nothing*/}

  return eEC;
}

/******************************************************************************
* name: eOSAL_Mailbox_Params_Init
* description: initalizes the OSAL mailbox parameters to the system defaults
*
* param description: tOSAL_Mailbox_Handle - pointer: pointer to the OSAL mailbox handle
*                    uiWait_time       - OS wait time before returning, default OSAL_MAILBOX_DEFAULT_WAIT_FOREVER
*                    uiBuff_Size       - size of each mailbox message,  default OSAL_MAILBOX_DEFAULT_BUFF_SIZE
*                    pvBuff            - Pointer to the mailbox buffer, default NULL
*                    uiNumber_of_items - number of mailbox messages,    default OSAL_MAILBOX_DEFAULT_MAX_MESSAGES
*                    pvMailbox_Handle  - OS specific mailbox handle,    default NULL
*
* return value description: ERROR_CODE - ER_OK: mailbox parameters successfully initalized
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Params_Init (tOSAL_Mailbox_Handle * ptMbox_handle)
{
  ERROR_CODE eEC = ER_OK;

  ptMbox_handle->pvBuff            = NULL;
  ptMbox_handle->pvMailbox_Handle  = NULL;
  ptMbox_handle->uiBuff_Size       = OSAL_MAILBOX_DEFAULT_BUFF_SIZE;
  ptMbox_handle->uiNumber_of_items = OSAL_MAILBOX_DEFAULT_MAX_MESSAGES;
  ptMbox_handle->uiWait_time       = OSAL_MAILBOX_DEFAULT_WAIT_FOREVER;

  return eEC;
}

/******************************************************************************
* name: eOSAL_Mailbox_Create
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
*                    typedef struct tOSAL_Mailbox_Handle
*                    {
*                      uint16_t uiWait_time;
*                      void * pvBuff;
*                      void * pvMailbox_Handle;
*                    }tOSAL_Mailbox_Handle;
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Create  (tOSAL_Mailbox_Handle * ptMbox_handle)
{
  ERROR_CODE eEC = ER_OK;

//  Mailbox_Handle * pTIRTOS_Mailbox_Handle = (Mailbox_Handle * )malloc(sizeof(Mailbox_Handle));
  Mailbox_Handle pTIRTOS_Mailbox_Handle;

  Mailbox_Params * pTIRTOS_Mailbox_params = (Mailbox_Params * )malloc(sizeof(Mailbox_Params));
  Mailbox_Params_init(pTIRTOS_Mailbox_params);

  Error_Block * pEB = malloc(sizeof(Error_Block));

  pTIRTOS_Mailbox_Handle = Mailbox_create(ptMbox_handle->uiBuff_Size,
                                          ptMbox_handle->uiNumber_of_items,
                                          pTIRTOS_Mailbox_params,
                                          pEB);

  ptMbox_handle->pvMailbox_Handle = pTIRTOS_Mailbox_Handle;
  return eEC;
}

/******************************************************************************
* name: eOSAL_Get_Mailbox_msg
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Get_msg (tOSAL_Mailbox_Handle * ptMailbox_handle)
{
  ERROR_CODE eEC = ER_OK;
  Mailbox_Handle pTIRTOS_Mailbox_Handle = ptMailbox_handle->pvMailbox_Handle;
  void * pvBuff = ptMailbox_handle->pvBuff;
  uint32_t uiWait_Time = 0;

  if(ptMailbox_handle->uiWait_time == OSAL_MAILBOX_DEFAULT_WAIT_FOREVER)
  {
    uiWait_Time = BIOS_WAIT_FOREVER;
  }

  Mailbox_pend(pTIRTOS_Mailbox_Handle, pvBuff, uiWait_Time);

  return eEC;
}

/******************************************************************************
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eOSAL_Queue_Get_msg   (tOSAL_Queue_Handle * ptQueue_handle)
{
  ERROR_CODE eEC = ER_OK;

  return eEC;
}
#elif defined(USING_FREE_RTOS)
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define TASK_HANDLE              TaskHandle_t
#define TASK_MAX_NUM             configMAX_PRIORITIES
#define TASK_MAX_PRIORITY        1
#define TASK_MIN_PRIORITY        configMAX_PRIORITIES
#define TASK_DEFAULT_STACK_SIZE  configMINIMAL_STACK_SIZE

#define OSAL_MAILBOX_DEFAULT_WAIT_FOREVER 0xFFFFFFFF
#define OSAL_MAILBOX_DEFAULT_BUFF_SIZE    256
#define OSAL_MAILBOX_DEFAULT_MAX_MESSAGES 3

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

typedef struct tOSAL_Activity_State
{
  bool bIs_Task_Desc_Init;
  bool bIs_OS_running;
}tOSAL_Activity_State;

tOSAL_Activity_State tOSAL_AS =
{
  false,  //bool bIs_Task_Desc_Init;

  false, //bool bIs_OS_running;
};

typedef struct tOSAL_Task_Descriptor
{
  tOSAL_Task_Parameters tTask_Param;
  TASK_HANDLE tTask_Handle;
}tOSAL_Task_Descriptor;

tOSAL_Task_Descriptor tOSAL_Task_Desc[TASK_MAX_NUM];

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eOSAL_Register_Task(tOSAL_Task_Parameters *pParam, tOSAL_Task_Descriptor ** pOSAL_Reg_Desciptor);
ERROR_CODE eOSAL_Unregister_Task(tOSAL_Task_Descriptor * pOSAL_Reg_Desciptor);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eOSAL_Register_Task(tOSAL_Task_Parameters *pParam, tOSAL_Task_Descriptor ** pOSAL_Reg_Desciptor)
{
  ERROR_CODE eEC = ER_FAIL;
  int i = 0;

  if(tOSAL_AS.bIs_Task_Desc_Init == false)
  {
    for(i = 0; i < TASK_MAX_NUM; i++)
    {
      tOSAL_Task_Desc[i].tTask_Param.pTaskFcn = NULL;
      tOSAL_Task_Desc[i].tTask_Param.pName = NULL;
      tOSAL_Task_Desc[i].tTask_Param.uiStack_Size = TASK_DEFAULT_STACK_SIZE;
      tOSAL_Task_Desc[i].tTask_Param.pParameters = NULL;
      tOSAL_Task_Desc[i].tTask_Param.uiTask_Priority = TASK_MIN_PRIORITY;
      tOSAL_Task_Desc[i].tTask_Handle = NULL;
    }
    tOSAL_AS.bIs_Task_Desc_Init = true;
  }

  for(i = 0; i < TASK_MAX_NUM; i++)
  {
    if(tOSAL_Task_Desc[i].tTask_Param.pTaskFcn == NULL)
    {
      tOSAL_Task_Desc[i].tTask_Param.pTaskFcn = pParam->pTaskFcn;
      tOSAL_Task_Desc[i].tTask_Param.pName = pParam->pName;
      tOSAL_Task_Desc[i].tTask_Param.uiStack_Size = pParam->uiStack_Size;
      tOSAL_Task_Desc[i].tTask_Param.pParameters = pParam->pParameters;
      tOSAL_Task_Desc[i].tTask_Param.uiTask_Priority = pParam->uiTask_Priority;
      tOSAL_Task_Desc[i].tTask_Handle = NULL;
      *pOSAL_Reg_Desciptor = &tOSAL_Task_Desc[i];
      eEC = ER_OK;
      break;
    }
    else
    {
      eEC = ER_FULL;
    }
  }

  return eEC;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eOSAL_Task_Param_Init(tOSAL_Task_Parameters *pParam)
{
  ERROR_CODE eEC = ER_OK;

  pParam->pTaskFcn = NULL;
  pParam->pName = NULL;
  pParam->uiStack_Size = 0;
  pParam->pParameters = NULL;
  pParam->uiTask_Priority = 0;

  return eEC;
}

/******************************************************************************
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eOSAL_Task_Create(tOSAL_Task_Parameters *pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  uint32_t uiRC = 0;
  tOSAL_Task_Descriptor * pTask_Desciptor = NULL;

  eEC = eOSAL_Register_Task(pParam, &pTask_Desciptor);

//  pTask_Desciptor = &tOSAL_Task_Desc[0];

  if(eEC == ER_OK)
  {
    uiRC = xTaskCreate(
                       pTask_Desciptor->tTask_Param.pTaskFcn,        //TaskFunction_t pvTaskCode,
                       pTask_Desciptor->tTask_Param.pName,           //const char * const pcName,
                       pTask_Desciptor->tTask_Param.uiStack_Size,    //unsigned short usStackDepth,
                       pTask_Desciptor->tTask_Param.pParameters,     //void *pvParameters,
                       pTask_Desciptor->tTask_Param.uiTask_Priority, //UBaseType_t uxPriority,
                       pTask_Desciptor->tTask_Handle                 //TaskHandle_t *pvCreatedTask
                       );

    if(uiRC == pdPASS)
    {
      eEC = ER_OK;
    }
  }

  return eEC;
}

/******************************************************************************
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eOSAL_Task_Delete(tOSAL_Task_Parameters *pParam)
{
  ERROR_CODE eEC = ER_FAIL;

  return eEC;
}

/******************************************************************************
* name: eOSAL_OS_start
* description: OSAL start abstraction function. For most operating systems
*              this function will not return once called. Do not rely on it
*              to return an error code.
* param description: none
* return value description: ERROR_CODE - ER_OK: Most OS's do not return once
*                                        their OS start function was called.
*                                        Do not rely on this return value.
******************************************************************************/
ERROR_CODE eOSAL_OS_start(void)
{
  ERROR_CODE eEC = ER_OK;

  tOSAL_AS.bIs_OS_running = true;

  //Start the FREE-RTOS, will not return
  //
  vTaskStartScheduler();

  while(1){};

  return eEC;
}

ERROR_CODE eOSAL_Is_OS_Running(void)
{
  ERROR_CODE eEC = ER_FALSE;

  if(tOSAL_AS.bIs_OS_running == true)
  {
    eEC = ER_TRUE;
  }
  else
  {
    eEC = ER_FALSE;
  }
  return eEC;
}

/******************************************************************************
* name: eOSAL_delay
* description: delays for <delay parameter> number of milliseconds. Will call
*              either a hardware or OS delay depending on if the OS is running
*              or not.
* param description: uint32_t : the number of milliseconds to delay
*                    uint32_t - pointer: pointer to variable that will contain
*                    the number of milliseconds delayed.
* return value description: ERROR_CODE - ER_OK: delay was successful
*                           ERROR_CODE - ER_FAIL: delay was not successful
******************************************************************************/
ERROR_CODE eOSAL_delay(uint32_t uiDelay, uint32_t * puiMS_Delayed)
{
  uint32_t i = 0;
  ERROR_CODE eEC = ER_FAIL;

  if(tOSAL_AS.bIs_OS_running == true)
  {
    //todo: OS_delay
    i = uiDelay;
  }
  else
  {
    //todo: hardware delay
    i = uiDelay;
  }

  if(i == uiDelay)
  {
    eEC = ER_OK;
  }
  else
  {
    eEC = ER_FAIL;
  }

  if(puiMS_Delayed != NULL)
  {
    *puiMS_Delayed = i;
  }else{/*do nothing*/}

  return eEC;
}

/******************************************************************************
* name: eOSAL_Mailbox_Params_Init
* description: initalizes the OSAL mailbox parameters to the system defaults
*
* param description: tOSAL_Mailbox_Handle - pointer: pointer to the OSAL mailbox handle
*                    uiWait_time       - OS wait time before returning, default OSAL_MAILBOX_DEFAULT_WAIT_FOREVER
*                    uiBuff_Size       - size of each mailbox message,  default OSAL_MAILBOX_DEFAULT_BUFF_SIZE
*                    pvBuff            - Pointer to the mailbox buffer, default NULL
*                    uiNumber_of_items - number of mailbox messages,    default OSAL_MAILBOX_DEFAULT_MAX_MESSAGES
*                    pvMailbox_Handle  - OS specific mailbox handle,    default NULL
*
* return value description: ERROR_CODE - ER_OK: mailbox parameters successfully initalized
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Params_Init (tOSAL_Mailbox_Handle * ptMbox_handle)
{
  ERROR_CODE eEC = ER_OK;

  ptMbox_handle->pvBuff            = NULL;
  ptMbox_handle->pvMailbox_Handle  = NULL;
  ptMbox_handle->uiBuff_Size       = OSAL_MAILBOX_DEFAULT_BUFF_SIZE;
  ptMbox_handle->uiNumber_of_items = OSAL_MAILBOX_DEFAULT_MAX_MESSAGES;
  ptMbox_handle->uiWait_time       = OSAL_MAILBOX_DEFAULT_WAIT_FOREVER;

  return eEC;
}

/******************************************************************************
* name: eOSAL_Mailbox_Create
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
*                    typedef struct tOSAL_Mailbox_Handle
*                    {
*                      uint16_t uiWait_time;
*                      void * pvBuff;
*                      void * pvMailbox_Handle;
*                    }tOSAL_Mailbox_Handle;
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Create  (tOSAL_Mailbox_Handle * ptMbox_handle)
{
  ERROR_CODE eEC = ER_FAIL;

  return eEC;
}

/******************************************************************************
* name: eOSAL_Get_Mailbox_msg
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Get_msg (tOSAL_Mailbox_Handle * ptMailbox_handle)
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
ERROR_CODE eOSAL_Queue_Get_msg   (tOSAL_Queue_Handle * ptQueue_handle)
{
  ERROR_CODE eEC = ER_FAIL;

  return eEC;
}
#elif defined(SOME_OTHER_RTOS)

#endif
#endif //__OSAL_C__
