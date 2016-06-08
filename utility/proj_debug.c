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
#define DBG_STR_LEN 256

#define DBG_RCV_BUFF_LEN    128

#define DBG_LOG_EN_NONE           0x0000001
#define DBG_LOG_EN_APP            0x0000002
#define DBG_LOG_EN_DRIVER         0x0000004
#define DBG_LOG_EN_PLATFORM       0x0000008
#define DBG_LOG_EN_THIRD_PARTY    0x0000010
#define DBG_LOG_EN_UTILITY        0x0000020

//platform specific defines
#ifdef DBG_STM32F4
  #define USART_HANDLE USART_HandleTypeDef
  #if (DEBUG_CFG_DEBUG_IN >= 1)
    #define DBG_INF_IRQ_HANDLER       USART6_IRQHandler
    #define DBG_INF_IRQ_RCV_HANDLER   HAL_UART_RxCpltCallback
  #else
    #error "DBG_INF_IRQ not defined!"
  #endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
#else
  #error "Platform not defined!"
#endif

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

char cDBG_String[DBG_STR_LEN];

char cDebug_Startup_Msg[] = "\r\n\
Project %s                   \r\n\
Firmware Version %i.%i.%i    \r\n";

#if (DEBUG_CFG_DEBUG_IN >= 1)

char cRcv_Char = 0;

char cDebug_Context_Home[] =              "\r\n\
Home debug menu                            \r\n\
Enter a number from below and press enter: \r\n\
1)Debug Level                              \r\n\
2)Debug Control                            \r\n\
3)System Control                           \r\n\
4)Application Control                      \r\n\
0)Shut down                                \r\n\
->";

char cDebug_Context_Log_Level[] =     "\r\n\
Log level enable/disable menu          \r\n\
Enter a log level to enable/disable    \r\n\
[ ] = disabled, [X] = enabled          \r\n\
1)APP[%c]                              \r\n\
2)DRIVER[%c]                           \r\n\
3)PLATFORM[%c]                         \r\n\
4)THIRD_PARTY[%c]                      \r\n\
5)UTILITY[%c]                          \r\n\
0)Exit                                 \r\n\
->";

char cDebug_Context_Debug_Control[] = "\r\n\
Debug Control menu                     \r\n\
Enter an option from below             \r\n\
[ ] = disabled, [X] = enabled          \r\n\
1)Debug Out[%c]                        \r\n\
2)Debug Pin A[%c]                      \r\n\
3)Debug Pin A Set                      \r\n\
4)Debug Pin A Clear                    \r\n\
5)Debug Pin A Toggle                   \r\n\
6)Debug Pin B[%c]                      \r\n\
7)Debug Pin B Set                      \r\n\
8)Debug Pin B Clear                    \r\n\
9)Debug Pin B Toggle                   \r\n\
0)Exit                                 \r\n\
->";

char cDebug_Context_System_Control[] = "\r\n\
System Control menu                     \r\n\
Enter an option from below              \r\n\
[ ] = disabled, [X] = enabled           \r\n\
1)System Reset                          \r\n\
2)Wifi[%c]                              \r\n\
3)Wifi Reset                            \r\n\
3)Bluetooth[%c]                         \r\n\
4)Bluetooth Reset                       \r\n\
0)Exit                                  \r\n\
->";

char cDebug_Context_Application_Control[] = "\r\n\
Application Control menu                     \r\n\
Enter an option from below                   \r\n\
[ ] = disabled, [X] = enabled                \r\n\
0)Exit                                       \r\n\
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
  DEBUG_CONTEXT_SYS_CTRL,
  DEBVUG_CONTEXT_APP_CTRL,
  //DEBUG_CONTEXT_tbd,
  DEBUG_CONTEXT_LIMIT,
}eDEBUG_IN_CONTEXT;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct tDebug_Activity_State
{
  bool bIs_Debug_Intf_Setup;

#if (DEBUG_CFG_DEBUG_IN >= 1)
  eDEBUG_IN_CONTEXT eDbg_In_Context;
  char cDbg_Rcv_Buff[DBG_RCV_BUFF_LEN];
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

  uint32_t  uiLog_Level_En;
}tDebug_Activity_State;

