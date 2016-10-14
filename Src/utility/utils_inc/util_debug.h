//*****************************************************************************
//
// proj_debug.h - project wide debug api include file
//
// Copyright (c) notice
//
//*****************************************************************************

#ifndef __PROJ_DEBUG_H__
#define __PROJ_DEBUG_H__
//*****************************************************************************
// includes
//*****************************************************************************
#include <proj_inc/proj_main.h>

#if (PROJ_CONFIG_USE_UTIL_DEBUG <= 0)
//*****************************************************************************
// defines
//*****************************************************************************
#define __error__(a, b)
//#define vDEBUG(a, __VA_ARGS__)
#define vDEBUG(a, ...)
#define vDEBUG_ASSERT(a, ...)
#define vDEBUG_GPIO_SET_1()
#define vDEBUG_GPIO_CLR_1()
#define vDEBUG_GPIO_TOGGLE_1()
#define vDEBUG_init()

#else
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#include "util_debug_config.h"

/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#if (DEBUG_CFG_LOG_LEVEL == 0)
  #define vDEBUG_LVL(x, a, ...)
#elif (DEBUG_CFG_LOG_LEVEL == 1)
#else
  #error "DEBUG_LOG_LEVEL invalid value, default 0"
#endif

#if (DEBUG_CFG_PERSISTANT_SETTINGS == 0)
#elif (DEBUG_CFG_PERSISTANT_SETTINGS == 1)
#else
  #error "DEBUG_PERSISTANT_SETTINGS invalid value, default 0"
#endif

#if (DEBUG_CFG_DEBUG_OUT == 0)
  #define vDEBUG(a, ...)
#elif (DEBUG_CFG_DEBUG_OUT  == 1)
#else
  #error "DEBUG_OUT invalid value, default 0"
#endif

#if (DEBUG_CFG_DEBUG_IN == 0)
#elif (DEBUG_CFG_DEBUG_IN  == 1)
#else
  #error "DEBUG_IN invalid value, default 0"
#endif

#if (DEBUG_CFG_ASSERT == 0)
  #define vDEBUG_ASSERT(a, x) ((void)(x))
#elif (DEBUG_CFG_ASSERT == 1)
#else
  #error "DEBUG_ASSERT invalid value, default 0"
#endif

#if (DEBUG_CFG_GPIO_A == 0)
  #define vDEBUG_GPIO_SET_A()
  #define vDEBUG_GPIO_CLR_A()
  #define vDEBUG_GPIO_TOGGLE_A()
#elif (DEBUG_CFG_GPIO_A == 1)
#else
  #error "DEBUG_CFG_GPIO_A invalid value, default 0"
#endif

#if (DEBUG_CFG_GPIO_B == 0)
  #define vDEBUG_GPIO_SET_B()
  #define vDEBUG_GPIO_CLR_B()
  #define vDEBUG_GPIO_TOGGLE_B()
#elif (DEBUG_CFG_GPIO_B == 1)
#else
  #error "DEBUG_CFG_GPIO_B invalid value, default 0"
#endif

#if (DEBUG_CFG_GPIO_C == 0)
  #define vDEBUG_GPIO_SET_C()
  #define vDEBUG_GPIO_CLR_C()
  #define vDEBUG_GPIO_TOGGLE_C()
#elif (DEBUG_CFG_GPIO_C == 1)
#else
  #error "DEBUG_CFG_GPIO_C invalid value, default 0"
#endif

#if (DEBUG_CFG_GPIO_D == 0)
  #define vDEBUG_GPIO_SET_D()
  #define vDEBUG_GPIO_CLR_D()
  #define vDEBUG_GPIO_TOGGLE_D()
#elif (DEBUG_CFG_GPIO_D == 1)
#else
  #error "DEBUG_CFG_GPIO_D invalid value, default 0"
#endif

/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#if (DEBUG_CFG_LOG_LEVEL >= 1)
typedef enum LOG_LEVEL
{
  LOG_LVL_NONE = 0,
  LOG_LVL_APP,
  LOG_LVL_DRIVER,
  LOG_LVL_PLATFORM,
  LOG_LVL_THIRD_PARTY,
  LOG_LVL_UTILITY,
  LOG_LVL_MAX,
}eDEBUG_LOG_LEVEL;
#endif //#if (DEBUG_LOG_LEVEL >= 1)


/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

void __error__(char *pcFilename, uint32_t ui32Line);
//void vUSB_driverlib_out(char *pcFilename, uint32_t ui32Line); //USB driverlib debug api
#if (DEBUG_CFG_DEBUG_OUT  >= 1)
  void vDEBUG(char * cMsg,...);
#endif //#if (DEBUG_CFG_DEBUG_OUT  == 1)

#if  ((DEBUG_CFG_LOG_LEVEL >= 1) & (DEBUG_CFG_DEBUG_OUT >= 1) & (DEBUG_CFG_DEBUG_IN >= 1))
  void vDEBUG_LVL(eDEBUG_LOG_LEVEL eLog_Lvl, char * cMsg, ...);
#endif //#if (DEBUG_LOG_LEVEL >= 1)

#if (DEBUG_CFG_ASSERT >= 1)
  void vDEBUG_ASSERT(char * cMsg,int iAssert);
#endif //#if (DEBUG_CFG_ASSERT >= 1)

#if (DEBUG_CFG_GPIO_A  >= 1)
  void vDEBUG_GPIO_SET_A(void);
  void vDEBUG_GPIO_CLR_A(void);
  void vDEBUG_GPIO_TOGGLE_A(void);
#endif //#if (DEBUG_GPIO_A  >= 1)

void vDEBUG_init(void);

#endif //__PROJ_DEBUG_H__
#endif //DEBUG
