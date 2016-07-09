//*****************************************************************************
//
// proj_debug.c - project wide debug api
//
// Copyright (c) notice
//
//*****************************************************************************
#if (PROJ_CONFIG_USE_UTIL_DEBUG >= 1)

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

#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)
  #include "stm32f4xx_hal.h"
#else
  #error "Platform not defined!"
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
//#include "core_cm4.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define SYS_HALT_MSG  "****!!!!SYS HALT!!!!****\r\n"
#define SYS_ASSERT_RST_MSG  "****!!!!SYS ASSERT RESET!!!!****\r\n"

#define DBG_CRNL "\r\n"
#define DBG_CRNL_LEN 2
#define DBG_STR_LEN 256

#define DBG_RCV_BUFF_LEN    128

#define DBG_CONTEXT_MENU_DEFAULT_MENU    0xFF

#define DBG_LOG_EN_NONE           0x0000000
#define DBG_LOG_EN_APP            0x0000001
#define DBG_LOG_EN_DRIVER         0x0000002
#define DBG_LOG_EN_PLATFORM       0x0000004
#define DBG_LOG_EN_THIRD_PARTY    0x0000008
#define DBG_LOG_EN_UTILITY        0x0000010

//platform specific defines
#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)
  #define UART_HANDLE_T     UART_HandleTypeDef
  #define UART_DEBUG_HANDLE huart3
  #if (DEBUG_CFG_DEBUG_IN >= 1)
    #define DBG_INF_IRQ_HANDLER       USART3_IRQHandler
  #endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
#else
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
5)Driver Control                           \r\n\
0)Shut down                                \r\n\
->";

char cDebug_Context_Log_Level[] =     "\r\n\
Log level enable/disable menu          \r\n\
Enter a log level to enable/disable    \r\n\
[0] = disabled, [1] = enabled          \r\n\
1)APP[%i]                              \r\n\
2)DRIVER[%i]                           \r\n\
3)PLATFORM[%i]                         \r\n\
4)THIRD_PARTY[%i]                      \r\n\
5)UTILITY[%i]                          \r\n\
0)Exit                                 \r\n\
->";

char cDebug_Context_Debug_Control[] = "\r\n\
Debug Control menu                     \r\n\
Enter an option from below             \r\n\
[0] = disabled, [1] = enabled          \r\n\
1)Debug Out[%i]                        \r\n\
2)Debug Pin A[%i]                      \r\n\
3)Debug Pin A Set                      \r\n\
4)Debug Pin A Clear                    \r\n\
5)Debug Pin A Toggle                   \r\n\
6)Debug Pin B[%i]                      \r\n\
7)Debug Pin B Set                      \r\n\
8)Debug Pin B Clear                    \r\n\
9)Debug Pin B Toggle                   \r\n\
0)Exit                                 \r\n\
->";

char cDebug_Context_System_Control[] = "\r\n\
System Control menu                     \r\n\
Enter an option from below              \r\n\
[0] = disabled, [1] = enabled           \r\n\
1)System Reset                          \r\n\
2)Wifi[%i]                              \r\n\
3)Wifi Reset                            \r\n\
3)Bluetooth[%i]                         \r\n\
4)Bluetooth Reset                       \r\n\
0)Exit                                  \r\n\
->";

char cDebug_Context_Application_Control[] = "\r\n\
Application Control menu                     \r\n\
Enter an option from below                   \r\n\
[0] = disabled, [1] = enabled                \r\n\
0)Exit                                       \r\n\
->";