tDebug_Activity_State tDebug_AS =
{
  false, //bool bIs_Debug_Intf_Setup;

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

//Debug Interface handles
USART_HANDLE * ptDebug_UART_Handle = NULL;
USART_HANDLE tDebug_UART_Handle;


/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

bool bDbg_Interface_Init(void);
void vDbg_Interface_Out(char * cString, uint32_t uiLen);
bool bDbg_Detect_Debugger(void);
void DBG_INF_IRQ_HANDLER(void);
void DBG_INF_IRQ_RCV_HANDLER(USART_HANDLE *pHandle);
void vDEBUG_IN(char * pChar);

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
bool bDbg_Interface_Init(void)
{
  bool              bEC = false;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;
  GPIO_InitTypeDef  tDebug_UART_GPIO_Init;

  __GPIOC_CLK_ENABLE();
  __USART6_CLK_ENABLE();
  __HAL_RCC_USART6_CLK_ENABLE();

  tDebug_UART_GPIO_Init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  tDebug_UART_GPIO_Init.Mode = GPIO_MODE_AF_PP;
  tDebug_UART_GPIO_Init.Pull = GPIO_PULLUP;
  tDebug_UART_GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  tDebug_UART_GPIO_Init.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(GPIOC,&tDebug_UART_GPIO_Init);

  tDebug_UART_Handle.Init.BaudRate = 115200;
  tDebug_UART_Handle.Init.WordLength = USART_WORDLENGTH_8B;
  tDebug_UART_Handle.Init.StopBits = USART_STOPBITS_1;
  tDebug_UART_Handle.Init.Parity = USART_PARITY_NONE;
#if (DEBUG_CFG_DEBUG_IN >= 1)
  tDebug_UART_Handle.Init.Mode = USART_MODE_TX_RX;
#else
  tDebug_UART_Handle.Init.Mode = USART_MODE_TX;
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
  tDebug_UART_Handle.Init.CLKPolarity = USART_POLARITY_LOW;
  tDebug_UART_Handle.Init.CLKPhase = USART_PHASE_1EDGE;
  tDebug_UART_Handle.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  tDebug_UART_Handle.Instance = USART6;

#if (DEBUG_CFG_DEBUG_IN >= 1)
  HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART6_IRQn);
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

  eHAL_Status = HAL_USART_Init(&tDebug_UART_Handle);
  if(eHAL_Status == HAL_OK)
  {
    bEC = true;
  }

  return bEC;
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
void vDbg_Interface_Out(char * cString, uint32_t uiLen)
{
#ifdef DBG_STM32F4
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  eHAL_Status = HAL_USART_Transmit(&tDebug_UART_Handle, (uint8_t *)cString, uiLen, 3000);

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
  bool bDebugger_Present = false;
#ifdef DBG_STM32F4
  //todo define api to detect debugger
  __ASM volatile("BKPT 0x00A8");
#elif
  #error "No debuger detect api defined"
#endif
  return bDebugger_Present;
}

#if (DEBUG_CFG_DEBUG_IN >= 1)
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
//void DBG_INF_IRQ_HANDLER(void)
//{
//  vDEBUG_IN(&cRcv_Char);
//  HAL_USART_Receive_IT(&tDebug_UART_Handle, (uint8_t *)&cRcv_Char, 1);
//  return;
//}

void DBG_INF_IRQ_RCV_HANDLER(USART_HANDLE *pHandle)
{
  if(pHandle == &tDebug_UART_Handle)
  {
    vDEBUG_IN(&cRcv_Char);
    HAL_USART_Receive_IT(&tDebug_UART_Handle, (uint8_t *)&cRcv_Char, 1);
  }
}

#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

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

  if(tDebug_AS.bIs_Debug_Intf_Setup == true)
  {
    if((uiLen + 1) < DBG_STR_LEN)
    {
      memset(cDBG_String, 0x00, (uiLen + 1));
      memcpy(cDBG_String, cMsg, uiLen);

      vDbg_Interface_Out(cDBG_String, uiLen);
    }
  }
  return;
}
#endif //#if(DEBUG_CFG_DEBUG_OUT >= 1)

#if (DEBUG_CFG_DEBUG_IN >= 1)
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
  vDEBUG_IN((char *)husart->pRxBuffPtr);
}

void vDEBUG_IN(char * pChar)
{
  static char cRcvd_Char;

  if(*pChar == '\r')
  {
    cRcvd_Char = *pChar;
  }
  else if(('9' >= *pChar) | (*pChar >= '0'))
  {
    cRcvd_Char = *pChar;
    memcpy(tDebug_AS.cDbg_Rcv_Buff, &tDebug_AS.cDbg_Rcv_Buff[1], DBG_RCV_BUFF_LEN);
    tDebug_AS.cDbg_Rcv_Buff[0] = cRcvd_Char;

    //echo character out
    vDbg_Interface_Out(&cRcvd_Char, 1);
  }
  else
  {
    //received invalid character, do nothing
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
  bool bEC = false;
  char * pStartup_Msg = NULL;
  int iStartup_Msg_Size = 0;

  if(tDebug_AS.bIs_Debug_Intf_Setup == false)
  {
    bEC = bDbg_Interface_Init();
    if(bEC == true)
    {
      tDebug_AS.bIs_Debug_Intf_Setup = true;
      iStartup_Msg_Size = strlen(cDebug_Startup_Msg) + strlen(DEBUG_PROJECT_NAME) + 9;
      pStartup_Msg = calloc(iStartup_Msg_Size, sizeof(char));
      snprintf(pStartup_Msg, iStartup_Msg_Size, cDebug_Startup_Msg, DEBUG_PROJECT_NAME, DEBUG_VERSION_MAJOR, DEBUG_VERSION_MINOR, DEBUG_VERSION_PATCH);
      vDbg_Interface_Out(pStartup_Msg, strlen(pStartup_Msg));
      free(pStartup_Msg);
    }else{/*do nothing*/}
  }

#if (DEBUG_CFG_DEBUGGER_DETECT >= 1)
  bEC = bDbg_Detect_Debugger();
  if(bEC == true)
  {
    vDEBUG("Debugger connected!");
  }
  else
  {
    vDEBUG("Debugger NOT connected!");
  }
#endif

#if (DEBUG_CFG_DEBUG_IN >= 1)
  //Display initial debug message and context menu

  vDbg_Interface_Out(cDebug_Context_Home, strlen(cDebug_Context_Home));
  tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_HOME;
  HAL_USART_Receive_IT(&tDebug_UART_Handle, (uint8_t *)&cRcv_Char, 1);
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

  return;
}
#endif //__PROJ_DEBUG_C__
#endif //DEBUG
