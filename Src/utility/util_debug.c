//*****************************************************************************
//
// proj_debug.c - project wide debug api
//
// Copyright (c) notice
//
//*****************************************************************************
#ifndef __PROJ_DEBUG_C__
#define __PROJ_DEBUG_C__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//Standard includes
  #include <stdint.h>
  #include <stdbool.h>
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>

//Project specific includes
  #include "proj_inc/project_config.h"

#if (PROJ_CONFIG_USE_UTIL_DEBUG == 0)
  //Debug interface messaging not implemented
#else

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug_config.h"
  #include "utils_inc/util_debug.h"

//Third party includes
  #include "ThirdParty_inc/osal.h"

//Platform includes
  #include "board.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define SYS_HALT_MSG  "****!!!!SYS HALT!!!!****\r\n"
#define SYS_ASSERT_RST_MSG  "****!!!!SYS ASSERT RESET!!!!****\r\n"

#define DBG_CRNL "\r\n"
#define DBG_CRNL_LEN 2
#define DBG_STR_LEN 1024//#define DBG_STR_LEN 256

#define DBG_RCV_BUFF_LEN    128

#define DBG_CONTEXT_MENU_DEFAULT_MENU    0xFF

#define DBG_LOG_EN_NONE           0x0000000
#define DBG_LOG_EN_APP            0x0000001
#define DBG_LOG_EN_DRIVER         0x0000002
#define DBG_LOG_EN_PLATFORM       0x0000004
#define DBG_LOG_EN_THIRD_PARTY    0x0000008
#define DBG_LOG_EN_UTILITY        0x0000010

//Debug menu repost timeouts, used to determine when to repost the menu after
//a debug out
  #define DBG_MENU_REPOST_TIMEOUT_1S  1000
  #define DBG_MENU_REPOST_TIMEOUT_2S  2000
  #define DBG_MENU_REPOST_TIMEOUT_3S  3000
  #define DBG_MENU_REPOST_TIMEOUT_5S  5000
#define DBG_MENU_REPOST_TIMEOUT  DBG_MENU_REPOST_TIMEOUT_3S

//platform specific defines
#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)
  #define UART_HANDLE_T       UART_HandleTypeDef
  #define UART_DEBUG_HANDLE   huart3
  #if (DEBUG_CFG_DEBUG_IN >= 1)
    #define HAL_UART_RxCpltCallback   DBG_INF_IRQ_HANDLER
    #define DBG_INTF_IRQN             USART3_IRQn
  #endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
#elif (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF401RE)
  #define UART_HANDLE_T       UART_HandleTypeDef
  #define UART_DEBUG_HANDLE   huart2
  #if (DEBUG_CFG_DEBUG_IN >= 1)
    ##define HAL_UART_RxCpltCallback   DBG_INF_IRQ_HANDLER
    #define DBG_INTF_IRQN              USART2_IRQn
  #endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
#elif (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF412ZG)
  #define UART_HANDLE_T       UART_HandleTypeDef
  #define UART_DEBUG_HANDLE   huart3
  #if (DEBUG_CFG_DEBUG_IN >= 1)
    #define DBG_INF_IRQ_HANDLER  HAL_UART_RxCpltCallback
    #define DBG_INTF_IRQN        USART3_IRQn
  #endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
#else
  #error "util_debug.c, No platform specific defines!"
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
Debug Control menu                                                        \r\n\
Enter an option from below                                                \r\n\
[0] = disabled, [1] = enabled                                             \r\n\
1)Debug Out[%i]   (select to toggle disable/enable)                       \r\n\
2)Debug In[%i]    (select to toggle disable/enable)                       \r\n\
3)Debug Pin A[%i] (select to toggle disable/enable)                       \r\n\
4)Debug Pin A Set                                                         \r\n\
5)Debug Pin A Clear                                                       \r\n\
6)Debug Pin A Toggle                                                      \r\n\
7)Debug Pin B[%i] (select to toggle disable/enable)                       \r\n\
8)Debug Pin B Set                                                         \r\n\
9)Debug Pin B Clear                                                       \r\n\
10)Debug Pin B Toggle                                                     \r\n\
0)Exit                                                                    \r\n\
->";

