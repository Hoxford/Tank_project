//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx_hal.h"

#include "proj_inc/main_proj.h"
#include "utils_inc/error_codes.h"
#include "utils_inc/osal.h"
#include "utils_inc/proj_debug.h"
#include "utils_inc/clock.h"
#include "board.h"

#include "drivers_inc/camera.h"
#include "drivers_inc/nvram.h"
#include "drivers_inc/wifi.h"
#include "drivers_inc/usb.h"
#include "app_inc/commander.h"

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

static uint32_t uiCurrent_Tick = 0;

tOSAL_Task_Parameters tOSAL_Task_Param;
tWifi_Request         tWifi_Req;
tCommand_Request      tCmnd_Req;
tNvram_Request        tNVReq;
tUsb_Request          tUsb_Req;
tCamera_Request       tCamera_Req;

#ifndef __WIFI_H__
  #define tWifi_Request void *
  #define vWifi_Driver_Task(p)
  #define eWifi_Request_Param_Init(p)
  #define eWifi_Request(p)
#endif

extern void HardFault_Handler();
extern void timer_tick();
extern void HAL_IncTick(void);

void vApplicationTickHook( void )
{

  eClock_process_init();
//  /* Fill the FPU registers with 0. */
//  vRegTestClearFlopRegistersToParameterValue( 0UL );
//
//  /* Trigger a timer 2 interrupt, which will fill the registers with a
//  different value and itself trigger a timer 3 interrupt.  Note that the
//  timers are not actually used.  The timer 2 and 3 interrupt vectors are
//  just used for convenience. */
//  NVIC_SetPendingIRQ( TIM2_IRQn );
//
//  /* Ensure that, after returning from the nested interrupts, all the FPU
//  registers contain the value to which they were set by the tick hook
//  function. */
//  configASSERT( ulRegTestCheckFlopRegistersContainParameterValue( 0UL ) );

  return;
}

void vApplicationMallocFailedHook( void )
{
  /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created.  It is also called by various parts of the
  demo application.  If heap_1.c or heap_2.c are used, then the size of the
  heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
  FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
  to query the size of free heap space that remains (although it does not
  provide information on how the remaining heap might be fragmented). */
  taskDISABLE_INTERRUPTS();
  HardFault_Handler();
  for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
  uiCurrent_Tick = HAL_GetTick();

  /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
  to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
  task.  It is essential that code added to this hook function never attempts
  to block in any way (for example, call xQueueReceive() with a block time
  specified, or call vTaskDelay()).  If the application makes use of the
  vTaskDelete() API function (as this demo application does) then it is also
  important that vApplicationIdleHook() is permitted to return to its calling
  function, because it is the responsibility of the idle task to clean up
  memory allocated by the kernel to any task that has since been deleted. */

  return;
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
  ( void ) pcTaskName;
  ( void ) pxTask;

  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected. */
  taskDISABLE_INTERRUPTS();
  HardFault_Handler();
  for( ;; );
}

void main_proj(void)
{
  ERROR_CODE eEC = ER_FAIL;

  vDEBUG_init();
  eEC = ER_OK;

#if (PROJ_CONFIG_USE_DRVR_PERSISTENT_SETTINGS >= 1)

  //Initialize NV ram
  eNvram_Request_Param_Init(&tNVReq);
  tNVReq.eRequestID = NVRAM_REQUEST_INIT;
  eEC = eNvram_Request(&tNVReq);

#endif //#if (PROJ_CONFIG_USE_DRVR_PERSISTENT_SETTINGS >= 1)

#if (PROJ_CONFIG_USE_APP_COMMANDER >= 1)
  //create command task
  eCommand_Param_Init(&tCmnd_Req);
  eOSAL_Task_Param_Init(&tOSAL_Task_Param);
  tCmnd_Req.eRequestID = CMND_REQUEST_TASK_PARAMETERS;
  tCmnd_Req.pCommander_Task_Param = &tOSAL_Task_Param;
  eEC = eCommand_Request(&tCmnd_Req);

  if(eEC == ER_OK)
  {
    eEC = eOSAL_Task_Create(&tOSAL_Task_Param);
  }
#endif //#if (PROJ_CONFIG_USE_APP_COMMANDER >= 1)

#if (PROJ_CONFIG_USE_DRVR_WIFI >= 1)

  //create wifi task
  eWifi_Request_Param_Init(&tWifi_Req);
  eOSAL_Task_Param_Init(&tOSAL_Task_Param);
  tWifi_Req.eRequestID = WIFI_REQUEST_TASK_PARAMETERS;
  tWifi_Req.pWifi_Task_Param = &tOSAL_Task_Param;
  eEC = eWifi_Request(&tWifi_Req);

  if(eEC == ER_OK)
  {
    eEC = eOSAL_Task_Create(&tOSAL_Task_Param);
  }

#endif  //#if (PROJ_CONFIG_USE_DRVR_WIFI >= 1)

#if (PROJ_CONFIG_USE_DRVR_USB >= 1)

  //create USB task
  eUSB_Request_Param_Init(&tUsb_Req);
  eOSAL_Task_Param_Init(&tOSAL_Task_Param);
  tUsb_Req.eRequestID = USB_REQUEST_TASK_PARAM;
  tUsb_Req.pUsb_Task_Param = &tOSAL_Task_Param;
  eEC = eUSB_Request(&tUsb_Req);

  if(eEC == ER_OK)
  {
    eEC = eOSAL_Task_Create(&tOSAL_Task_Param);
  }

#endif //#if (PROJ_CONFIG_USE_DRVR_USB >= 1)

#if (PROJ_CONFIG_USE_DRVR_CAMERA >= 1)

  //create Camera task
  eCamera_Request_Param_Init(&tCamera_Req);
  eOSAL_Task_Param_Init(&tOSAL_Task_Param);
  tCamera_Req.eRequestID = CAMERA_REQUEST_TASK_PARAM;
  tCamera_Req.pCamera_Task_Param = &tOSAL_Task_Param;
  eEC = eCamera_Request(&tCamera_Req);

  if(eEC == ER_OK)
  {
    eEC = eOSAL_Task_Create(&tOSAL_Task_Param);
  }

#endif //#if (PROJ_CONFIG_USE_DRVR_CAMERA >= 1)

  if(eEC == ER_OK)
  {
    eOSAL_OS_start();
  }

  vDEBUG_ASSERT("main() process start fail", eEC == ER_OK);

  return;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
