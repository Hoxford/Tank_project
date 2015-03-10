/******************************************************************************
*
* clock.c - real world clock code that keeps track of the milisecond, second,
*   minute, hour, day, month and year
* Copyright (c) notice
*
******************************************************************************/
#ifndef __CLOCK_C__
#define __CLOCK_C__
/******************************************************************************
* includes
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "utils_inc/error_codes.h"
#include "utils_inc/clock.h"
#include "board.h"
/******************************************************************************
* defines
******************************************************************************/

/******************************************************************************
* variables
******************************************************************************/
uint32_t  uiSystem_total_ms_count = 0;
uintmax_t  uiSys_ms   = 0;
uint16_t   uiSys_sec  = 0;
uint16_t   uiSys_min  = 0;
uint16_t   uiSys_hour = 0;
uintmax_t  uiSys_TotalRuntime_ms   = 0;
uint16_t   uiSys_TotalRuntime_sec  = 0;
uint16_t   uiSys_TotalRuntime_min  = 0;
uint16_t   uiSys_TotalRuntime_hour = 0;

uintmax_t  uiSanity_check = 0;

/******************************************************************************
* external variables
******************************************************************************/

/******************************************************************************
* enums
******************************************************************************/

/******************************************************************************
* structures
******************************************************************************/

/******************************************************************************
* external functions
******************************************************************************/

/******************************************************************************
* private function declarations
******************************************************************************/

/******************************************************************************
* private functions
******************************************************************************/
/******************************************************************************
* name:
* description:
* param description:
* return value description:
******************************************************************************/
//int example_PrivateFunction(param_1, param_2)
//{
//}

/******************************************************************************
* public functions
******************************************************************************/
/******************************************************************************
* name:
* description:
* param description:
* return value description:
******************************************************************************/
ERROR_CODE eClock_process_init(void)
{
  ERROR_CODE eEC = ER_OK;
  eBSP_Get_Current_ms_count(&uiSystem_total_ms_count);

  //go through and update the total runtime varialbes
  if(uiSystem_total_ms_count >= 1000)
  {
    uiSys_ms = uiSystem_total_ms_count;
    uiSys_TotalRuntime_ms = uiSystem_total_ms_count;

    while(uiSys_ms >= 1000)
    {
      uiSys_ms = uiSys_ms - 1000;
      uiSys_sec++;
    }

    if(uiSys_sec >= 60)
    {
      while(uiSys_sec >= 60)
      {
        uiSys_sec = uiSys_sec - 60;
        uiSys_min++;
      }

      if(uiSys_min >= 60)
      {
        while(uiSys_min >= 60)
        {
          uiSys_min = uiSys_min - 60;
          uiSys_hour++;
        }

        if(uiSys_hour >= 24)
        {
          while(uiSys_hour >= 24)
          {
            uiSys_hour = uiSys_hour - 24;
          }
        }else{/*do nothing*/}
      }else{/*do nothing*/}
    }else{/*do nothing*/}

    while(uiSys_TotalRuntime_ms >= 1000)
    {
      uiSys_TotalRuntime_ms = uiSys_TotalRuntime_ms - 1000;
      uiSys_TotalRuntime_sec++;
    }

    if(uiSys_TotalRuntime_sec >= 60)
    {
      while(uiSys_TotalRuntime_sec >= 60)
      {
        uiSys_TotalRuntime_sec = uiSys_TotalRuntime_sec - 60;
        uiSys_TotalRuntime_min++;
      }

      if(uiSys_TotalRuntime_min >= 60)
      {
        while(uiSys_TotalRuntime_min >= 60)
        {
          uiSys_TotalRuntime_min = uiSys_TotalRuntime_min - 60;
          uiSys_TotalRuntime_hour++;
        }
      }else{/*do nothing*/}
    }else{/*do nothing*/}
  }else{/*do nothing*/}

  if((uiSystem_total_ms_count == 0) &\
     (uiSys_ms                == 0) &\
     (uiSys_sec               == 0) &\
     (uiSys_min               == 0) &\
     (uiSys_hour              == 0) &\
     (uiSys_TotalRuntime_ms   == 0) &\
     (uiSys_TotalRuntime_sec  == 0) &\
     (uiSys_TotalRuntime_min  == 0) &\
     (uiSys_TotalRuntime_hour == 0))
  {
    eEC = ER_FAIL;
  }
  else
  {
    eEC = ER_OK;
  }
  return eEC;
}
/******************************************************************************
* name:
* description:
* param description:
* return value description:
******************************************************************************/
ERROR_CODE eClock_get_total_runtime(uintmax_t *uiTotal_time)
{
  ERROR_CODE eEC = ER_OK;
  uintmax_t uiCurrent_time = 0;

  uiCurrent_time = uiSystem_total_ms_count;

  if(uiCurrent_time != 0)
  {
    *uiTotal_time = uiCurrent_time;
    eEC = ER_OK;
  }
  else
  {
    eEC = ER_FAIL;
  }
  return eEC;

}