char cDebug_Context_System_Control[] = "\r\n\
System Control menu                                                       \r\n\
Enter an option from below                                                \r\n\
[0] = disabled, [1] = enabled                                             \r\n\
1)System Reset                                                            \r\n\
2)Force Halt                                                              \r\n\
3)Interrupts[%i] (select to toggle disable/enable)                        \r\n\
4)System shutdown                                                         \r\n\
0)Exit                                                                    \r\n\
->";

char cDebug_Context_System_Control_Halt_Warn[] = "\r\n\
                  !!!!WARNING!!!!                                         \r\n\
Forcing the system to halt may make it become UNRESPONSIVE.               \r\n\
Are you sure you want the system to halt(Y/N)?                            \r\n\
->";

char cDebug_Context_System_Control_Resume[] = "\r\n\
System in force halt mode               \r\n\
Enter an option from below              \r\n\
1)System Reset                          \r\n\
2)System Resume                         \r\n\
->";

char cDebug_Context_System_Control_Resume_Warn[] = "\r\n\
                  !!!!WARNING!!!!                                         \r\n\
Resuming the system from forced halt may make it UNSTABLE.                \r\n\
It is recommended to perform a restart instead of a resume.               \r\n\
Are you sure you want the system to resume(Y/N)?                          \r\n\
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
2)Wifi[%i]                              \r\n\
3)Wifi Reset                            \r\n\
3)Bluetooth[%i]                         \r\n\
4)Bluetooth Reset                       \r\n\
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

typedef struct Debug_Activity_State_t
{
  bool bIs_Debug_Intf_Setup;
  bool bDebug_Pin_A_Set;
  bool bDebug_Pin_B_Set;
  bool bDebug_Out;
  uint32_t  uiLog_Level_En;

#if (DEBUG_CFG_DEBUG_IN >= 1)
  bool                bDebug_In;
  uint32_t            uiDebug_Repost_Menu_Timeout;
  bool                bDebug_Service_Pending;
  eDEBUG_IN_CONTEXT   eDbg_In_Context;
  char                cDbg_Rcv_Char;
  char                cDbg_Rcv_Buff[DBG_RCV_BUFF_LEN];
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)

}Debug_Activity_State_t;

Debug_Activity_State_t Debug_AS_t =
{
  false, //bool bIs_Debug_Intf_Setup;
  false, //bool bDebug_Pin_A_Set;
  false, //bool bDebug_Pin_B_Set;
  false, //bool bDebug_Out;
  0,     //uint32_t  uiLog_Level_En;
#if (DEBUG_CFG_DEBUG_IN >= 1)
  false, //bool bDebug_In;
  0,     //uint32_t uiDebug_Repost_Menu_Timeout;
  false, //bool bDebug_Service_Pending;
  DEBUG_CONTEXT_NONE, //eDEBUG_IN_CONTEXT eDbg_In_Context;
  0x00, //char cDbg_Rcv_Char;
  //char cDbg_Rcv_Buff[DBG_RCV_BUFF_LEN];
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  },
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
};

//platform specific structures
#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)
  //Debug Interface handles
  extern UART_HANDLE_T UART_DEBUG_HANDLE;
#elif (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF401RE)
  //Debug Interface handles
  extern UART_HANDLE_T UART_DEBUG_HANDLE;
#elif (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF412ZG)
  //Debug Interface handle
  extern UART_HANDLE_T UART_DEBUG_HANDLE;