char cDebug_Context_Driver_Control[] = "\r\n\
Application Control menu                     \r\n\
Enter an option from below                   \r\n\
[0] = disabled, [1] = enabled                \r\n\
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
  DEBUG_CONTEXT_DBG_CTRL,
  DEBUG_CONTEXT_SYS_CTRL,
  DEBUG_CONTEXT_APP_CTRL,
  DEBUG_CONTEXT_DRVR_CTRL,
  DEBUG_CONTEXT_SHUTDOWN,
  //DEBUG_CONTEXT_tbd,
  DEBUG_CONTEXT_LIMIT,
}eDEBUG_IN_CONTEXT;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct tDebug_Activity_State
{
  bool bIs_Debug_Intf_Setup;
  bool bDebug_Pin_A_Set;
  bool bDebug_Pin_B_Set;

#if (DEBUG_CFG_DEBUG_IN >= 1)
  bool bDebug_Out;
  eDEBUG_IN_CONTEXT eDbg_In_Context;
  char cDbg_Rcv_Buff[DBG_RCV_BUFF_LEN];
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

  uint32_t  uiLog_Level_En;
}tDebug_Activity_State;

tDebug_Activity_State tDebug_AS =
{
  false, //bool bIs_Debug_Intf_Setup;
  false, //bool bDebug_Pin_A_Set;
  false, //bool bDebug_Pin_B_Set;
#if (DEBUG_CFG_DEBUG_IN >= 1)
  true,  //bool bDebug_Out;
  0, //eDEBUG_IN_CONTEXT eDbg_In_Context;
  //char cDbg_Rcv_Buff[DBG_RCV_BUFF_LEN];
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

  0,     //uint32_t  uiLog_Level_En;
};

//platform specific structures
#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)
  //Debug Interface handles
  extern UART_HandleTypeDef huart3;
  #if (DEBUG_CFG_DEBUG_IN >= 1)
    #define DBG_INF_IRQ_HANDLER       UART3_IRQHandler
  #endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
#else
#endif



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
//void DBG_INF_IRQ_RCV_HANDLER(USART_HANDLE *pHandle);
void vDEBUG_IN(char * pChar);
void vDbg_Menu_Context(int iOption);

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
  bool              bEC = true;

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
#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  if(tDebug_AS.bDebug_Out == true)
  {
    eHAL_Status = HAL_UART_Transmit(&UART_DEBUG_HANDLE, (uint8_t *)cString, uiLen, 3000);

    if(eHAL_Status != HAL_OK)
    {
      while(1){};
    }
  }
#else
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
#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)
  //todo define api to detect debugger
  __ASM volatile("BKPT 0x00A8");
