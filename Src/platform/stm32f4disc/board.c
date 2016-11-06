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
#include "ThirdParty_inc/osal.h"
#include "utils_inc/util_debug.h"

#include "board.h"

//STM32F4 specific includes
//#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "core_cm4.h"


/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//camera defines
#define CAMERA_TIMER_INSTANCE  TIM4
#define CAMERA_I2C_INSTANCE    I2C2
#define I2C_ADDRESS            0x30F

//wifi defines
	#define WIFI_TX_BUF_LEN       512
	#define WIFI_RX_BUF_LEN       512
  #define WIFI_UART_HANDLE      huart2
  #define WIFI_UART_HANDLE_T    UART_HandleTypeDef
  #define WIFI_INF_IRQ_HANDLER  HAL_UART_RxCpltCallback
  #define WIFI_INTF_IRQN        USART2_IRQn

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

//wifi variables
//
uint8_t uiWifi_TX_Buff[WIFI_TX_BUF_LEN];
uint8_t uiWifi_RX_Buff[WIFI_RX_BUF_LEN];

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
extern I2C_HandleTypeDef hi2c2;

extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart2;
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
  bool bIs_Usb_Intf_Init;
  void (* vUART_ISR[5])(void);
}tBSP_Activity_State;

tBSP_Activity_State tBSP_AS =
{
  false,  //bool bIs_Camera_Intf_Init;
  false,  //bool bIs_Wifi_Intf_Init;
  false,  //bool bIs_Usb_Intf_Init;
  NULL, NULL, NULL, NULL, NULL,
};