#else
  #error "util_debug.c, No platform specific structures defined!"
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
//void DBG_INF_IRQ_HANDLER(void);
//void DBG_INF_IRQ_RCV_HANDLER(USART_HANDLE *pHandle);
void vDEBUG_IN(char * pChar);
void vDbg_Menu_Context(int iOption);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
/******************************************************************************
* name: bDbg_Interface_Init
* description:
*   Perform platform specific debug interface initialization.
* parameters:
*   none
* return value description:
*   bool - true: platform debug interface is initialized
*        - false: platform debug interface failed initialization
******************************************************************************/
bool bDbg_Interface_Init(void)
{
  bool bEC = false;

#if ((PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)  | \
     (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF401RE) | \
     (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF412ZG))
  //Debug interface initialized via STM32CubeMX project creator
  bEC = true;
#else
  #error "util_debug.c, bDbg_Interface_Init Platform not defined!"
#endif //PROJ_CONFIG_PLATFORM

  return bEC;
}

/******************************************************************************
* name: vDbg_Interface_Out
* description:
*   Performs platform specific
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
#if ((PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)  | \
     (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF401RE) | \
     (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF412ZG))
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  //check if the debug out interface is
  if(Debug_AS_t.bDebug_Out == true)
  {
    eHAL_Status = HAL_UART_Transmit(&UART_DEBUG_HANDLE, (uint8_t *)cString, uiLen, 3000);

    if(eHAL_Status != HAL_OK)
    {
      while(1){};
    }
  }
#else
  #error "util_debug.c, No debug interface out api defined"
#endif

  return;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: bDbg_Detect_Debugger
* description:
*   Detects if a debug probe/device is attached to the target
* parameters:
*   none
* return value description:
*   bool - true: debugger is present
*        - false: debugger is not present or not detected
******************************************************************************/
bool bDbg_Detect_Debugger(void)
{
  bool bDebugger_Present = false;
#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)
  //todo define api to detect debugger
  __ASM volatile("BKPT 0x00A8");
#elif (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF401RE)
  //todo define api to detect debugger
#elif (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF412ZG)
  //todo define api to detect debugger
#else
  #error "No debuger detect api defined"
#endif
  return bDebugger_Present;
}

#if (DEBUG_CFG_DEBUG_IN >= 1)
/******************************************************************************
* name: DBG_INF_IRQ_HANDLER
* description:
*   Debug uart receive callback function. The function and code in the routine
*   is platform specific. A dev will need to define/add/develop code if an
*   existing platform is not defined.
* parameters:
*   Mandatory parameters:
*     UART_HANDLE_T - * pHandle: Handle of the receive uart.
*   Optional platform specific parameters:
*     void - * pBuff: Pointer to the buffer containing the received data.
*     SIZE_INT - Buff_Len: Length of the received buffer.
*
* return value description:
*   none
******************************************************************************/
#if ((PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)   |\
     (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF401RE)  |\
     (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF412ZG))
void DBG_INF_IRQ_HANDLER(UART_HANDLE_T *pHandle)
{
  if(pHandle == &UART_DEBUG_HANDLE)
  {
    //clear the pending interrupt
    HAL_NVIC_ClearPendingIRQ(DBG_INTF_IRQN);
    //receive the debug
    vDEBUG_IN(&cRcv_Char);
    HAL_UART_Receive_IT(&UART_DEBUG_HANDLE, (uint8_t *)&cRcv_Char, 1);
  }
  return;
}
#else
  #error "DBG_INF_IRQ_HANDLER api for rx callback not defined"
#endif

