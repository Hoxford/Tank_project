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
#include <string.h>

#include "utils_inc/error_codes.h"
#include "utils_inc/osal.h"
#include "utils_inc/proj_debug.h"

#include "board.h"

//STM32F4 specific includes
#include "stm32f4xx.h"
#include "core_cm4.h"


/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//wifi defines
#define WIFI_TX_BUF_LEN  512
#define WIFI_RX_BUF_LEN  512

//flash defines
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_11   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_11 + 0x20000   /* End @ of user Flash area */

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

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

//USART_HandleTypeDef tWifi_UART_Handle;
UART_HandleTypeDef  tWifi_UART_Handle;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//extern void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);

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
//    tWifi_GPIO_Init.Pin       = GPIO_PIN_10 | GPIO_PIN_9;
    tWifi_GPIO_Init.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
    tWifi_GPIO_Init.Mode      = GPIO_MODE_AF_PP;//GPIO_MODE_AF_OD;//
    tWifi_GPIO_Init.Pull      = GPIO_NOPULL;//GPIO_PULLDOWN;//GPIO_PULLUP;//
    tWifi_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
//    tWifi_GPIO_Init.Alternate = GPIO_AF7_USART1;
    tWifi_GPIO_Init.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA,&tWifi_GPIO_Init);

//    tWifi_UART_Handle.Init.BaudRate    = 115200;//57600;
//    tWifi_UART_Handle.Init.WordLength  = USART_WORDLENGTH_8B;
//    tWifi_UART_Handle.Init.StopBits    = USART_STOPBITS_1;
//    tWifi_UART_Handle.Init.Parity      = USART_PARITY_NONE;
//    tWifi_UART_Handle.Init.Mode        = USART_MODE_TX_RX;
//    tWifi_UART_Handle.Init.CLKPolarity = USART_POLARITY_LOW;//USART_POLARITY_HIGH;//
//    tWifi_UART_Handle.Init.CLKPhase    = USART_PHASE_1EDGE;//USART_PHASE_2EDGE;//
//    tWifi_UART_Handle.Init.CLKLastBit  = USART_LASTBIT_DISABLE;//USART_LASTBIT_ENABLE;//
//    tWifi_UART_Handle.Instance         = USART2;
//    eHAL_Status = HAL_USART_Init(&tWifi_UART_Handle);

    tWifi_UART_Handle.Init.BaudRate     = 115200;//57600;
    tWifi_UART_Handle.Init.WordLength   = UART_WORDLENGTH_8B;
    tWifi_UART_Handle.Init.StopBits     = UART_STOPBITS_1;
    tWifi_UART_Handle.Init.Parity       = UART_PARITY_NONE;
    tWifi_UART_Handle.Init.Mode         = UART_MODE_TX_RX;
    tWifi_UART_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    tWifi_UART_Handle.Init.OverSampling = UART_OVERSAMPLING_8;
    tWifi_UART_Handle.Instance          = USART2;
    eHAL_Status = HAL_UART_Init(&tWifi_UART_Handle);

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

    tWifi_GPIO_Init.Pin    = GPIO_PIN_0;
    tWifi_GPIO_Init.Mode   = GPIO_MODE_OUTPUT_PP;
    tWifi_GPIO_Init.Pull   = GPIO_NOPULL;
    tWifi_GPIO_Init.Speed  = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA,&tWifi_GPIO_Init);

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

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Rst_Clr(void)
{
  ERROR_CODE eEC = ER_OK;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

  return eEC;
}

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Rst_Set(void)
{
  ERROR_CODE eEC = ER_OK;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

  return eEC;
}

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Rst(void)
{
  ERROR_CODE eEC = ER_OK;

  eBSP_Wifi_Rst_Set();

  eBSP_Wifi_Rst_Clr();

  return eEC;
}

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Send(tBSP_tWifi_Transmit * pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

//  eHAL_Status = HAL_USART_Transmit(&tWifi_UART_Handle, pParam->pBuff, pParam->uiBuff_Len, 3000);
  eHAL_Status = HAL_UART_Transmit(&tWifi_UART_Handle, pParam->pBuff, pParam->uiBuff_Len, 3000);

//  vDEBUG((char *)pParam->pBuff);

  if(eHAL_Status == HAL_OK)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Receive(tBSP_Wifi_Receive * pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

//  eHAL_Status = HAL_USART_Receive(&tWifi_UART_Handle, pParam->pBuff, pParam->uiBuff_Len, 3000);
  eHAL_Status = HAL_UART_Receive(&tWifi_UART_Handle, pParam->pBuff, pParam->uiBuff_Len, 100);
  if(eHAL_Status == HAL_OK)
  {
    eEC = ER_OK;
  }
  else if(eHAL_Status == HAL_TIMEOUT)
  {
    eEC = ER_TIMEOUT;
  }


  return eEC;
}

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_FLASH_READ(tBSP_Flash_Read * pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  uint32_t uiReadAddress = pParam->uiStart_Addr;
  uint32_t uiIndex = 0;

  while(uiIndex < pParam->uiBuff_Len)
  {
    pParam->pBuff[uiIndex++] = *(__IO uint32_t*)uiReadAddress++;//FLASH_SETTINGS_INDEX;//pParam->uiStart_Addr[uiIndex];
  }

  eEC = ER_OK;

  return eEC;
}

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_FLASH_WRITE(tBSP_Flash_Write * pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_EC = HAL_ERROR;
  uint32_t uiStartSector;
  uint32_t uiEndSector;
  uint32_t uiSectorIndex;
  uint8_t  uiByteOffset = 0;

  /* Clear pending flags (if any) */
//  HAL_FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
//                      FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

  uiStartSector = ADDR_FLASH_SECTOR_11;

  if(pParam->uiBuff_Len | 0x00000003)
  {
    uiByteOffset = pParam->uiBuff_Len & 0x00000003;
  }

  uiSectorIndex = (pParam->uiBuff_Len)/4;

  uiEndSector = uiStartSector + uiSectorIndex;
  if(uiByteOffset > 0)
  {
    uiEndSector += 1;
  }

  uiSectorIndex = uiStartSector;
  while(uiSectorIndex < uiEndSector)
  {
    eHAL_EC = HAL_FLASH_Program(TYPEPROGRAM_WORD, uiStartSector, *pParam->pBuff);
    if(eHAL_EC != HAL_OK)
    {
      break;
    }
//    FLASH_Program_Word(uiSectorIndex, pParam->pBuff);
    pParam->pBuff += 4;
    uiSectorIndex += 1;
  }
//  FLASH_Program_Byte(ADDR_FLASH_SECTOR_11, uint32_t Data);

  return eEC;
}

/******************************************************************************
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Inc_ms_count(void)
{
  ERROR_CODE eEC = ER_OK;



  return eEC;
}

ERROR_CODE eBSP_Get_Current_ms_count(uint32_t * uiSystem_total_ms_count)
{
  ERROR_CODE eEC = ER_OK;

  *uiSystem_total_ms_count = HAL_GetTick();

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


  SystemCoreClock = HAL_RCC_GetHCLKFreq();

  SysTick_Config (SystemCoreClock / 1000);

  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __USART2_CLK_ENABLE();
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
