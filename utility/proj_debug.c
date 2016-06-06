//*****************************************************************************
//
// proj_debug.c - project wide debug api
//
// Copyright (c) notice
//
//*****************************************************************************
#ifndef DEBUG

#else
#ifndef __PROJ_DEBUG_C__
#define __PROJ_DEBUG_C__
//*****************************************************************************
// includes
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "utils_inc/error_codes.h"
#include "utils_inc/osal.h"
#include "utils_inc/proj_debug.h"

#include "board.h"

#include "stm32f4xx.h"

#include "cmsis_device.h"
#include "core_cm4.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//platform defines
#define DBG_STM32F4


#define SYS_HALT_MSG  "****!!!!SYS HALT!!!!****\r\n"
#define SYS_ASSERT_RST_MSG  "****!!!!SYS ASSERT RESET!!!!****\r\n"

#define DBG_CRNL "\r\n"
#define DBG_CRNL_LEN 2
#define DBG_STR_LEN 64

#define DBG_RCV_BUFF_LEN    128

#define DBG_LOG_EN_NONE           0x0000001
#define DBG_LOG_EN_APP            0x0000002
#define DBG_LOG_EN_DRIVER         0x0000004
#define DBG_LOG_EN_PLATFORM       0x0000008
#define DBG_LOG_EN_THIRD_PARTY    0x0000010
#define DBG_LOG_EN_UTILITY        0x0000020

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

char cDBG_String[DBG_STR_LEN];
#if (DEBUG_CFG_DEBUG_IN >= 1)
char cDebug_Context_Home[] = "\r\n                                           \
                              Home debug menu                            \r\n\
                              Enter a number from below and press enter: \r\n\
                              1)Debug Level                              \r\n\
                              ->";

char cDebug_Context_Log_Level[] = "\r\n                                       \
                                   Log level enable/disable menu          \r\n\
                                   Enter a log level to enable/disable    \r\n\
                                   [ ] = disabled, [X] = enabled          \r\n\
                                   1)APP[%c]                              \r\n\
                                   2)DRIVER[%c]                           \r\n\
                                   3)PLATFORM[%c]                         \r\n\
                                   4)THIRD_PARTY[%c]                      \r\n\
                                   5)UTILITY[%c]                          \r\n\
                                   ->";
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef enum DEBUG_IN_CONTEXT
{
  DEBUG_CONTEXT_NONE,
  DEBUG_CONTEXT_HOME,
  DEBUG_CONTEXT_LOG_LEVEL,
  //DEBUG_CONTEXT_tbd,
  DEBUG_CONTEXT_LIMIT,
}eDEBUG_IN_CONTEXT;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct tDebug_Activity_State
{
  bool bIs_Debug_UART_Setup;

#if (DEBUG_CFG_DEBUG_IN >= 1)
  eDEBUG_IN_CONTEXT eDbg_In_Context;
  char cDbg_Rcv_Buff[DBG_RCV_BUFF_LEN];
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

  uint32_t  uiLog_Level_En;
}tDebug_Activity_State;

tDebug_Activity_State tDebug_AS =
{
  false, //bool bIs_Debug_UART_Setup;

#if (DEBUG_CFG_DEBUG_IN >= 1)
  0, //eDEBUG_IN_CONTEXT eDbg_In_Context;
  //char cDbg_Rcv_Buff[DBG_RCV_BUFF_LEN];
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

  0,     //uint32_t  uiLog_Level_En;
};


#ifdef DBG_STM32F4
  USART_HandleTypeDef * ptDebug_UART_Handle = NULL;
  USART_HandleTypeDef tDebug_UART_Handle;
#elif
  #error "No debug interface handles defined"
#endif

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