/******************************************************************************
* name: vDEBUG_IN
* description:
*   Function to be called from the debug input ISR or input callback.
* parameters:
*   char * - pChar: Pointer to the buffer containing the received character
*                   from the debug input.
* return value description:
*   none
******************************************************************************/
void vDEBUG_IN(char * pChar)
{
  static char cRcvd_Char;

  //Check if debug in is enabled
  if(Debug_AS_t.bDebug_In == true)
  {
    //copy the received char to a local buffer
    cRcvd_Char = *pChar;

    //check if the received char is valid
    if((cRcvd_Char == '\r') | (cRcvd_Char <= '9')  |  (cRcvd_Char >= '0'))
    {
      Debug_AS_t.cDbg_Rcv_Char = cRcvd_Char;
      Debug_AS_t.bDebug_Service_Pending = true;
    }
    else
    {
      //received invalid character, do nothing
    }
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

  if(Debug_AS_t.eDbg_In_Context == DEBUG_CONTEXT_NONE)
  {
    vDbg_Interface_Out(cDebug_Context_Home, strlen(cDebug_Context_Home));
    Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_HOME;
  }
  else if(Debug_AS_t.eDbg_In_Context == DEBUG_CONTEXT_HOME)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      vDbg_Interface_Out(cDebug_Context_Home, strlen(cDebug_Context_Home));
      Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_HOME;
    }
    else
    {
      switch(iOption)
      {
        case 1: /* Debug Level */
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_LOG_LEVEL;
          break;
        case 2: /* Debug Control */
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_DBG_CTRL;
          break;
        case 3: /* System Control */
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_SYS_CTRL;
          break;
        case 4: /* Application Control */
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_APP_CTRL;
          break;
        case 5: /* Driver Control */
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_DRVR_CTRL;
          break;
        case 0: /* Shut down */
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_SHUTDOWN;
          break;
        default:
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_HOME;
          break;
      }
      //Recursively call vDbg_Menu_Context to print new menu context
      vDbg_Menu_Context(DBG_CONTEXT_MENU_DEFAULT_MENU);
    }
  }
  else if(Debug_AS_t.eDbg_In_Context == DEBUG_CONTEXT_LOG_LEVEL)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      iMenu_Txt_Len = strlen(cDebug_Context_Log_Level) + 5;
      pMenu_Txt = calloc(iMenu_Txt_Len, sizeof(char));
      snprintf(pMenu_Txt, iMenu_Txt_Len, cDebug_Context_Log_Level,
              (Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_APP)         >> 0,
              (Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_DRIVER)      >> 1,
              (Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_PLATFORM)    >> 2,
              (Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_THIRD_PARTY) >> 3,
              (Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_UTILITY)     >> 4);
      iMenu_Txt_Len = strlen(pMenu_Txt);
      vDbg_Interface_Out(pMenu_Txt, iMenu_Txt_Len);
      free(pMenu_Txt);

      Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_LOG_LEVEL;
    }
    else
    {
      switch(iOption)
      {
        case 1:
          if((Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_APP) == DBG_LOG_EN_APP)
          {
            Debug_AS_t.uiLog_Level_En ^= DBG_LOG_EN_APP;
          }
          else
          {
            Debug_AS_t.uiLog_Level_En |= DBG_LOG_EN_APP;
          }

          break;
        case 2:
          if((Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_DRIVER) == DBG_LOG_EN_DRIVER)
          {
            Debug_AS_t.uiLog_Level_En ^= DBG_LOG_EN_DRIVER;
          }
          else
          {
            Debug_AS_t.uiLog_Level_En |= DBG_LOG_EN_DRIVER;
          }
          break;
        case 3:
          if((Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_PLATFORM) == DBG_LOG_EN_PLATFORM)
          {
            Debug_AS_t.uiLog_Level_En ^= DBG_LOG_EN_PLATFORM;
          }
          else
          {
            Debug_AS_t.uiLog_Level_En |= DBG_LOG_EN_PLATFORM;
          }
          break;
        case 4:
          if((Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_THIRD_PARTY) == DBG_LOG_EN_THIRD_PARTY)
          {
            Debug_AS_t.uiLog_Level_En ^= DBG_LOG_EN_THIRD_PARTY;
          }
          else
          {
            Debug_AS_t.uiLog_Level_En |= DBG_LOG_EN_THIRD_PARTY;
          }
          break;
        case 5:
          if((Debug_AS_t.uiLog_Level_En & DBG_LOG_EN_UTILITY) == DBG_LOG_EN_UTILITY)
          {
            Debug_AS_t.uiLog_Level_En ^= DBG_LOG_EN_UTILITY;
          }
          else
          {
            Debug_AS_t.uiLog_Level_En |= DBG_LOG_EN_UTILITY;
          }
          break;
        case 0:
          //exit the debug level menu and return to the home menu
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_HOME;
          break;
        default:
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_LOG_LEVEL;
          break;
      }

      //Display the updated menu
      vDbg_Menu_Context(DBG_CONTEXT_MENU_DEFAULT_MENU);
    }
  }
  else if(Debug_AS_t.eDbg_In_Context == DEBUG_CONTEXT_DBG_CTRL)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      vDbg_Interface_Out(cDebug_Context_Debug_Control, strlen(cDebug_Context_Debug_Control));


      iMenu_Txt_Len = strlen(cDebug_Context_Debug_Control) + 5;
      pMenu_Txt = calloc(iMenu_Txt_Len, sizeof(char));
      snprintf(pMenu_Txt, iMenu_Txt_Len, cDebug_Context_Debug_Control,
              (Debug_AS_t.bDebug_Out),
              (Debug_AS_t.bDebug_Pin_A_Set),
              (Debug_AS_t.bDebug_Pin_B_Set));
      vDbg_Interface_Out(pMenu_Txt, strlen(pMenu_Txt));
      free(pMenu_Txt);
      Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_DBG_CTRL;
    }
    else
    {
      switch(iOption)
      {
        case 1:
          //1)Debug Out[%i]
          if(Debug_AS_t.bDebug_Out == true)
          {
            //todo: print warning/re-activation message
            Debug_AS_t.bDebug_Out = false;
          }
          break;
        case 2:
          //2)Debug In[%i]
          if(Debug_AS_t.bDebug_In == true)
          {
            //todo: print warning/re-activation message
            Debug_AS_t.bDebug_In = false;
          }
          break;
        case 3:
          //3)Debug Pin A[%i]
          break;
        case 4:
          //4)Debug Pin A Set
          vDEBUG_GPIO_SET_A();
          break;
        case 5:
          //5)Debug Pin A Clear
          vDEBUG_GPIO_CLR_A();
          break;
        case 6:
          //6)Debug Pin A Toggle
          vDEBUG_GPIO_TOGGLE_A();
          break;
        case 7:
          //7)Debug Pin B[%i]
          break;
        case 8:
          //8)Debug Pin B Set
          vDEBUG_GPIO_SET_B();
          break;
        case 9:
          //9)Debug Pin B Clear
          vDEBUG_GPIO_CLR_B();
          break;
        case 10:
          //10)Debug Pin B Toggle
          vDEBUG_GPIO_TOGGLE_B();
          break;
        case 0:
          //exit the debug control menu and return to the home menu
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_HOME;
          break;
          break;
        default:
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_DBG_CTRL;
          break;
      }
    }
  }
  else if(Debug_AS_t.eDbg_In_Context == DEBUG_CONTEXT_SYS_CTRL)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      vDbg_Interface_Out(cDebug_Context_System_Control, strlen(cDebug_Context_System_Control));
      Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_SYS_CTRL;
    }
    else
    {
      switch(iOption)
      {
        default:
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_SYS_CTRL;
          break;
      }
    }
  }
  else if(Debug_AS_t.eDbg_In_Context == DEBUG_CONTEXT_APP_CTRL)
  {
    if(iOption == DBG_CONTEXT_MENU_DEFAULT_MENU)
    {
      vDbg_Interface_Out(cDebug_Context_Application_Control, strlen(cDebug_Context_Application_Control));
      Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_APP_CTRL;
    }
    else
    {
      switch(iOption)
      {
        default:
          Debug_AS_t.eDbg_In_Context = DEBUG_CONTEXT_APP_CTRL;
          break;
      }
    }
  }
  else if(Debug_AS_t.eDbg_In_Context == DEBUG_CONTEXT_DRVR_CTRL)
  {

  }
  else if(Debug_AS_t.eDbg_In_Context == DEBUG_CONTEXT_SHUTDOWN)
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

  if(Debug_AS_t.bIs_Debug_Intf_Setup == true)
  {

#if (DEBUG_CFG_DEBUG_IN >= 1)
    if(Debug_AS_t.bDebug_In == true)
    {
      //check if a \r\n is needed by checking if a message was sent out by checking if the timeout is 0
      if(Debug_AS_t.uiDebug_Repost_Menu_Timeout == 0)
      {
        vDbg_Interface_Out(DBG_CRNL, DBG_CRNL_LEN);
      }
      Debug_AS_t.uiDebug_Repost_Menu_Timeout = DBG_MENU_REPOST_TIMEOUT;
    }
#endif

    if((uiLen + 1) < DBG_STR_LEN)
    {
      memset(cDBG_String, 0x00, (uiLen + 1));
      memcpy(cDBG_String, cMsg, uiLen);

      vDbg_Interface_Out(cDBG_String, uiLen);
      vDbg_Interface_Out(DBG_CRNL, DBG_CRNL_LEN);
    }
  }
  return;
}
#endif //#if(DEBUG_CFG_DEBUG_OUT >= 1)