#else
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
void DBG_INF_IRQ_HANDLER(void)
{
  HAL_UART_IRQHandler(&huart3);
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
//void DBG_INF_IRQ_RCV_HANDLER(USART_HANDLE *pHandle)
void HAL_UART_RxCpltCallback(UART_HANDLE_T *pHandle)
{
  if(pHandle == &huart3)
  {
    HAL_NVIC_ClearPendingIRQ(USART3_IRQn);
    vDEBUG_IN(&cRcv_Char);
    HAL_UART_Receive_IT(&huart3, (uint8_t *)&cRcv_Char, 1);
  }
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
void vDEBUG_IN(char * pChar)
{
  static char cRcvd_Char;
  int iLen = 0;
  int iMenu = 0;

  if(*pChar == '\r')
  {
    iLen = strlen(tDebug_AS.cDbg_Rcv_Buff);

    //Check received buff len
    //
    if((iLen == 0) | (iLen >= 2))
    {
      //Length is an invalid value, erase the receive buffer and re-display the current context menu
      memset(tDebug_AS.cDbg_Rcv_Buff, 0x00, DBG_RCV_BUFF_LEN);
      vDbg_Menu_Context(DBG_CONTEXT_MENU_DEFAULT_MENU);
    }
    else if((iLen == 1) | (iLen == 2))
    {
      //Length is a valid value, display the correct context menu
      iMenu = atoi(tDebug_AS.cDbg_Rcv_Buff);
      vDbg_Menu_Context(iMenu);

      //erase the receive buffer
      memset(tDebug_AS.cDbg_Rcv_Buff, 0x00, DBG_RCV_BUFF_LEN);
    }else{/*do nothing*/}

    cRcvd_Char = *pChar;
  }
  else if(('9' >= *pChar) & (*pChar >= '0'))
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
  return;
}

/******************************************************************************
* todo: DESCRIPTION
* name: vDbg_Menu_Context
* description:
*   [put description here]
* parameters:
*   int iOption - : Numerical value of the context menu sub item that was
*                   entered via the terminal.
*
* return value description:
*   void - none
******************************************************************************/
void vDbg_Menu_Context(int iOption)
{
  int iMenu_Txt_Len = 0;
  char * pMenu_Txt  = NULL;

  if(tDebug_AS.eDbg_In_Context == DEBUG_CONTEXT_NONE)
  {
    vDbg_Interface_Out(cDebug_Context_Home, strlen(cDebug_Context_Home));
    tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_HOME;
  }
  else if(tDebug_AS.eDbg_In_Context == DEBUG_CONTEXT_HOME)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      vDbg_Interface_Out(cDebug_Context_Home, strlen(cDebug_Context_Home));
      tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_HOME;
    }
    else
    {
      switch(iOption)
      {
        case 1: /* Debug Level */
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_LOG_LEVEL;
          break;
        case 2: /* Debug Control */
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_DBG_CTRL;
          break;
        case 3: /* System Control */
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_SYS_CTRL;
          break;
        case 4: /* Application Control */
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_APP_CTRL;
          break;
        case 5: /* Driver Control */
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_DRVR_CTRL;
          break;
        case 0: /* Shut down */
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_SHUTDOWN;
          break;
        default:
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_HOME;
          break;
      }
      //Recursively call vDbg_Menu_Context to print new menu context
      vDbg_Menu_Context(DBG_CONTEXT_MENU_DEFAULT_MENU);
    }
  }
  else if(tDebug_AS.eDbg_In_Context == DEBUG_CONTEXT_LOG_LEVEL)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      iMenu_Txt_Len = strlen(cDebug_Context_Log_Level) + 5;
      pMenu_Txt = calloc(iMenu_Txt_Len, sizeof(char));
      snprintf(pMenu_Txt, iMenu_Txt_Len, cDebug_Context_Log_Level,
              (tDebug_AS.uiLog_Level_En & DBG_LOG_EN_APP)         >> 0,
              (tDebug_AS.uiLog_Level_En & DBG_LOG_EN_DRIVER)      >> 1,
              (tDebug_AS.uiLog_Level_En & DBG_LOG_EN_PLATFORM)    >> 2,
              (tDebug_AS.uiLog_Level_En & DBG_LOG_EN_THIRD_PARTY) >> 3,
              (tDebug_AS.uiLog_Level_En & DBG_LOG_EN_UTILITY)     >> 4);
      vDbg_Interface_Out(pMenu_Txt, strlen(pMenu_Txt));
      free(pMenu_Txt);
      tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_LOG_LEVEL;
    }
    else
    {
      switch(iOption)
      {
        case 1:
          if((tDebug_AS.uiLog_Level_En & DBG_LOG_EN_APP) == DBG_LOG_EN_APP)
          {
            tDebug_AS.uiLog_Level_En ^= DBG_LOG_EN_APP;
          }
          else
          {
            tDebug_AS.uiLog_Level_En |= DBG_LOG_EN_APP;
          }

          break;
        case 2:
          if((tDebug_AS.uiLog_Level_En & DBG_LOG_EN_DRIVER) == DBG_LOG_EN_DRIVER)
          {
            tDebug_AS.uiLog_Level_En ^= DBG_LOG_EN_DRIVER;
          }
          else
          {
            tDebug_AS.uiLog_Level_En |= DBG_LOG_EN_DRIVER;
          }
          break;
        case 3:
          if((tDebug_AS.uiLog_Level_En & DBG_LOG_EN_PLATFORM) == DBG_LOG_EN_PLATFORM)
          {
            tDebug_AS.uiLog_Level_En ^= DBG_LOG_EN_PLATFORM;
          }
          else
          {
            tDebug_AS.uiLog_Level_En |= DBG_LOG_EN_PLATFORM;
          }
          break;
        case 4:
          if((tDebug_AS.uiLog_Level_En & DBG_LOG_EN_THIRD_PARTY) == DBG_LOG_EN_THIRD_PARTY)
          {
            tDebug_AS.uiLog_Level_En ^= DBG_LOG_EN_THIRD_PARTY;
          }
          else
          {
            tDebug_AS.uiLog_Level_En |= DBG_LOG_EN_THIRD_PARTY;
          }
          break;
        case 5:
          if((tDebug_AS.uiLog_Level_En & DBG_LOG_EN_UTILITY) == DBG_LOG_EN_UTILITY)
          {
            tDebug_AS.uiLog_Level_En ^= DBG_LOG_EN_UTILITY;
          }
          else
          {
            tDebug_AS.uiLog_Level_En |= DBG_LOG_EN_UTILITY;
          }
          break;
        case 0:
          //exit the debug level menu and return to the home menu
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_HOME;
          break;
        default:
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_LOG_LEVEL;
          break;
      }

      //Display the updated menu
      vDbg_Menu_Context(DBG_CONTEXT_MENU_DEFAULT_MENU);
    }
  }
  else if(tDebug_AS.eDbg_In_Context == DEBUG_CONTEXT_DBG_CTRL)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      vDbg_Interface_Out(cDebug_Context_Debug_Control, strlen(cDebug_Context_Debug_Control));


      iMenu_Txt_Len = strlen(cDebug_Context_Debug_Control) + 5;
      pMenu_Txt = calloc(iMenu_Txt_Len, sizeof(char));
      snprintf(pMenu_Txt, iMenu_Txt_Len, cDebug_Context_Debug_Control,
              (tDebug_AS.bDebug_Out),
              (tDebug_AS.bDebug_Pin_A_Set),
              (tDebug_AS.bDebug_Pin_B_Set));
      vDbg_Interface_Out(pMenu_Txt, strlen(pMenu_Txt));
      free(pMenu_Txt);
      tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_DBG_CTRL;
    }
    else
    {
      switch(iOption)
      {
        case 1:
          if(tDebug_AS.bDebug_Out == true)
          {
            //todo: print warning/re-activation message
            tDebug_AS.bDebug_Out = false;
          }
          break;
        case 2:
          break;
        case 3:
          break;
        case 4:
          break;
        case 5:
          break;
        case 0:
          break;
        default:
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_DBG_CTRL;
          break;
      }
    }
  }
  else if(tDebug_AS.eDbg_In_Context == DEBUG_CONTEXT_SYS_CTRL)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      vDbg_Interface_Out(cDebug_Context_System_Control, strlen(cDebug_Context_System_Control));
      tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_SYS_CTRL;
    }
    else
    {
      switch(iOption)
      {
        default:
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_SYS_CTRL;
          break;
      }
    }
  }
  else if(tDebug_AS.eDbg_In_Context == DEBUG_CONTEXT_APP_CTRL)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      vDbg_Interface_Out(cDebug_Context_Application_Control, strlen(cDebug_Context_Application_Control));
      tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_APP_CTRL;
    }
    else
    {
      switch(iOption)
      {
        default:
          tDebug_AS.eDbg_In_Context = DEBUG_CONTEXT_APP_CTRL;
          break;
      }
    }
  }
  else if(tDebug_AS.eDbg_In_Context == DEBUG_CONTEXT_DRVR_CTRL)
  {

  }
  else if(tDebug_AS.eDbg_In_Context == DEBUG_CONTEXT_SHUTDOWN)
  {

  }else{/*do nothing*/}

  return;
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
      //Display initial debug message
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
  //Display initial user menu
  vDbg_Menu_Context(DBG_CONTEXT_MENU_DEFAULT_MENU);

  //prepare to receive data from the debug interface
  HAL_UART_Receive_IT(&huart3, (uint8_t *)&cRcv_Char, 1);
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

  return;
}
#endif //__PROJ_DEBUG_C__
#endif //#if (PROJ_CONFIG_USE_UTIL_DEBUG >= 1)
