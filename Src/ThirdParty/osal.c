/******************************************************************************
*
 osal.c - operating system abstraction layer
* Copyright (c) notice
*
******************************************************************************/
#ifndef __OSAL_C__
#define __OSAL_C__

//Standard includes
  #include <stdint.h>
  #include <stdbool.h>
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>

//Project specific includes
  #include "proj_inc/project_config.h"
#if (PROJ_CONFIG_USE_UTIL_OSAL == 0)
  //OSAL not implemented
#else
  #include "proj_inc/osal_config.h"

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"

//Third party includes
  #include "ThirdParty_inc/osal.h"

#if (PROJ_CONFIG_OS == PROJ_CONFIG_OS_TIRTOS)
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
* param description: OSAL_Mailbox_Handle_t - pointer: pointer to the OSAL mailbox handle
*                    uiWait_time       - OS wait time before returning, default OSAL_MAILBOX_DEFAULT_WAIT_FOREVER
*                    uiBuff_Size       - size of each mailbox message,  default OSAL_MAILBOX_DEFAULT_BUFF_SIZE
*                    pvBuff            - Pointer to the mailbox buffer, default NULL
*                    uiNumber_of_items - number of mailbox messages,    default OSAL_MAILBOX_DEFAULT_MAX_MESSAGES
*                    pvMailbox_Handle  - OS specific mailbox handle,    default NULL
*
* return value description: ERROR_CODE - ER_OK: mailbox parameters successfully initalized
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Params_Init (OSAL_Mailbox_Handle_t * ptMbox_handle)
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
*                    typedef struct OSAL_Mailbox_Handle_t
*                    {
*                      uint16_t uiWait_time;
*                      void * pvBuff;
*                      void * pvMailbox_Handle;
*                    }OSAL_Mailbox_Handle_t;
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Create  (OSAL_Mailbox_Handle_t * ptMbox_handle)
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
ERROR_CODE eOSAL_Mailbox_Get_msg (OSAL_Mailbox_Handle_t * ptMailbox_handle)
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
ERROR_CODE eOSAL_Queue_Get_msg   (OSAL_Queue_Handle_t * ptQueue_handle)
{
  ERROR_CODE eEC = ER_OK;

  return eEC;
}
#elif (PROJ_CONFIG_OS == PROJ_CONFIG_OS_FREERTOS)
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define TASK_HANDLE              TaskHandle_t
#define TASK_MAX_NUM             configMAX_PRIORITIES
#define TASK_MAX_PRIORITY        1
#define TASK_MIN_PRIORITY        configMAX_PRIORITIES
#define TASK_DEFAULT_STACK_SIZE  configMINIMAL_STACK_SIZE

#define QUEUE_HANDLE             QueueHandle_t
#define QUEUE_MAX_NUM            configQUEUE_REGISTRY_SIZE
#define QUEUE_WAIT_FOREVER       portMAX_DELAY

#define SEMAPHORE_HANDLE         SemaphoreHandle_t
#define SEMAPHORE_MAX_NUM        configQUEUE_REGISTRY_SIZE
#define SEMAPHORE_WAIT_FOREVER   portMAX_DELAY

#define DATA_MUTEX_MAX_NUM

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

typedef struct OSAL_Activity_State_t
{
  bool bIs_Task_Desc_Init;
  bool bIs_Queue_Desc_Init;
  bool bIs_Semaphore_Desc_Init;
  bool bIs_Data_Mutex_Desc_Init;
  bool bIs_OS_running;
}OSAL_Activity_State_t;

OSAL_Activity_State_t OSAL_AS_t =
{
  .bIs_Task_Desc_Init       = false,
  .bIs_Queue_Desc_Init      = false,
  .bIs_Semaphore_Desc_Init  = false,
  .bIs_OS_running           = false,
  .bIs_Data_Mutex_Desc_Init = false,
};

typedef struct OSAL_Task_Descriptor_t
{
  OSAL_Task_Parameters_t tTask_Param;
  TASK_HANDLE tTask_Handle;
}OSAL_Task_Descriptor_t;

