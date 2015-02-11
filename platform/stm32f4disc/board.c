/******************************************************************************
*
* board.c - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __BOARD_C__
#define __BOARD_C__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils_inc/error_codes.h"
#include "utils_inc/osal.h"
#include "utils_inc/proj_debug.h"

#include "board.h"

//STM32F4 specific includes

#include "stm32f4xx.h"
//#include "stm32f4xx_hal.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define WIFI_TX_BUF_LEN  512
#define WIFI_RX_BUF_LEN  512


/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

uint8_t uiWifi_TX_Buff[WIFI_TX_BUF_LEN];
uint8_t uiWifi_RX_Buff[WIFI_RX_BUF_LEN];

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct tBSP_Activity_State
{
  bool bIs_Camera_Intf_Init;

  bool bIs_Wifi_Intf_Init;
}tBSP_Activity_State;

tBSP_Activity_State tBSP_AS =
{
  false,  //bool bIs_Camera_Intf_Init;

  false,  //bool bIs_Wifi_Intf_Init;
};

USART_HandleTypeDef tWifi_UART_Handle;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eBSP_Camera_Intf_Init(void);
ERROR_CODE eBSP_Wifi_Intf_Init(void);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eBSP_Camera_Intf_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;
  GPIO_InitTypeDef      tCamera_GPIO_Init;

  if(tBSP_AS.bIs_Camera_Intf_Init == false)
  {
    //camera gpio init
    //
    //Config camera reset and power down pins to GPIO
    tCamera_GPIO_Init.Pin   = GPIO_PIN_4 | GPIO_PIN_5;
    tCamera_GPIO_Init.Mode  = GPIO_MODE_OUTPUT_PP;
    tCamera_GPIO_Init.Pull  = GPIO_NOPULL;
    tCamera_GPIO_Init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB,&tCamera_GPIO_Init);

    //config camera control interface pins to I2C
    tCamera_GPIO_Init.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    tCamera_GPIO_Init.Mode      = GPIO_MODE_AF_PP;
    tCamera_GPIO_Init.Pull      = GPIO_NOPULL;
    tCamera_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
    tCamera_GPIO_Init.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB,&tCamera_GPIO_Init);

    //config camera pixel(PCLK) and system (XCLK) pins to output timers
    tCamera_GPIO_Init.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
    tCamera_GPIO_Init.Mode      = GPIO_MODE_AF_PP;
    tCamera_GPIO_Init.Pull      = GPIO_NOPULL;
    tCamera_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
    tCamera_GPIO_Init.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOB,&tCamera_GPIO_Init);

    eEC = ER_OK;

    tBSP_AS.bIs_Camera_Intf_Init = true;
  }
  else
  {
    eEC = ER_OK;
  }

  return eEC;
}

ERROR_CODE eBSP_Wifi_Intf_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;
  GPIO_InitTypeDef      tWifi_GPIO_Init;

  if(tBSP_AS.bIs_Wifi_Intf_Init == false)
  {
    tWifi_GPIO_Init.Pin       = GPIO_PIN_10 | GPIO_PIN_9;
    tWifi_GPIO_Init.Mode      = GPIO_MODE_OUTPUT_PP;
    tWifi_GPIO_Init.Pull      = GPIO_NOPULL;
    tWifi_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
    tWifi_GPIO_Init.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA,&tWifi_GPIO_Init);

    tWifi_UART_Handle.Init.BaudRate = 115200;
    tWifi_UART_Handle.Init.WordLength = USART_WORDLENGTH_8B;
    tWifi_UART_Handle.Init.StopBits = USART_STOPBITS_1;
    tWifi_UART_Handle.Init.Parity = USART_PARITY_NONE;
    tWifi_UART_Handle.Init.Mode = USART_MODE_TX;
    tWifi_UART_Handle.Init.CLKPolarity = USART_POLARITY_LOW;
    tWifi_UART_Handle.Init.CLKPhase = USART_PHASE_1EDGE;
    tWifi_UART_Handle.Init.CLKLastBit = USART_LASTBIT_DISABLE;
    tWifi_UART_Handle.Instance = USART1;
    tWifi_UART_Handle.pTxBuffPtr = uiWifi_TX_Buff;
    tWifi_UART_Handle.pRxBuffPtr = uiWifi_RX_Buff;

    eHAL_Status = HAL_USART_Init(&tWifi_UART_Handle);
    if(eHAL_Status == HAL_OK)
    {
      eEC = ER_OK;
      tBSP_AS.bIs_Wifi_Intf_Init = true;
    }
    else
    {
      eEC = ER_FAIL;
      tBSP_AS.bIs_Wifi_Intf_Init = false;
    }
  }
  else
  {
    eEC = ER_OK;
  }
  return eEC;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eBSP_Wifi_Transmit(tWifi_Transmit * pParam)
{
  ERROR_CODE eEC = ER_FAIL;

  return eEC;
}

/******************************************************************************
* name: eBSP_Board_Init
* description:
* param description: void
* return value description: ERROR_CODE - ER_OK: board init good
*                                        ER_FAIL: board init fail
******************************************************************************/
ERROR_CODE eBSP_Board_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;

  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __USART1_CLK_ENABLE();
  __I2C1_CLK_ENABLE();
  __TIM4_CLK_ENABLE();

  eEC = eBSP_Camera_Intf_Init();

  if(eEC == ER_OK)
  {
    eEC = eBSP_Wifi_Intf_Init();
  }

  return eEC;
}

#endif //__FILE_NAME_C__
