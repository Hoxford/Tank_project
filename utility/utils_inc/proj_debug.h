//*****************************************************************************
//
// proj_debug.h - project wide debug api include file
//
// Copyright (c) notice
//
//*****************************************************************************
#ifndef DEBUG
#ifndef __PROJ_DEBUG_H__
#define __PROJ_DEBUG_H__
//*****************************************************************************
// includes
//*****************************************************************************

//*****************************************************************************
// defines
//*****************************************************************************
#define __error__(a, b)
//#define vDEBUG(a, __VA_ARGS__)
#define vDEBUG(a, ...)
void vDEBUG_ASSERT(a, ...);
#define vDEBUG_GPIO_SET_1()
#define vDEBUG_GPIO_CLR_1()
#define vDEBUG_GPIO_TOGGLE_1()
#define vDEBUG_init()
//*****************************************************************************
// variables
//*****************************************************************************

//*****************************************************************************
// external variables
//*****************************************************************************

//*****************************************************************************
// enums
//*****************************************************************************

//*****************************************************************************
// structures
//*****************************************************************************
#endif //__PROJ_DEBUG_H__

#else
#ifndef __PROJ_DEBUG_H__
#define __PROJ_DEBUG_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#include "proj_debug_config.h"

/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#if (DEBUG_LOG_LEVEL == 0)
  #define vDEBUG_LVL(x, a, ...)
#elif (DEBUG_LOG_LEVEL == 1)
#else
  #error "DEBUG_LOG_LEVEL invalid value, default 0"
#endif

#if (DEBUG_PERSISTANT_SETTINGS == 0)
#elif (DEBUG_PERSISTANT_SETTINGS == 1)
#else
  #error "DEBUG_PERSISTANT_SETTINGS invalid value, default 0"
#endif

#if (DEBUG_OUT == 0)
  #define vDEBUG(a, ...)
#elif (DEBUG_OUT  == 1)
#else
  #error "DEBUG_OUT invalid value, default 0"
#endif

#if (DEBUG_IN == 0)
#elif (DEBUG_IN  == 1)
#else
  #error "DEBUG_IN invalid value, default 0"
#endif

#if (DEBUG_ASSERT == 0)
  #define vDEBUG_ASSERT(a, x)
#elif (DEBUG_ASSERT == 1)
#else
  #error "DEBUG_ASSERT invalid value, default 0"
#endif

#if (DEBUG_GPIO_A == 0)
  #define vDEBUG_GPIO_SET_A()
  #define vDEBUG_GPIO_CLR_A()
  #define vDEBUG_GPIO_TOGGLE_A()
#elif (DEBUG_GPIO_A == 1)
#else
  #error "DEBUG_GPIO_A invalid value, default 0"
#endif

// Debug uart mappings
//
#define  DEBUG_SYSCTL_PERIPH_GPIO  SYSCTL_PERIPH_GPIOC
#define  DEBUG_SYSCTL_PERIPH_UART  SYSCTL_PERIPH_UART3
#define  DEBUG_UART                UART3_BASE
#define  DEBUG_UART_PIN_PORT       GPIO_PORTC_BASE
#define  DEBUG_TX_PIN_MUX_MODE     GPIO_PC7_U3TX
#define  DEBUG_RX_PIN_MUX_MODE     GPIO_PC6_U3RX
#define  DEBUG_TX_PIN              GPIO_PIN_7
#define  DEBUG_RX_PIN              GPIO_PIN_6

#define  DEBUG_BAUD                115200
#define  DEBUG_UART_CONFIG         (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)

//Debug pin A mappings
//
#define DEBUG_GPIO_PORT_1          GPIO_PORTB_BASE
#define DEBUG_GPIO_PIN_1           GPIO_PIN_3
#define DEBUG_GPIO_PIN_SET_1       GPIO_PIN_3
#define DEBUG_GPIO_PIN_CLR_1       0

/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#if (DEBUG_LOG_LEVEL >= 1)
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
#if (DEBUG_OUT  >= 1)
  void vDEBUG(char * cMsg,...);
#endif //#if (DEBUG_OUT  == 1)

#if  ((DEBUG_LOG_LEVEL >= 1) & (DEBUG_OUT >= 1) & (DEBUG_IN >= 1))
  void vDEBUG_LVL(eDEBUG_LOG_LEVEL eLog_Lvl, char * cMsg, ...);
#endif //#if (DEBUG_LOG_LEVEL >= 1)

#if (DEBUG_ASSERT >= 1)
  void vDEBUG_ASSERT(char * cMsg,int iAssert);
#endif //#if (DEBUG_ASSERT >= 1)

#if (DEBUG_GPIO_A  >= 1)
  void vDEBUG_GPIO_SET_A(void);
  void vDEBUG_GPIO_CLR_A(void);
  void vDEBUG_GPIO_TOGGLE_A(void);
#endif //#if (DEBUG_GPIO_A  >= 1)

void vDEBUG_init(void);

#endif //__PROJ_DEBUG_H__
#endif //DEBUG