OSAL_Task_Descriptor_t tOSAL_Task_Desc[TASK_MAX_NUM];

typedef struct tOSAL_Queue_Descriptor
{
  OSAL_Queue_Parameters_t  tQueue_Param;
  OSAL_Queue_Handle_t      tQueue_Handle;
}tOSAL_Queue_Descriptor;

tOSAL_Queue_Descriptor tOSAL_Queue_Desc[QUEUE_MAX_NUM];

typedef struct OSAL_Semaphore_Descriptor
{
  OSAL_Semaphore_Handle_t   Sem_Handle_t;
  void *                    pMutex_Data;
  uint32_t                  ui32Mutex_Data_Size;
}OSAL_Semaphore_Descriptor_t, * pOSAL_Semaphore_Descriptor;

OSAL_Semaphore_Descriptor_t OSAL_Sem_Desc_t[SEMAPHORE_MAX_NUM];

typedef struct OSAL_Data_Mutex_Descriptor
{
  OSAL_Data_Mutex_Handle_t   Data_Mutex_Handle_t;
  void *                     pMutex_Data;
  uint32_t                   ui32Mutex_Data_Size;
}OSAL_Data_Mutex_Descriptor_t, * pOSAL_Data_Mutex_Descriptor;

OSAL_Data_Mutex_Descriptor_t OSAL_Data_Mutex_desc_t[5];
/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static ERROR_CODE eOSAL_Register_Task(OSAL_Task_Parameters_t * pParam, OSAL_Task_Descriptor_t ** pOSAL_Reg_Desciptor);
static ERROR_CODE eOSAL_Unregister_Task(OSAL_Task_Descriptor_t * pOSAL_Reg_Desciptor);

static ERROR_CODE eOSAL_Register_Queue(OSAL_Queue_Parameters_t * pParam, tOSAL_Queue_Descriptor ** pQueue_Desc);
static ERROR_CODE eOSAL_Unregister_Queue(tOSAL_Queue_Descriptor * pQueue_Descriptor);

static ERROR_CODE eOSAL_Register_Semaphore(pOSAL_Semaphore_Descriptor * pSem_Desc);
static ERROR_CODE eOSAL_Unregister_Semaphore(pOSAL_Semaphore_Handle pSem_Handle);

static ERROR_CODE eOSAL_Register_Data_Mutex(pOSAL_Data_Mutex_Descriptor * pDesc);
static ERROR_CODE eOSAL_Unregister_Data_Mutex(pOSAL_Data_Mutex_Handle pHandle);
static ERROR_CODE eOSAL_Get_Mutex_Desc(pOSAL_Data_Mutex_Handle pHandle, OSAL_Data_Mutex_Descriptor_t ** pDesc);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: eOSAL_Register_Task
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description:
*   (ERROR_CODE):
*     = ER_OK: Task registered
*     = ER_FAIL: Failed to register the task
*     = ER_FULL: Cannot create any more tasks
******************************************************************************/
static ERROR_CODE eOSAL_Register_Task(OSAL_Task_Parameters_t *pParam, OSAL_Task_Descriptor_t ** pOSAL_Reg_Desciptor)
{
  ERROR_CODE eEC = ER_FAIL;
  int i = 0;
  //todo test
  if(OSAL_AS_t.bIs_Task_Desc_Init == false)
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
    OSAL_AS_t.bIs_Task_Desc_Init = true;
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
* name: eOSAL_Unregister_Task
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
static ERROR_CODE eOSAL_Unregister_Task(OSAL_Task_Descriptor_t * pOSAL_Reg_Desciptor)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("",false);
  //todo finish & test
  if(pOSAL_Reg_Desciptor == NULL)
  {
    eEC = ER_PARAM;
  }
  return eEC;
}

