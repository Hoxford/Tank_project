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

#define SYS_HALT_MSG  "****!!!!SYS HALT!!!!****\r\n"
#define SYS_ASSERT_RST_MSG  "****!!!!SYS ASSERT RESET!!!!****\r\n"

#define DBG_CRNL "\r\n"
#define DBG_CRNL_LEN 2
#define DBG_STR_LEN 64

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

char cDBG_String[DBG_STR_LEN];

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct tDebut_Activity_State
{
  bool bIs_Debug_UART_Setup;
}tDebug_Activity_State;

tDebug_Activity_State tDebug_AS =
{
  false, //bool bIs_Debug_UART_Setup;
};

USART_HandleTypeDef * ptDebug_UART_Handle = NULL;
USART_HandleTypeDef tDebug_UART_Handle;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

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

  //    eHAL_Status = HAL_USART_Transmit(ptDebug_UART_Handle, (uint8_t *)cDBG_String, uiLen, 3000);
      eHAL_Status = HAL_USART_Transmit(&tDebug_UART_Handle, (uint8_t *)cDBG_String, uiLen, 3000);

      memset(cDBG_String, 0x00, (DBG_CRNL_LEN + 1));
      memcpy(cDBG_String, DBG_CRNL, DBG_CRNL_LEN);
      eHAL_Status = HAL_USART_Transmit(&tDebug_UART_Handle, (uint8_t *)cDBG_String, DBG_CRNL_LEN, 3000);

      if(eHAL_Status != HAL_OK)
      {
        while(1){};
      }

    }
  }
  return;
}

void vDEBUG_ASSERT(char * cMsg,int iAssert)
{
//  ERROR_CODE eEC = ER_FAIL;

  if(iAssert)
  {
    return;
  }
  else
  {
    //todo detct debugger eEC = eBSP_debugger_detect();
//    if(eEC == ER_OK)
//    {
//      vDEBUG(SYS_HALT_MSG);
//      vDEBUG(cMsg);
//
//      //todo: disable interrupts
//      //todo: system halt
//      while(1){};
//    }
//    else
//    {
//      vDEBUG(SYS_HALT_MSG);
//      vDEBUG(cMsg);
//
//      //todo: disable interrupts
//      //todo: system reset
//      while(1){};
//    }
    __ASM volatile("BKPT 0x00A8");
    while (1){}0
  }
}

/******************************************************************************
* name: vDEBUG_GPIO_SET_1
* description: sets high DEBUG_GPIO_PIN_1
* param description:
* return value description:
******************************************************************************/
void vDEBUG_GPIO_SET_1(void)
{
  //todo: set gpio pin
  return;
}

/******************************************************************************
* name: vDEBUG_GPIO_CLR_1
* description: sets low DEBUG_GPIO_PIN_1
* param description:
* return value description:
******************************************************************************/
void vDEBUG_GPIO_CLR_1(void)
{
  //todo: clear pin
  return;
}

/******************************************************************************
* name: vDEBUG_GPIO_TOGGLE_1
* description: toggles DEBUG_GPIO_PIN_1
* param description:
* return value description:
******************************************************************************/
void vDEBUG_GPIO_TOGGLE_1(void)
{
  //todo: toggle pin

  return;
}

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
