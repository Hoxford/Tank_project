/******************************************************************************
*
* osal.h - Operating System abstraction layer include file
* Copyright (c) notice
*
******************************************************************************/
#ifndef __OSAL_H__
#define __OSAL_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#include "proj_inc/project_config.h"
#if (PROJ_CONFIG_USE_UTIL_OSAL == 0)
#define eOSAL_Task_Param_Init     (p)
#define eOSAL_Task_Create         (p)
#define eOSAL_Task_Delete         (p)
#define eOSAL_OS_start            ()
#define eOSAL_Is_OS_Running       ()
#define eOSAL_delay               (x, p)
#define eOSAL_Mailbox_Params_Init (p)
#define eOSAL_Mailbox_Create      (p)
#define eOSAL_Mailbox_Get_msg     (p)
#define eOSAL_Mailbox_Post_msg    (p)
#define eOSAL_Queue_Params_Init   (p)
#define eOSAL_Queue_Create        (p,pp)
#define eOSAL_Queue_Get_msg       (p,pp)
#define eOSAL_Queue_Post_msg      (p,pp)
#else //(PROJ_CONFIG_USE_UTIL_OSAL > 0)

#if (PROJ_CONFIG_OS == PROJ_CONFIG_OS_TIRTOS)
//TI RTOS XDCtools header files
  #include <xdc/runtime/Error.h>
  #include <xdc/std.h>
  #include <xdc/runtime/System.h>
  #include <xdc/runtime/Types.h>
//#include <xdc/runtime/knl/Semaphore.h>

//TI RTOS sysbios header files
  #include <ti/sysbios/BIOS.h>
//fatfs sysbios header files
  #include <ti/sysbios/fatfs/diskio.h>
//gate sysbios header files
  #include <ti/sysbios/gates/GateMutex.h>
  #include <ti/sysbios/gates/GateMutexPri.h>
//hal sysbios header files
  #include <ti/sysbios/hal/Timer.h>
  #include <ti/sysbios/family/arm/m3/Hwi.h>

//knl sysbios header files
  #include <ti/sysbios/knl/Task.h>
  #include <ti/sysbios/knl/Queue.h>
  #include <ti/sysbios/knl/Mailbox.h>
  #include <ti/sysbios/knl/Clock.h>
  #include <ti/sysbios/knl/Semaphore.h>

//driver sysbios header files
  #include <ti/drivers/SDSPI.h>
  #include <ti/drivers/sdspi/SDSPITiva.h>
  #include <ti/drivers/USBMSCHFatFs.h>
  #include <ti/drivers/GPIO.h>
#elif (PROJ_CONFIG_OS == PROJ_CONFIG_OS_FREERTOS)
  #include "FreeRTOS.h"
  #include "task.h"
#endif
/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define OSAL_SEM_WAIT_TIMEOUT_WAITFOREVER  BIOS_WAIT_FOREVER
#define OSAL_SEM_WAIT_TIMEOUT_NO_WAIT      BIOS_NO_WAIT
#define OSAL_SEM_WAIT_TIMEOUT_1TICK        1
#define OSAL_SEM_WAIT_TIMEOUT_1SECOND      100000

#define OSAL_MBOX_WAIT_TIMEOUT_WAITFOREVER  BIOS_WAIT_FOREVER
#define OSAL_MBOX_WAIT_TIMEOUT_NO_WAIT      BIOS_NO_WAIT

#define OSAL_QUEUE_TIMEOUT_WAITFOREVER  -1
#define OSAL_QUEUE_TIMEOUT_NO_WAIT      0

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
typedef struct OSAL_Mailbox_Handle_t
{
  uint32_t uiWait_time;         //OS wait time before returning
  uint16_t uiBuff_Size;         //size of each mailbox message
  void *   pvBuff;              //Pointer to the mailbox buffer, generated by the OSAL
  uint16_t uiNumber_of_items;   //number of mailbox messages
  void *   pvMailbox_Handle;    //OS mailbox handle, generated by the OSAL
}OSAL_Mailbox_Handle_t, * pOSAL_Mailbox_Handle;

typedef struct OSAL_Queue_Parameters_t
{
  uint32_t uiNum_Of_Queue_Elements;
  uint32_t uiSize_Of_Queue_Element;
  void * pMsgBuff;
  int32_t iTimeout;
}OSAL_Queue_Parameters_t, * pOSAL_Queue_Parameters;

typedef struct OSAL_Queue_Handle_t
{
  void * pHandle;
  uint32_t uiHandle_Index;
}OSAL_Queue_Handle_t, * pOSAL_Queue_Handle;

typedef struct OSAL_Task_Parameters_t
{
    void (* pTaskFcn)(void * param);
    char * pName;
    uint32_t uiStack_Size;
    void * pParameters;
    uint32_t uiTask_Priority;
}OSAL_Task_Parameters_t, * pOSAL_Task_Parameters;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
ERROR_CODE eOSAL_Task_Param_Init     (OSAL_Task_Parameters_t *pParam);
ERROR_CODE eOSAL_Task_Create         (OSAL_Task_Parameters_t *pParam);
ERROR_CODE eOSAL_Task_Delete         (OSAL_Task_Parameters_t *pParam);
ERROR_CODE eOSAL_OS_start            (void);
ERROR_CODE eOSAL_Is_OS_Running       (void);
ERROR_CODE eOSAL_delay               (uint32_t uiDelay, uint32_t * puiMS_Delayed);
ERROR_CODE eOSAL_Mailbox_Params_Init (OSAL_Mailbox_Handle_t * ptMbox_handle);
ERROR_CODE eOSAL_Mailbox_Create      (OSAL_Mailbox_Handle_t * ptMbox_handle);
ERROR_CODE eOSAL_Mailbox_Get_msg     (OSAL_Mailbox_Handle_t * ptMailbox_handle);
ERROR_CODE eOSAL_Mailbox_Post_msg    (OSAL_Mailbox_Handle_t * ptMailbox_handle);
ERROR_CODE eOSAL_Queue_Params_Init   (OSAL_Queue_Parameters_t * ptQueue_param);
ERROR_CODE eOSAL_Queue_Create        (OSAL_Queue_Parameters_t * ptQueue_param, OSAL_Queue_Handle_t ** pQueue_Handle);
ERROR_CODE eOSAL_Queue_Get_msg       (OSAL_Queue_Handle_t * ptQueue_handle, void * pMsg);
ERROR_CODE eOSAL_Queue_Post_msg      (OSAL_Queue_Handle_t * ptQueue_handle, void * pMsg);

#endif //PROJ_CONFIG_USE_UTIL_OSAL
#endif //__FILE_NAME_H__