/******************************************************************************
* name: eOSAL_Register_Queue
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
static ERROR_CODE eOSAL_Register_Queue(OSAL_Queue_Parameters_t * pParam, tOSAL_Queue_Descriptor ** pQueue_Desc)
{
  ERROR_CODE eEC = ER_FAIL;
  int i = 0;
  //todo test
  if(OSAL_AS_t.bIs_Queue_Desc_Init == false)
  {
    for(i = 0; i < QUEUE_MAX_NUM; i++)
    {
      tOSAL_Queue_Desc[i].tQueue_Handle.pHandle = NULL;
      tOSAL_Queue_Desc[i].tQueue_Param.uiNum_Of_Queue_Elements = 0;
      tOSAL_Queue_Desc[i].tQueue_Param.uiSize_Of_Queue_Element = 0;
      tOSAL_Queue_Desc[i].tQueue_Param.pMsgBuff = NULL;
      tOSAL_Queue_Desc[i].tQueue_Param.iTimeout = 0;
    }

    OSAL_AS_t.bIs_Queue_Desc_Init = true;
  }

  for(i = 0; i < QUEUE_MAX_NUM; i++)
  {
    if(tOSAL_Queue_Desc[i].tQueue_Handle.pHandle == NULL)
    {
      tOSAL_Queue_Desc[i].tQueue_Param.uiNum_Of_Queue_Elements = pParam->uiNum_Of_Queue_Elements;
      tOSAL_Queue_Desc[i].tQueue_Param.uiSize_Of_Queue_Element = pParam->uiSize_Of_Queue_Element;
      tOSAL_Queue_Desc[i].tQueue_Param.pMsgBuff = pParam->pMsgBuff;
      if(pParam->iTimeout == OSAL_QUEUE_TIMEOUT_WAITFOREVER)
      {
        tOSAL_Queue_Desc[i].tQueue_Param.iTimeout = QUEUE_WAIT_FOREVER;
      }
      else
      {
        tOSAL_Queue_Desc[i].tQueue_Param.iTimeout = pParam->iTimeout;
      }
      tOSAL_Queue_Desc[i].tQueue_Handle.uiHandle_Index = i;
      *pQueue_Desc = &tOSAL_Queue_Desc[i];

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
* name: eOSAL_Unregister_Queue
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
static ERROR_CODE eOSAL_Unregister_Queue(tOSAL_Queue_Descriptor * pQueue_Descriptor)
{
  ERROR_CODE eEC = ER_FAIL;
  //todo finish & test
  vDEBUG_ASSERT("",false);
  return eEC;
}

static ERROR_CODE eOSAL_Register_Semaphore(pOSAL_Semaphore_Descriptor * pSem_Desc)
{
  ERROR_CODE eEC = ER_FAIL;

  int i = 0;
  //todo finish & test
  vDEBUG_ASSERT("",false);
  if(OSAL_AS_t.bIs_Semaphore_Desc_Init == false)
  {
    for(i = 0; i < SEMAPHORE_MAX_NUM; i++)
    {
      memset(&OSAL_Sem_Desc_t[i], 0x00, sizeof(OSAL_Semaphore_Descriptor_t));

      OSAL_Sem_Desc_t[i].Sem_Handle_t.uiHandle_Index = i;
    }

    OSAL_AS_t.bIs_Semaphore_Desc_Init = true;
  }

  for(i = 0; i < SEMAPHORE_MAX_NUM; i++)
  {
    if(OSAL_Sem_Desc_t[i].Sem_Handle_t.pHandle == NULL)
    {
      OSAL_Sem_Desc_t[i].Sem_Handle_t.uiHandle_Index = i;
      *pSem_Desc = &OSAL_Sem_Desc_t[i];
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

static ERROR_CODE eOSAL_Unregister_Semaphore(pOSAL_Semaphore_Handle pSem_Handle)
{
  ERROR_CODE eEC = ER_FAIL;
  int i = 0;
  //todo finish
  vDEBUG_ASSERT("",false);
  for(i = 0; i < SEMAPHORE_MAX_NUM; i++)
  {
//    if(OSAL_Sem_Desc_t[i].Sem_Handle_t == pSem_Handle)
//    {
//      OSAL_Sem_Desc_t[i].Sem_Handle_t.pHandle = NULL;
//      OSAL_Sem_Desc_t[i].Sem_Handle_t.uiHandle_Index = i;
//
//      eEC = ER_OK;
//      break;
//    }
//    else
//    {
//      eEC = ER_NONE;
//    }
  }

  return eEC;
}

static ERROR_CODE eOSAL_Register_Data_Mutex(pOSAL_Data_Mutex_Descriptor * pDesc)
{
  ERROR_CODE eEC = ER_FAIL;

  int i = 0;
  //todo finish & test
  if(OSAL_AS_t.bIs_Data_Mutex_Desc_Init == false)
  {
    for(i = 0; i < 5; i++)
    {
      OSAL_Data_Mutex_desc_t[i].Data_Mutex_Handle_t.pHandle = NULL;
      OSAL_Data_Mutex_desc_t[i].Data_Mutex_Handle_t.uiHandle_Index = i;
    }

    OSAL_AS_t.bIs_Data_Mutex_Desc_Init = true;
  }

  for(i = 0; i < 5; i++)
  {
    if(OSAL_Data_Mutex_desc_t[i].Data_Mutex_Handle_t.pHandle == NULL)
    {
      OSAL_Data_Mutex_desc_t[i].Data_Mutex_Handle_t.uiHandle_Index = i;
      *pDesc = &OSAL_Data_Mutex_desc_t[i];
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

static ERROR_CODE eOSAL_Unregister_Data_Mutex(pOSAL_Data_Mutex_Handle pHandle)
{
  ERROR_CODE eEC = ER_FAIL;
  //todo finish & test
  vDEBUG_ASSERT("",false);
  return eEC;
}

static ERROR_CODE eOSAL_Get_Mutex_Desc(pOSAL_Data_Mutex_Handle pHandle, OSAL_Data_Mutex_Descriptor_t ** pDesc)
{
  ERROR_CODE eEC = ER_FAIL;
  pOSAL_Data_Mutex_Descriptor pDM_Desc;

  if(OSAL_AS_t.bIs_Data_Mutex_Desc_Init == false)
  {
    eEC = ER_NOT_READY;
  }
  else
  {
    pDM_Desc = &OSAL_Data_Mutex_desc_t[pHandle->uiHandle_Index];
    if(pDM_Desc->Data_Mutex_Handle_t.pHandle != pHandle->pHandle)
    {
      *pDesc = NULL;
      eEC = ER_INVALID;
    }
    else
    {
      *pDesc = pDM_Desc;
      eEC = ER_OK;
    }
  }
//  vDEBUG_ASSERT("",false);
  return eEC;
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
  extern void HardFault_Handler(void);
  ( void ) pcTaskName;
  ( void ) pxTask;

  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected. */
  taskDISABLE_INTERRUPTS();
  HardFault_Handler();
  for( ;; );
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: eOSAL_Task_Param_Init
******************************************************************************/
ERROR_CODE eOSAL_Task_Param_Init(pOSAL_Task_Parameters pParam)
{
  ERROR_CODE eEC = ER_OK;
  void * pReturn;

  if(pParam == NULL)
  {
    eEC = ER_PARAM;
  }
  else
  {
    pReturn = memset(pParam, 0x00, sizeof(OSAL_Task_Parameters_t));
    if(pReturn == pParam)
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
* name: eOSAL_Task_Create
******************************************************************************/
ERROR_CODE eOSAL_Task_Create(pOSAL_Task_Parameters pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  uint32_t uiRC = 0;
  OSAL_Task_Descriptor_t * pTask_Desciptor = NULL;

  eEC = eOSAL_Register_Task(pParam, &pTask_Desciptor);

  if(eEC == ER_OK)
  {
    uiRC = xTaskCreate(
                       pTask_Desciptor->tTask_Param.pTaskFcn,        //TaskFunction_t pvTaskCode,
                       pTask_Desciptor->tTask_Param.pName,           //const char * const pcName,
                       pTask_Desciptor->tTask_Param.uiStack_Size,    //unsigned short usStackDepth,
                       pTask_Desciptor->tTask_Param.pParameters,     //void *pvParameters,
                       pTask_Desciptor->tTask_Param.uiTask_Priority, //UBaseType_t uxPriority,
                       &pTask_Desciptor->tTask_Handle                 //TaskHandle_t *pvCreatedTask
                       );

    if(uiRC == pdPASS)
    {
      eEC = ER_OK;
    }
    else
    {
      //todo: unregister task
      //todo need to implement method to return task handle to caller
      eEC = ER_FAIL;
    }
  }

  return eEC;
}

/******************************************************************************
* name: eOSAL_Task_Delete
******************************************************************************/
ERROR_CODE eOSAL_Task_Delete(pOSAL_Task_Handle pHandle)
{
  ERROR_CODE eEC = ER_FAIL;
  //todo finish & test
  //todo need to implement task handle into task create
  vDEBUG_ASSERT("",false);
  eOSAL_Unregister_Task(NULL);
  return eEC;
}

/******************************************************************************
* name: eOSAL_OS_start
******************************************************************************/
ERROR_CODE eOSAL_OS_start(void)
{
  ERROR_CODE eEC = ER_OK;

  OSAL_AS_t.bIs_OS_running = true;

  //Start the FREE-RTOS, will not return
  //
  vTaskStartScheduler();

  while(1){};

  return eEC;
}

/******************************************************************************
* name: eOSAL_Is_OS_Running
******************************************************************************/
ERROR_CODE eOSAL_Is_OS_Running(void)
{
  ERROR_CODE eEC = ER_FALSE;

  if(OSAL_AS_t.bIs_OS_running == true)
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
******************************************************************************/
ERROR_CODE eOSAL_delay(uint32_t uiDelay, uint32_t * puiMS_Delayed)
{
  uint32_t i = 0;
  ERROR_CODE eEC = ER_FAIL;

  if(OSAL_AS_t.bIs_OS_running == true)
  {
    vTaskDelay(uiDelay);
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
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Params_Init (OSAL_Mailbox_Handle_t * ptMbox_handle)
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
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Create  (OSAL_Mailbox_Handle_t * ptMbox_handle)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("eOSAL_Mailbox_Create not defined", eEC == ER_OK);
  return eEC;
}

/******************************************************************************
* name: eOSAL_Get_Mailbox_msg
******************************************************************************/
ERROR_CODE eOSAL_Mailbox_Get_msg (OSAL_Mailbox_Handle_t * ptMailbox_handle)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("eOSAL_Mailbox_Get_msg not defined", eEC == ER_OK);
  return eEC;
}

/******************************************************************************
* name: eOSAL_Queue_Params_Init
******************************************************************************/
ERROR_CODE eOSAL_Queue_Params_Init(OSAL_Queue_Parameters_t * ptQueue_param)
{
  ERROR_CODE eEC = ER_OK;

  ptQueue_param->uiNum_Of_Queue_Elements = 0;
  ptQueue_param->uiSize_Of_Queue_Element = 0;

  return eEC;
}

/******************************************************************************
* name: eOSAL_Queue_Create
******************************************************************************/
ERROR_CODE eOSAL_Queue_Create(OSAL_Queue_Parameters_t * ptQueue_param, OSAL_Queue_Handle_t ** pQueue_Handle)
{
  ERROR_CODE eEC = ER_FAIL;
  tOSAL_Queue_Descriptor * pQueue_Desc;

  eEC = eOSAL_Register_Queue(ptQueue_param, &pQueue_Desc);

  if(eEC == ER_OK)
  {
    pQueue_Desc->tQueue_Handle.pHandle = xQueueCreate(
                                                      ptQueue_param->uiNum_Of_Queue_Elements,
                                                      ptQueue_param->uiSize_Of_Queue_Element
                                                      );

    if(pQueue_Desc->tQueue_Handle.pHandle != NULL)
    {
      *pQueue_Handle = &pQueue_Desc->tQueue_Handle;
      eEC = ER_OK;
    }
  }

  return eEC;
}

/******************************************************************************
* name: eOSAL_Queue_Delete
******************************************************************************/
ERROR_CODE eOSAL_Queue_Delete(OSAL_Queue_Handle_t * ptQueue_handle)
{
  ERROR_CODE eEC = ER_FAIL;
  //todo finish & test
    vDEBUG_ASSERT("",false);
  eOSAL_Unregister_Queue(NULL);
  return eEC;
}

/******************************************************************************
* name: eOSAL_Queue_Get_msg
******************************************************************************/
ERROR_CODE eOSAL_Queue_Get_msg(OSAL_Queue_Handle_t * ptQueue_handle, void * pMsg)
{
  ERROR_CODE eEC = ER_FAIL;
  uint32_t uiRC = 0;

  uiRC = xQueueReceive(
                       ptQueue_handle->pHandle,
                       pMsg,
                       tOSAL_Queue_Desc[ptQueue_handle->uiHandle_Index].tQueue_Param.iTimeout
                       );
  if(uiRC == pdPASS)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* name: eOSAL_Queue_Post_msg
******************************************************************************/
ERROR_CODE eOSAL_Queue_Post_msg(OSAL_Queue_Handle_t * ptQueue_handle, void * pMsg)
{
  ERROR_CODE eEC = ER_FAIL;
  uint32_t uiRC = 0;

  uiRC = xQueueSendToFront(
                           ptQueue_handle->pHandle,
                           pMsg,
                           tOSAL_Queue_Desc[ptQueue_handle->uiHandle_Index].tQueue_Param.iTimeout
                           );

  if(uiRC == pdPASS)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* name: eOSAL_Semaphore_Params_Init
******************************************************************************/
ERROR_CODE eOSAL_Semaphore_Params_Init(pOSAL_Semaphore_Parameters pParameters)
{
  return ER_OK;
}

/******************************************************************************
* name: eOSAL_Semaphore_Create
******************************************************************************/
ERROR_CODE eOSAL_Semaphore_Create(pOSAL_Semaphore_Parameters pParameters, pOSAL_Semaphore_Handle * pSemaphore_Handle)
{
  ERROR_CODE eEC = ER_FAIL;
  pOSAL_Semaphore_Descriptor pSem_Desc;

  eEC = eOSAL_Register_Semaphore(&pSem_Desc);

  pSem_Desc->Sem_Handle_t.pHandle = xSemaphoreCreateBinary();

  return eEC;
}

ERROR_CODE eOSAL_Semaphore_Delete(pOSAL_Semaphore_Handle pSemaphore_Handle)
{
  ERROR_CODE  eEC = ER_FAIL;
  //todo finish & test
  vDEBUG_ASSERT("",false);
  eOSAL_Unregister_Semaphore(NULL);
  return eEC;
}


/******************************************************************************
* name: eOSAL_Semaphore_Wait
******************************************************************************/
ERROR_CODE eOSAL_Semaphore_Wait(pOSAL_Semaphore_Handle pSemaphore_Handle)
{
  ERROR_CODE eEC = ER_FAIL;
  //todo finish & test
  vDEBUG_ASSERT("",false);
  return eEC;
}

/******************************************************************************
* name: eOSAL_Semaphore_Wait_Timeout
******************************************************************************/
ERROR_CODE eOSAL_Semaphore_Wait_Timeout(pOSAL_Semaphore_Handle pSemaphore_Handle, uint32_t uiTimeout)
{
  ERROR_CODE eEC = ER_FAIL;
  //todo finish & test
  vDEBUG_ASSERT("",false);
  return eEC;
}

/******************************************************************************
* name: eOSAL_Semaphore_Post
******************************************************************************/
ERROR_CODE eOSAL_Semaphore_Post(pOSAL_Semaphore_Handle pSemaphore_Handle)
{
  ERROR_CODE eEC = ER_FAIL;
  //todo finish & test
  vDEBUG_ASSERT("",false);
  return eEC;
}

/******************************************************************************
* name: eOSAL_Mutex_Create
******************************************************************************/
ERROR_CODE eOSAL_Mutex_Create(pOSAL_Mutex_Parameters pParameters, pOSAL_Mutex_Handle *pMutex_Handle)
{
  ERROR_CODE eEC = ER_FAIL;
  //todo finish & test
  vDEBUG_ASSERT("",false);
  xSemaphoreCreateMutex();

  return eEC;
}

/******************************************************************************
* name: eOSAL_Mutex_Create
******************************************************************************/
ERROR_CODE eOSAL_Mutex_Get(pOSAL_Mutex_Handle pHandle)
{
  ERROR_CODE eEC = ER_FAIL;
  xSemaphoreTake(pHandle->pHandle, 1);
  return eEC;
}

/******************************************************************************
* name: eOSAL_Mutex_Return
******************************************************************************/
ERROR_CODE eOSAL_Mutex_Return(pOSAL_Mutex_Handle pHandle)
{
  ERROR_CODE eEC = ER_FAIL;
  xSemaphoreGive(pHandle->pHandle);
  return eEC;
}

/******************************************************************************
* name: eOSAL_Data_Mutex_Create
******************************************************************************/
ERROR_CODE eOSAL_Data_Mutex_Create(pOSAL_Data_Mutex_Parameters pParameters, OSAL_Data_Mutex_Handle_t ** pHandle)
{
  ERROR_CODE eEC = ER_FAIL;
  pOSAL_Data_Mutex_Descriptor pDesc;

  eEC = eOSAL_Register_Data_Mutex(&pDesc);

  if(eEC == ER_OK)
  {
    pDesc->Data_Mutex_Handle_t.pHandle = xSemaphoreCreateMutex();

    if(pDesc->Data_Mutex_Handle_t.pHandle != NULL)
    {
      pDesc->pMutex_Data = calloc(pParameters->uiObject_Size, sizeof(uint8_t));
      if(pDesc->pMutex_Data != NULL)
      {
        pDesc->ui32Mutex_Data_Size = pParameters->uiObject_Size;
        *pHandle = &pDesc->Data_Mutex_Handle_t;
        eEC = ER_OK;
      }
      else
      {
        eEC = ER_NOMEM;
      }
    }
    else
    {
      *pHandle = NULL;
      eEC = ER_CREATE;
    }
  }

  return eEC;
}

/******************************************************************************
* name: eOSAL_Data_Mutex_Delete
******************************************************************************/
ERROR_CODE eOSAL_Data_Mutex_Delete(pOSAL_Data_Mutex_Handle pHandle)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", false);
  eOSAL_Unregister_Data_Mutex(NULL);
  return eEC;
}

/******************************************************************************
* name: eOSAL_Data_Mutex_Get
******************************************************************************/
ERROR_CODE eOSAL_Data_Mutex_Get(pOSAL_Data_Mutex_Handle pHandle, void ** pData)
{
  ERROR_CODE eEC = ER_FAIL;
  bool bRC = false;
  uint32_t ui32Timeout = 0;
  pOSAL_Data_Mutex_Descriptor pDesc;

  //determine if the mutex was created
  eEC = eOSAL_Get_Mutex_Desc(pHandle, &pDesc);
  if(eEC == ER_OK)
  {
    bRC = xSemaphoreTake(pHandle->pHandle, 0);//, QUEUE_WAIT_FOREVER); //todo: add wait time to mutex creation
    if(bRC == true)
    {
      *pData = pDesc->pMutex_Data;
    }
    else
    {
      *pData = NULL;
    }
  }

  //todo: test when max number of mutex's tanken

  return eEC;
}

/******************************************************************************
* name: eOSAL_Data_Mutex_Return
******************************************************************************/
ERROR_CODE eOSAL_Data_Mutex_Return(pOSAL_Data_Mutex_Handle pHandle, void ** pData)
{
  ERROR_CODE eEC = ER_FAIL;
  bool bRC = false;
  uint32_t ui32Timeout = 0;
  pOSAL_Data_Mutex_Descriptor pDesc;

  //determine if the mutex was created
  eEC = eOSAL_Get_Mutex_Desc(pHandle, &pDesc);
  if(eEC == ER_OK)
  {
    bRC = xSemaphoreGive(pHandle->pHandle);
    if(bRC == true)
    {
      *pData = NULL;
    }
  }

  return eEC;
}

#elif defined(SOME_OTHER_RTOS)

#endif

#endif //PROJ_CONFIG_USE_UTIL_OSAL
#endif //__OSAL_C__