#if (DEBUG_CFG_DEBUG_IN >= 1)
/******************************************************************************
* name: vDEBUG_SERVICE
* description:
*   Routine to be called during a regular service routine at intervals of 1
*   Millisecond. (IE From a low priority/idle task or a super loop). This
*   function will handle any pending, non-critical debug input, output or
*   action. In order for some enabled debug features to work properly this
*   function should not be considered optional.
* parameters:
*   none
* return value description:
*   none
******************************************************************************/
void vDEBUG_SERVICE(void)
{
  int iLen = 0;
  int iMenu = 0;

  //Check if a debug service is pending
  if(Debug_AS_t.bDebug_Service_Pending == true)
  {
    if(Debug_AS_t.bDebug_In == true)
    {
      if(Debug_AS_t.cDbg_Rcv_Char == '\r')
      {
        iLen = strlen(Debug_AS_t.cDbg_Rcv_Buff);

        //Check received buff len
        //
        if((iLen == 0) | (iLen > 2))
        {
          //Length is an invalid value, erase the receive buffer and re-display the current context menu
          memset(Debug_AS_t.cDbg_Rcv_Buff, 0x00, DBG_RCV_BUFF_LEN);
          vDbg_Menu_Context(DBG_CONTEXT_MENU_DEFAULT_MENU);
        }
        else if((iLen == 1) | (iLen == 2))
        {
          //Length is a valid value, display the correct context menu
          iMenu = atoi(Debug_AS_t.cDbg_Rcv_Buff);
          vDbg_Menu_Context(iMenu);

          //erase the receive buffer
          memset(Debug_AS_t.cDbg_Rcv_Buff, 0x00, DBG_RCV_BUFF_LEN);
        }else{/*do nothing*/}
      }
      else if(('9' >= Debug_AS_t.cDbg_Rcv_Char) & (Debug_AS_t.cDbg_Rcv_Char >= '0'))
      {
        memcpy(Debug_AS_t.cDbg_Rcv_Buff, &Debug_AS_t.cDbg_Rcv_Buff[1], DBG_RCV_BUFF_LEN);
        Debug_AS_t.cDbg_Rcv_Buff[0] = Debug_AS_t.cDbg_Rcv_Char;

        //echo character out
        vDbg_Interface_Out(&Debug_AS_t.cDbg_Rcv_Char, 1);

        //reset the receive char buffer.
        Debug_AS_t.cDbg_Rcv_Char = 0;
      }
      else
      {
        //received invalid character, do nothing
      }
    }else{/*do nothing*/}

    //Reset the repost timeout control variable
    Debug_AS_t.uiDebug_Repost_Menu_Timeout = 0;

    //Reset the debug service timeout control variable
    Debug_AS_t.bDebug_Service_Pending = false;
  }
  else
  {
    if(Debug_AS_t.uiDebug_Repost_Menu_Timeout > 0)
    {
      Debug_AS_t.uiDebug_Repost_Menu_Timeout--;
      if(Debug_AS_t.uiDebug_Repost_Menu_Timeout == 0)
      {
        Debug_AS_t.bDebug_Service_Pending = true;
        Debug_AS_t.cDbg_Rcv_Char = '\r';
      }
    }
  }
  return;
}