//USART_HandleTypeDef tWifi_UART_Handle;
//UART_HandleTypeDef  tWifi_UART_Handle;
//TIM_HandleTypeDef   tCamera_Timer_Handle;
//I2C_HandleTypeDef   tCamera_I2C_Handle;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//extern void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eBSP_Camera_Intf_Init(void);
ERROR_CODE eBSP_Wifi_Intf_Init(void);
ERROR_CODE eBSP_Usb_Intf_Init(void);
ERROR_CODE eBSP_SystemClock_Config(void);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Camera_Intf_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;
//  GPIO_InitTypeDef      tCamera_GPIO_Init;
//  uint32_t uwPrescalerValue;
//
//  if(tBSP_AS.bIs_Camera_Intf_Init == false)
//  {
//    //camera gpio init
//    //
//    //Config camera reset and power down pins to GPIO
//    __GPIOB_CLK_ENABLE();
//    tCamera_GPIO_Init.Pin   = GPIO_PIN_4 | GPIO_PIN_5;
//    tCamera_GPIO_Init.Mode  = GPIO_MODE_OUTPUT_PP;
//    tCamera_GPIO_Init.Pull  = GPIO_NOPULL;
//    tCamera_GPIO_Init.Speed = GPIO_SPEED_HIGH;
//    HAL_GPIO_Init(GPIOB,&tCamera_GPIO_Init);
//
//    //config camera control interface pins to I2C
//    __GPIOB_CLK_ENABLE();
//    tCamera_GPIO_Init.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
//    tCamera_GPIO_Init.Mode      = GPIO_MODE_AF_PP;
//    tCamera_GPIO_Init.Pull      = GPIO_NOPULL;
//    tCamera_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
//    tCamera_GPIO_Init.Alternate = GPIO_AF4_I2C2;
//    HAL_GPIO_Init(GPIOB,&tCamera_GPIO_Init);
//    __I2C2_CLK_ENABLE();
//
//    //I2C init
//    tCamera_I2C_Handle.Instance             = CAMERA_I2C_INSTANCE;
//    tCamera_I2C_Handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
//    tCamera_I2C_Handle.Init.ClockSpeed      = 400000;
//    tCamera_I2C_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//    tCamera_I2C_Handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
//    tCamera_I2C_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//    tCamera_I2C_Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
//    tCamera_I2C_Handle.Init.OwnAddress1     = I2C_ADDRESS;
//    tCamera_I2C_Handle.Init.OwnAddress2     = 0;
//
//    if(HAL_I2C_Init(&tCamera_I2C_Handle) != HAL_OK)
//    {
//      /* Initialization Error */
//      eEC = ER_FAIL;
//    }
//
//    //config camera pixel(PCLK) and system (XCLK) pins to output timers
//    __GPIOD_CLK_ENABLE();
//    tCamera_GPIO_Init.Pin       = GPIO_PIN_14 | GPIO_PIN_15;
//    tCamera_GPIO_Init.Mode      = GPIO_MODE_AF_PP;
//    tCamera_GPIO_Init.Pull      = GPIO_NOPULL;
//    tCamera_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
//    tCamera_GPIO_Init.Alternate = GPIO_AF2_TIM4;
//    HAL_GPIO_Init(GPIOD,&tCamera_GPIO_Init);
//    __TIM4_CLK_ENABLE();
//
//    //Timer init
//    /* Compute the prescaler value to have TIM4 counter clock equal to 10 MHz */
//    uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000000) - 1;
//
//    /* Set TIMx instance */
//    tCamera_Timer_Handle.Instance = CAMERA_TIMER_INSTANCE;
//
//    /* Initialize TIM4 peripheral as follow:
//         + Period = uwPrescalerValue/2
//         + Prescaler = ((SystemCoreClock/2)/10000000) - 1
//         + ClockDivision = 0
//         + Counter direction = Up
//    */
//    tCamera_Timer_Handle.Init.Period = uwPrescalerValue/2;
//    tCamera_Timer_Handle.Init.Prescaler = uwPrescalerValue;
//    tCamera_Timer_Handle.Init.ClockDivision = 0;
//    tCamera_Timer_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
//    if(HAL_TIM_Base_Init(&tCamera_Timer_Handle) != HAL_OK)
//    {
//      eEC = ER_FAIL;
//    }
//
//    HAL_TIM_Base_Start(&tCamera_Timer_Handle);
//
//    //config Digital Camera Interface pins
//    __GPIOC_CLK_ENABLE();
//                                  //DCMI 0, 1, 2, 3
//    tCamera_GPIO_Init.Pin       = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
//    tCamera_GPIO_Init.Mode      = GPIO_MODE_INPUT;
//    tCamera_GPIO_Init.Pull      = GPIO_NOPULL;
//    tCamera_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
//    tCamera_GPIO_Init.Alternate = 0;
//    HAL_GPIO_Init(GPIOC, &tCamera_GPIO_Init);
//                                  //DCMI 4, 7
//    __GPIOE_CLK_ENABLE();
//    tCamera_GPIO_Init.Pin       = GPIO_PIN_4 | GPIO_PIN_7;
//    tCamera_GPIO_Init.Mode      = GPIO_MODE_INPUT;
//    tCamera_GPIO_Init.Pull      = GPIO_NOPULL;
//    tCamera_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
//    tCamera_GPIO_Init.Alternate = 0;
//    HAL_GPIO_Init(GPIOE, &tCamera_GPIO_Init);
//                                  //DCMI 5, 6
//    __GPIOB_CLK_ENABLE();
//    tCamera_GPIO_Init.Pin       = GPIO_PIN_6 | GPIO_PIN_8;
//    tCamera_GPIO_Init.Mode      = GPIO_MODE_INPUT;
//    tCamera_GPIO_Init.Pull      = GPIO_NOPULL;
//    tCamera_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
//    tCamera_GPIO_Init.Alternate = 0;
//    HAL_GPIO_Init(GPIOB, &tCamera_GPIO_Init);
//
//    //config HREF horizontal timing VSYNC vertical timing pins
//    __GPIOE_CLK_ENABLE();
//    tCamera_GPIO_Init.Pin       = GPIO_PIN_12 | GPIO_PIN_13;
//    tCamera_GPIO_Init.Mode      = GPIO_MODE_IT_RISING;
//    tCamera_GPIO_Init.Pull      = GPIO_NOPULL;
//    tCamera_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
//    tCamera_GPIO_Init.Alternate = 0;
//    HAL_GPIO_Init(GPIOE, &tCamera_GPIO_Init);
//
//    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0x0F, 0);
//    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
//
//    eEC = ER_OK;
//
//    tBSP_AS.bIs_Camera_Intf_Init = true;
//  }
//  else
//  {
//    eEC = ER_OK;
//  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;
//  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;
//  GPIO_InitTypeDef      tWifi_GPIO_Init;

  if(tBSP_AS.bIs_Wifi_Intf_Init == false)
  {
////    tWifi_GPIO_Init.Pin       = GPIO_PIN_10 | GPIO_PIN_9;
//    tWifi_GPIO_Init.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
//    tWifi_GPIO_Init.Mode      = GPIO_MODE_AF_PP;//GPIO_MODE_AF_OD;//
//    tWifi_GPIO_Init.Pull      = GPIO_NOPULL;//GPIO_PULLDOWN;//GPIO_PULLUP;//
//    tWifi_GPIO_Init.Speed     = GPIO_SPEED_HIGH;
////    tWifi_GPIO_Init.Alternate = GPIO_AF7_USART1;
//    tWifi_GPIO_Init.Alternate = GPIO_AF7_USART2;
//    HAL_GPIO_Init(GPIOA,&tWifi_GPIO_Init);
//
////    tWifi_UART_Handle.Init.BaudRate    = 115200;//57600;
////    tWifi_UART_Handle.Init.WordLength  = USART_WORDLENGTH_8B;
////    tWifi_UART_Handle.Init.StopBits    = USART_STOPBITS_1;
////    tWifi_UART_Handle.Init.Parity      = USART_PARITY_NONE;
////    tWifi_UART_Handle.Init.Mode        = USART_MODE_TX_RX;
////    tWifi_UART_Handle.Init.CLKPolarity = USART_POLARITY_LOW;//USART_POLARITY_HIGH;//
////    tWifi_UART_Handle.Init.CLKPhase    = USART_PHASE_1EDGE;//USART_PHASE_2EDGE;//
////    tWifi_UART_Handle.Init.CLKLastBit  = USART_LASTBIT_DISABLE;//USART_LASTBIT_ENABLE;//
////    tWifi_UART_Handle.Instance         = USART2;
////    eHAL_Status = HAL_USART_Init(&tWifi_UART_Handle);
//
//    tWifi_UART_Handle.Init.BaudRate     = 115200;//57600;
//    tWifi_UART_Handle.Init.WordLength   = UART_WORDLENGTH_8B;
//    tWifi_UART_Handle.Init.StopBits     = UART_STOPBITS_1;
//    tWifi_UART_Handle.Init.Parity       = UART_PARITY_NONE;
//    tWifi_UART_Handle.Init.Mode         = UART_MODE_TX_RX;
//    tWifi_UART_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
//    tWifi_UART_Handle.Init.OverSampling = UART_OVERSAMPLING_8;
//    tWifi_UART_Handle.Instance          = USART2;
//    eHAL_Status = HAL_UART_Init(&tWifi_UART_Handle);
//
//    if(eHAL_Status == HAL_OK)
//    {
//      eEC = ER_OK;
//      tBSP_AS.bIs_Wifi_Intf_Init = true;
//    }
//    else
//    {
//      eEC = ER_FAIL;
//      tBSP_AS.bIs_Wifi_Intf_Init = false;
//    }
//
//    tWifi_GPIO_Init.Pin    = GPIO_PIN_0;
//    tWifi_GPIO_Init.Mode   = GPIO_MODE_OUTPUT_PP;
//    tWifi_GPIO_Init.Pull   = GPIO_NOPULL;
//    tWifi_GPIO_Init.Speed  = GPIO_SPEED_HIGH;
//    HAL_GPIO_Init(GPIOA,&tWifi_GPIO_Init);
    tBSP_AS.bIs_Wifi_Intf_Init = true;
  }
  else
  {
    eEC = ER_OK;
  }
  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Usb_Intf_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;

  //The low level interface initialization is handled by the 3rd party driver
  if(tBSP_AS.bIs_Usb_Intf_Init == false)
  {
    tBSP_AS.bIs_Usb_Intf_Init = true;
  }

  eEC = ER_OK;

  return eEC;
}