void vDbg_Interface_Out(char * cString, uint32_t uiLen);
bool bDbg_Detect_Debugger(void);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: [put name here]
* description:
*   [put description here]
* parameters:
*   type - value: value description (in order from left to right)
*   example:
*   bool - true: do action when set to true
*        - false: do action when set to false
*   Example_Struct - pStruct:
*     (int)->iFoo: Structure member description
*     (uint32_t *)->pFoo: Structure member description
*     (eEXAMPLE_ENUM)->eFOO: Structure member description
*
* return value description:
*   type - value: value description
*   examples:
*   bool - true: did function action and result is true
*        - false: did function action and result is false
*   uint32_t - : integer value after function action
*   Example_Struct - *: address of the created object
*                  - NULL: created object fail
******************************************************************************/
void vDbg_Interface_Out(char * cString, uint32_t uiLen)
{
#ifdef DBG_STM32F4
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  eHAL_Status = HAL_USART_Transmit(&tDebug_UART_Handle, (uint8_t *)cString, uiLen, 3000);

//  memset(cDBG_String, 0x00, (DBG_CRNL_LEN + 1));
//  memcpy(cDBG_String, DBG_CRNL, DBG_CRNL_LEN);
//  eHAL_Status = HAL_USART_Transmit(&tDebug_UART_Handle, (uint8_t *)cDBG_String, DBG_CRNL_LEN, 3000);
  eHAL_Status = HAL_USART_Transmit(&tDebug_UART_Handle, (uint8_t *)DBG_CRNL, DBG_CRNL_LEN, 3000);

  if(eHAL_Status != HAL_OK)
  {
    while(1){};
  }
#elif
  #error "No debug interface out api defined"
#endif

  return;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: [put name here]
* description:
*   [put description here]
* parameters:
*   type - value: value description (in order from left to right)
*   example:
*   bool - true: do action when set to true
*        - false: do action when set to false
*   Example_Struct - pStruct:
*     (int)->iFoo: Structure member description
*     (uint32_t *)->pFoo: Structure member description
*     (eEXAMPLE_ENUM)->eFOO: Structure member description
*
* return value description:
*   type - value: value description
*   examples:
*   bool - true: did function action and result is true
*        - false: did function action and result is false
*   uint32_t - : integer value after function action
*   Example_Struct - *: address of the created object
*                  - NULL: created object fail
******************************************************************************/
bool bDbg_Detect_Debugger(void)
{
  bool bDebugger_Preset = false;
#ifdef DBG_STM32F4
#elif
  #error "No debuger detect api defined"
#endif
  return bDebugger_Present;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#if(DEBUG_CFG_DEBUG_OUT >= 1)
//*****************************************************************************
// name: vDEBUG
// description: sends a message out the debug message interface
// param description: none
// return value description: none
//*****************************************************************************
void vDEBUG(char * cMsg, ...)
{
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;
  uint16_t uiLen = 0;
  uiLen = strlen(cMsg);

  if(tDebug_AS.bIs_Debug_UART_Setup == true)
  {
    if((uiLen + 1) < DBG_STR_LEN)
    {
      memset(cDBG_String, 0x00, (uiLen + 1));
      memcpy(cDBG_String, cMsg, uiLen);

      vDbg_Interface_Out(cDBG_String, uiLen);

      if(eHAL_Status != HAL_OK)
      {
        while(1){};
      }

    }
  }
  return;
}
#endif //#if(DEBUG_OUT >= 1)

#if (DEBUG_CFG_DEBUG_IN >= 1)
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{

}

void vDEBUG_IN(char * pChar)
{
  if(*pChar == '\r')
  {

  }
  else if(('9' >= *pChar) | (*pChar >= '0'))
  {
    memcpy(tDebug_AS.cDbg_Rcv_Buff, &tDebug_AS.cDbg_Rcv_Buff[1], DBG_RCV_BUFF_LEN);
    tDebug_AS.cDbg_Rcv_Buff[0] = *pChar;
  }
  else
  {

  }
}
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

#if ((DEBUG_CFG_LOG_LEVEL >= 1) & (DEBUG_CFG_DEBUG_OUT >= 1) & (DEBUG_CFG_DEBUG_IN >= 1))
//*****************************************************************************
// name: vDEBUG_LVL
// description: sends a message out the debug message interface according to
//              enabled log levels.
// param description: none
// return value description: none
//*****************************************************************************
void vDEBUG_LVL(eDEBUG_LOG_LEVEL eLog_Lvl, char * cMsg, ...)
{
  return;
}
#endif //((DEBUG_LOG_LEVEL >= 1) & (DEBUG_OUT >= 1))


#if (DEBUG_CFG_ASSERT  >= 1)
void vDEBUG_ASSERT(char * cMsg,int iAssert)
{
//  ERROR_CODE eEC = ER_FAIL;

  if(iAssert)
  {
    return;
  }
  else
  {
#if (DEBUG_CFG_DEBUGGER_DETECT >= 1)
    //todo detct debugger eEC = eBSP_debugger_detect();
    if(bDgb_Detect_Debugger() == true)
    {
      vDEBUG(SYS_HALT_MSG);
      vDEBUG(cMsg);

      __ASM volatile("BKPT 0x00A8");
      while(1){};
    }
    else
    {
      vDEBUG(SYS_HALT_MSG);
      vDEBUG(cMsg);

      //todo: disable interrupts
      //todo: system reset
      while(1){};
    }
#else
    __ASM volatile("BKPT 0x00A8");
    while (1){}
#endif  //#if (DEBUG_CFG_DEBUGGER_DETECT >= 1)
  }
}
#endif //#if (DEBUG_CFG_ASSERT  >= 1)

#if (DEBUG_CFG_GPIO_A  >= 1)
/******************************************************************************
* name: vDEBUG_GPIO_SET_A
* description: sets high DEBUG_GPIO_PIN_A
* param description:
* return value description:
******************************************************************************/
void vDEBUG_GPIO_SET_A(void)
{
  //todo: set gpio pin
  __ASM volatile("BKPT 0x00A8");
  return;
}

/******************************************************************************
* name: vDEBUG_GPIO_CLR_A
* description: sets low DEBUG_GPIO_PIN_A
* param description:
* return value description:
******************************************************************************/
void vDEBUG_GPIO_CLR_A(void)
{
  //todo: clear pin
  __ASM volatile("BKPT 0x00A8");
  return;
}

/******************************************************************************
* name: vDEBUG_GPIO_TOGGLE_1
* description: toggles DEBUG_GPIO_PIN_1
* param description:
* return value description:
******************************************************************************/
void vDEBUG_GPIO_TOGGLE_A(void)
{
  //todo: toggle pin
  __ASM volatile("BKPT 0x00A8");
  return;
}
#endif //#if (DEBUG_CFG_GPIO_A  >= 1)

/******************************************************************************
* name: vDEBUG_init
* description: initalizes the debug message interface and the debug gpio pins
* param description: none
* return value description: none
******************************************************************************/
void vDEBUG_init(void)
{
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;
  GPIO_InitTypeDef      tDebug_UART_GPIO_Init;

  if(tDebug_AS.bIs_Debug_UART_Setup == false)
  {
//    ptDebug_UART_Handle = malloc(sizeof(USART_HandleTypeDef));
//
//    if(ptDebug_UART_Handle != NULL)
//    {
//      tDebug_UART_GPIO_Init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
//      tDebug_UART_GPIO_Init.Mode = GPIO_MODE_AF_PP;
//      tDebug_UART_GPIO_Init.Pull = GPIO_NOPULL;
//      tDebug_UART_GPIO_Init.Speed = GPIO_SPEED_HIGH;
//      tDebug_UART_GPIO_Init.Alternate = GPIO_AF8_USART6;
//      HAL_GPIO_Init(GPIOC,&tDebug_UART_GPIO_Init);
//
//      ptDebug_UART_Handle->Init.BaudRate = 115200;
//      ptDebug_UART_Handle->Init.WordLength = USART_WORDLENGTH_8B;
//      ptDebug_UART_Handle->Init.StopBits = USART_STOPBITS_1;
//      ptDebug_UART_Handle->Init.Parity = USART_PARITY_NONE;
//      ptDebug_UART_Handle->Init.Mode = USART_MODE_TX;
//      ptDebug_UART_Handle->Init.CLKPolarity = USART_POLARITY_LOW;
//      ptDebug_UART_Handle->Init.CLKPhase = USART_PHASE_1EDGE;
//      ptDebug_UART_Handle->Init.CLKLastBit = USART_LASTBIT_DISABLE;
//      ptDebug_UART_Handle->Instance = USART1;
//      ptDebug_UART_Handle->pTxBuffPtr = (uint8_t *)cDBG_String;
//
//      eHAL_Status = HAL_USART_Init(ptDebug_UART_Handle);
//      if(eHAL_Status == HAL_OK)
//      {
//        tDebug_AS.bIs_Debug_UART_Setup = true;
//      }
//    }

    __GPIOC_CLK_ENABLE();
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    __USART6_CLK_ENABLE();
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    tDebug_UART_GPIO_Init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    tDebug_UART_GPIO_Init.Mode = GPIO_MODE_AF_PP;
    tDebug_UART_GPIO_Init.Pull = GPIO_NOPULL;
    tDebug_UART_GPIO_Init.Speed = GPIO_SPEED_HIGH;
    tDebug_UART_GPIO_Init.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC,&tDebug_UART_GPIO_Init);

    tDebug_UART_Handle.Init.BaudRate = 115200;
    tDebug_UART_Handle.Init.WordLength = USART_WORDLENGTH_8B;
    tDebug_UART_Handle.Init.StopBits = USART_STOPBITS_1;
    tDebug_UART_Handle.Init.Parity = USART_PARITY_NONE;
    tDebug_UART_Handle.Init.Mode = USART_MODE_TX;
    tDebug_UART_Handle.Init.CLKPolarity = USART_POLARITY_LOW;
    tDebug_UART_Handle.Init.CLKPhase = USART_PHASE_1EDGE;
    tDebug_UART_Handle.Init.CLKLastBit = USART_LASTBIT_DISABLE;
    tDebug_UART_Handle.Instance = USART6;
//    tDebug_UART_Handle.pTxBuffPtr = (uint8_t *)cDBG_String;

    eHAL_Status = HAL_USART_Init(&tDebug_UART_Handle);
    if(eHAL_Status == HAL_OK)
    {
      tDebug_AS.bIs_Debug_UART_Setup = true;
    }
  }

#if (DEBUG_CFG_DEBUG_IN >= 1)
  vDEBUG(cDebug_Context_Home);
  tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_HOME;
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

  //todo detect debugger eEC = eBSP_debugger_detect();
//  if(eEC == ER_OK)
//  {
//    vDEBUG("Debugger connected!");
//  }
//  else
//  {
//    vDEBUG("Debugger NOT connected!");
//  }

  return;
}
#endif //__PROJ_DEBUG_C__
#endif //DEBUG