#if  (DEBUG_CFG_LOG_LEVEL >= 1)
//*****************************************************************************
// name: vDEBUG_LVL
// description: sends a message out the debug message interface according to
//              enabled log levels.
// param description: none
// return value description: none
//*****************************************************************************
void vDEBUG_LVL(eDEBUG_LOG_LEVEL eLog_Lvl, char * cMsg, ...)
{
  //todo: implement
  return;
}
#endif //(DEBUG_CFG_LOG_LEVEL >= 1)
#endif  //(DEBUG_CFG_DEBUG_IN >= 1)

#if (DEBUG_CFG_ASSERT  >= 1)
/******************************************************************************
* name: vDEBUG_ASSERT
* description:
*   performs an assert operation based on the parameters passed in. If the
*   assert value is true the assert will pass. If the assert is false the
*   assert will fail, the system will halt and the message will be posed to
*   the debug interface.
* parameters:
*   char * cMsg - : Pointer to the message to post if the assert fails
*   int iAssert - : assert value to check. If >= 1 the assert will pass. If
*                   < 1 the assert will fail.
* return value description:
*   void - none
******************************************************************************/
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
* description: initializes the debug message interface and the debug gpio pins.
*   This function must be called during system initialization in order for
*   debug messaging and features to work.
* param description: none
* return value description: none
******************************************************************************/
void vDEBUG_init(void)
{
  bool bEC = false;
  char * pStartup_Msg = NULL;
  int iStartup_Msg_Size = 0;

  if(Debug_AS_t.bIs_Debug_Intf_Setup == false)
  {
    bEC = bDbg_Interface_Init();
    if(bEC == true)
    {
      //Set debug interface activity state control variable
      Debug_AS_t.bIs_Debug_Intf_Setup = true;

      //Check if debug out is enabled
      if(DEBUG_CFG_DEBUG_OUT <= 1)
      {
        //Set debug interface out activity state control variable
        Debug_AS_t.bDebug_Out = true;

        //Display initial debug message
        iStartup_Msg_Size = strlen(cDebug_Startup_Msg) + strlen(DEBUG_PROJECT_NAME) + DEBUG_FW_VERSION_LEN + 1; //start up msg len + project name len + firmware ver len + null char
        pStartup_Msg = calloc(iStartup_Msg_Size, sizeof(char));
        snprintf(pStartup_Msg, iStartup_Msg_Size, cDebug_Startup_Msg, DEBUG_PROJECT_NAME, DEBUG_VERSION_MAJOR, DEBUG_VERSION_MINOR, DEBUG_VERSION_PATCH);
        vDEBUG(pStartup_Msg);
        free(pStartup_Msg);
      }
    }else{/*do nothing*/}

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
    //Set debug interface in activity state control variable
    Debug_AS_t.bDebug_In = true;

    //Display initial user menu
    vDbg_Menu_Context(DBG_CONTEXT_MENU_DEFAULT_MENU);

    //prepare to receive data from the debug interface
#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)
    HAL_UART_Receive_IT(&UART_DEBUG_HANDLE, (uint8_t *)&cRcv_Char, 1);
#elif (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF401RE)
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&cRcv_Char, 1);
#elif (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF412ZG)
    HAL_UART_Receive_IT(&UART_DEBUG_HANDLE, (uint8_t *)&cRcv_Char, 1);
#else
    #error "No debug interface interrupt start defined"
#endif  //PROJ_CONFIG_PLATFORM
#endif //#if (DEBUG_CFG_DEBUG_IN >= 1)
  }

  return;
}
#endif //__PROJ_DEBUG_C__
#endif //#if (PROJ_CONFIG_USE_UTIL_DEBUG == 0)