/******************************************************************************
* todo: DESCRIPTION
* name: eBSP_SystemClock_Config
* description:
* param description: none
* return value description: ERROR_CODE - ER_OK: System clock configured
*                                      - ER_FAIL: System clock configuration process failed
******************************************************************************/
ERROR_CODE eBSP_SystemClock_Config(void)
{

  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_STATUS = HAL_ERROR;

  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  eHAL_STATUS = HAL_RCC_OscConfig(&RCC_OscInitStruct);

  if(eHAL_STATUS == HAL_OK)
  {
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
   clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    eHAL_STATUS = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  }

  if(eHAL_STATUS == HAL_OK)
  {
    /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
    if (HAL_GetREVID() == 0x1001)
    {
      /* Enable the Flash prefetch */
      __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }
  }

  if(eHAL_STATUS == HAL_OK)
  {
    eEC = ER_OK;
  }

  return eEC;

}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Camera_Intf_Send(pBSP_Camera_Send pParam)
{
  ERROR_CODE eEC = ER_FAIL;

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Camera_Intf_Receive(pBSP_Camera_Receive pParam)
{
  ERROR_CODE eEC = ER_FAIL;

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Rst_Clr(void)
{
  ERROR_CODE eEC = ER_OK;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Rst_Set(void)
{
  ERROR_CODE eEC = ER_OK;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_En_Clr(void)
{
  ERROR_CODE eEC = ER_OK;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_En_Set(void)
{
  ERROR_CODE eEC = ER_OK;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

  return eEC;
}

/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name: eBSP_Wifi_Intf_Config
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Config(pUART_Config pConfig)
{
  ERROR_CODE eEC = ER_FAIL;

  if((pConfig->eDataBits) & (pConfig->eParity  ) &
     (pConfig->eStopBits) & (pConfig->eControl ) &
     (pConfig->uiBaud   ))
  {
    /*config the uart*/
  }

  if(pConfig->vUART_ISR != NULL)
  {

  }

  if(pConfig->vUART_Rcv_Timeout_ISR != NULL)
  {

  }

  if(pConfig->vUART_Send_Timeout_ISR != NULL)
  {

  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Send(pBSP_Wifi_Transmit pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

//  eHAL_Status = HAL_USART_Transmit(&tWifi_UART_Handle, pParam->pBuff, pParam->uiBuff_Len, 3000);
  eHAL_Status = HAL_UART_Transmit(&WIFI_UART_HANDLE, pParam->pBuff, pParam->uiBuff_Len, 3000);

//  vDEBUG((char *)pParam->pBuff);

  if(eHAL_Status == HAL_OK)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Receive(pBSP_Wifi_Receive pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  eHAL_Status = HAL_UART_Receive(&WIFI_UART_HANDLE, pParam->pBuff, pParam->uiBuff_Len, 100);
  if(eHAL_Status == HAL_OK)
  {
    eEC = ER_OK;
  }
  else if(eHAL_Status == HAL_TIMEOUT)
  {
    eEC = ER_TIMEOUT;
  }
  else
  {
    eEC = ER_FAIL;
  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Receive_IT(pBSP_Wifi_Receive pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  eHAL_Status = HAL_UART_Receive_IT(&WIFI_UART_HANDLE, pParam->pBuff, pParam->uiBuff_Len);
  if((eHAL_Status == HAL_OK) |
     (eHAL_Status == HAL_BUSY))
  {
    eEC = ER_OK;
  }
  else
  {
    eEC = ER_FAIL;
  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_FLASH_READ(pBSP_Flash_Read pParam)
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
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_FLASH_WRITE(pBSP_Flash_Write pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_EC = HAL_ERROR;
  uint32_t uiStartSector;
  uint32_t uiEndSector;
  uint32_t uiSectorIndex;
  uint32_t uiData;

  uiStartSector = ADDR_FLASH_SECTOR_11;
  uiSectorIndex = uiStartSector;
  uiEndSector = uiStartSector + pParam->uiBuff_Len;

  eHAL_EC = HAL_FLASH_Unlock();
  vDEBUG_ASSERT("eBSP_FLASH_WRITE unlock fail", eHAL_EC == HAL_OK);

  //todo: disable interrupts

  /* Clear pending flags (if any) */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

  while(uiSectorIndex < uiEndSector)
  {
    uiData = 0;
    uiData |= (uint32_t)pParam->pBuff[3];
    uiData <<= 8;
    uiData |= (uint32_t)pParam->pBuff[2];
    uiData <<= 8;
    uiData |= (uint32_t)pParam->pBuff[1];
    uiData <<= 8;
    uiData |= (uint32_t)pParam->pBuff[0];
    eHAL_EC = HAL_FLASH_Program(TYPEPROGRAM_WORD, uiSectorIndex, uiData);
    if(eHAL_EC != HAL_OK)
    {
      break;
    }
    pParam->pBuff += 4;
    uiSectorIndex += 4;
  }
  //todo: enable interrupts
  vDEBUG_ASSERT("eBSP_FLASH_WRITE program fail", eHAL_EC == HAL_OK);

  eHAL_EC = HAL_FLASH_Lock();
  vDEBUG_ASSERT("eBSP_FLASH_WRITE lock fail", eHAL_EC == HAL_OK);

  if(eHAL_EC == HAL_OK)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_FLASH_GET_START_ADDR(uint32_t * pStartAddr)
{
  ERROR_CODE eEC = ER_FAIL;

  if(pStartAddr != NULL)
  {
    *pStartAddr = FLASH_USER_START_ADDR;
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_FLASH_ERASE(void)
{
  ERROR_CODE eEC = ER_FAIL;
  uint32_t fault_sec = 0;
  HAL_StatusTypeDef eHAL_EC = HAL_ERROR;

  FLASH_EraseInitTypeDef er
  = {
      .TypeErase = TYPEERASE_SECTORS,
      .Sector = FLASH_SECTOR_11,
      .NbSectors = 1,
      .VoltageRange = VOLTAGE_RANGE_3
  };

  HAL_FLASH_Unlock();
  eHAL_EC = HAL_FLASHEx_Erase(&er, &fault_sec);
  vDEBUG_ASSERT("HAL_FLASHEx_Erase fail", eHAL_EC == HAL_OK);
  HAL_FLASH_Lock();

  if(eHAL_EC == HAL_OK)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
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

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_Get_Current_ms_count(uint32_t * uiSystem_total_ms_count)
{
  ERROR_CODE eEC = ER_OK;

  *uiSystem_total_ms_count = HAL_GetTick();

  return eEC;
}

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: eBSP_Board_Init
* description:
* param description: void
* return value description: ERROR_CODE - ER_OK: board init good
*                                        ER_FAIL: board init fail
******************************************************************************/
ERROR_CODE eBSP_Board_Init(void)
{
  ERROR_CODE eEC = ER_OK;

//  eEC = eBSP_Camera_Intf_Init();

  if(eEC == ER_OK)
  {
    eEC = eBSP_Wifi_Intf_Init();
  }

  if(eEC == ER_OK)
  {
    eEC = eBSP_Usb_Intf_Init();
  }

  return eEC;
}

#endif //__FILE_NAME_C__