/******************************************************************************
* name:
* description:
* param description:
* return value description:
******************************************************************************/
ERROR_CODE eClock_get_total_real_runtime(uintmax_t *uiTotal_time)
{
  ERROR_CODE eEC = ER_OK;
  uintmax_t uiCurrent_time = 0;

  uiCurrent_time |= uiSys_ms;
  uiCurrent_time |= (uiSys_sec  << 12);
  uiCurrent_time |= (uiSys_min  << 20);
  uiCurrent_time |= (uiSys_hour << 28);

  if(uiCurrent_time != 0)
  {
    *uiTotal_time = uiCurrent_time;
    eEC = ER_OK;
  }
  else
  {
    eEC = ER_FAIL;
  }
  return eEC;
}

/******************************************************************************
* name:
* description:
* param description:
* return value description:
******************************************************************************/
ERROR_CODE eClock_get_time(uintmax_t *uiClock_time)
{
  ERROR_CODE eEC = ER_OK;
  uintmax_t uiCurrent_time = 0;

  uiCurrent_time |= uiSys_TotalRuntime_ms;
  uiCurrent_time |= (uiSys_TotalRuntime_sec  << 12);
  uiCurrent_time |= (uiSys_TotalRuntime_min  << 20);
  uiCurrent_time |= (uiSys_TotalRuntime_hour << 28);

  if(uiCurrent_time != 0)
  {
    *uiClock_time = uiCurrent_time;
    eEC = ER_OK;
  }
  else
  {
    eEC = ER_FAIL;
  }
  return eEC;
}

/******************************************************************************
* name:
* description:
* param description:
* return value description:
******************************************************************************/
ERROR_CODE eClock_process(void)
{
  ERROR_CODE eEC = ER_OK;
  uint32_t uiSystem_curr_ms_count;
  uintmax_t uiSystem_prev_ms_count;
  uintmax_t uiSystem_ms_count_diff;

  //store the previous ms count
  uiSystem_prev_ms_count = uiSystem_total_ms_count;
  //get the current ms count
  eBSP_Get_Current_ms_count(&uiSystem_curr_ms_count);

  //check if the ms counter has ticked
  if(uiSystem_curr_ms_count > uiSystem_prev_ms_count)
  {
    //reset the sanity check control variable
    uiSanity_check = 0;

    //store the total ms count
    uiSystem_total_ms_count = uiSystem_curr_ms_count;

    //get the milisecond differences
    uiSystem_ms_count_diff = uiSystem_curr_ms_count - uiSystem_prev_ms_count;

    uiSys_ms += uiSystem_ms_count_diff;
    uiSys_TotalRuntime_ms += uiSystem_ms_count_diff;

    //go through and update the time tracking variables. Update and correct them if the system
    //was asleep for a long time.
    if(uiSys_ms >= 1000)
    {
      while(uiSys_ms >= 1000)
      {
        uiSys_ms = uiSys_ms - 1000;
        uiSys_sec++;
      }

      if(uiSys_sec >= 60)
      {
        while(uiSys_sec >= 60)
        {
          uiSys_sec = uiSys_sec - 60;
          uiSys_min++;
        }

        if(uiSys_min >= 60)
        {
          while(uiSys_min >= 60)
          {
            uiSys_min = uiSys_min - 60;
            uiSys_hour++;
          }

          if(uiSys_hour >= 24)
          {
            while(uiSys_hour >= 24)
            {
              uiSys_hour = uiSys_hour - 24;
            }
          }else{/*do nothing*/}
        }else{/*do nothing*/}
      }else{/*do nothing*/}
    }else{/*do nothing*/}

    //go through and update the total runtime varialbes
    if(uiSys_TotalRuntime_ms >= 1000)
    {
      while(uiSys_TotalRuntime_ms >= 1000)
      {
        uiSys_TotalRuntime_ms = uiSys_TotalRuntime_ms - 1000;
        uiSys_TotalRuntime_sec++;
      }

      if(uiSys_TotalRuntime_sec >= 60)
      {
        while(uiSys_TotalRuntime_sec >= 60)
        {
          uiSys_TotalRuntime_sec = uiSys_TotalRuntime_sec - 60;
          uiSys_TotalRuntime_min++;
        }

        if(uiSys_TotalRuntime_min >= 60)
        {
          while(uiSys_TotalRuntime_min >= 60)
          {
            uiSys_TotalRuntime_min = uiSys_TotalRuntime_min - 60;
            uiSys_TotalRuntime_hour++;
          }
        }else{/*do nothing*/}
      }else{/*do nothing*/}
    }else{/*do nothing*/}
  }
  else
  {
    uiSanity_check++;
    if(uiSanity_check == 0xFFFFFFFFFFFFFFFF)
    {
      eEC = ER_FAIL;
      uiSanity_check = 0;
    }
  }

  return eEC;
}

#endif //__CLOCK_C__
