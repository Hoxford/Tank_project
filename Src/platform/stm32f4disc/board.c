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

//Standard includes
  #include <stdint.h>
  #include <stdbool.h>
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>

//Project specific includes
  #include "proj_inc/project_config.h"
#if (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  /* Utility include files here */

//Third party includes
  #include "ThirdParty_inc/osal.h"
  /* Third party include files here */

//Platform includes
  #include "board.h"
  //STM32F4 specific includes
  //#include "stm32f4xx.h"
  #include "stm32f4xx_hal.h"
  #include "core_cm4.h"
  /* Platform include files here */





/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//camera defines
#define CAMERA_TIMER_INSTANCE  TIM4
#define CAMERA_I2C_INSTANCE    I2C2
#define I2C_ADDRESS            0x30F

//bluetooth defines
  #define BT_TX_BUF_LEN         512
  #define BT_RX_BUF_LEN       512
  #define BT_UART_HANDLE      huart4
  #define BT_UART_HANDLE_T    UART_HandleTypeDef
  #define BT_INF_IRQ_HANDLER  HAL_UART_RxCpltCallback
  #define BT_INTF_IRQN        USART4_IRQn

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

extern UART_HandleTypeDef huart4;
/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct BSP_Activity_State
{
  bool bIs_Camera_Intf_Init;
  bool bIs_Wifi_Intf_Init;
  bool bIs_Usb_Intf_Init;
  bool bIs_BT_Intf_Receiving;
  bool bIs_BT_Intf_Sending;
  void * pPointer;
  void (* vUART_ISR[5])(void);
//  void (* vUART_ISR)(void);

}BSP_Activity_State_t, *pBSP_Activity_State;

BSP_Activity_State_t BSP_AS_t =
{
  .bIs_Camera_Intf_Init  = false,
  .bIs_Wifi_Intf_Init    = false,
  .bIs_Usb_Intf_Init     = false,
  .bIs_BT_Intf_Receiving = false,
  .bIs_BT_Intf_Sending   = false,
  .pPointer              = NULL,
  .vUART_ISR[0] = NULL,
  .vUART_ISR[1] = NULL,
  .vUART_ISR[2] = NULL,
  .vUART_ISR[3] = NULL,
  .vUART_ISR[4] = NULL,
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

static ERROR_CODE eBSP_Wifi_Intf_Init(void);
static ERROR_CODE eBSP_Usb_Intf_Init(void);
static uint32_t   uiGetUartBits(UART_DATA_BITS eDataBits);
static uint32_t   uiGetUartParity(UART_PARITY eParity);
static uint32_t   uiGetUartStopBits(UART_STOP_BITS eStopBits);
static uint32_t   uiGetUartFlowControl(UART_FLOW_CONTROL eControl);
static uint32_t   uiGetUartMode(UART_MODE eMode);

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
static ERROR_CODE eBSP_Wifi_Intf_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;
//  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;
//  GPIO_InitTypeDef      tWifi_GPIO_Init;

  if(BSP_AS_t.bIs_Wifi_Intf_Init == false)
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
//      BSP_AS_t.bIs_Wifi_Intf_Init = true;
//    }
//    else
//    {
//      eEC = ER_FAIL;
//      BSP_AS_t.bIs_Wifi_Intf_Init = false;
//    }
//
//    tWifi_GPIO_Init.Pin    = GPIO_PIN_0;
//    tWifi_GPIO_Init.Mode   = GPIO_MODE_OUTPUT_PP;
//    tWifi_GPIO_Init.Pull   = GPIO_NOPULL;
//    tWifi_GPIO_Init.Speed  = GPIO_SPEED_HIGH;
//    HAL_GPIO_Init(GPIOA,&tWifi_GPIO_Init);
    BSP_AS_t.bIs_Wifi_Intf_Init = true;
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
static ERROR_CODE eBSP_Usb_Intf_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;

  //The low level interface initialization is handled by the 3rd party driver
  if(BSP_AS_t.bIs_Usb_Intf_Init == false)
  {
    BSP_AS_t.bIs_Usb_Intf_Init = true;
  }

  eEC = ER_OK;

  return eEC;
}

static uint32_t uiGetUartBits(UART_DATA_BITS eDataBits)
{
  uint32_t uiBits = 0xFFFFFFFF;
  if((DATA_BITS_INVALID < eDataBits)&(eDataBits < DATA_BITS_LIMIT))
  {
    if(eDataBits == DATA_BITS_9)
      uiBits = UART_WORDLENGTH_9B;
    else if(eDataBits == DATA_BITS_8)
      uiBits = UART_WORDLENGTH_8B;
  }

  return uiBits;
}

static uint32_t uiGetUartParity(UART_PARITY eParity)
{
  uint32_t uiParity = 0xFFFFFFFF;
  if((PARITY_INVALID < eParity)&(eParity < PARITY_LIMIT))
  {
    if(eParity == PARITY_NONE)
      uiParity = UART_PARITY_NONE;
    else if(eParity == PARITY_EVEN)
      uiParity = UART_PARITY_EVEN;
    else if(eParity == PARITY_ODD)
      uiParity = UART_PARITY_ODD;
    else if(eParity == PARITY_MARK)
      uiParity = UART_PARITY_NONE;
  }

  return uiParity;
}

static uint32_t uiGetUartStopBits(UART_STOP_BITS eStopBits)
{
  uint32_t uiStopBits = 0xFFFFFFFF;
  if((STOP_BITS_INVALID < eStopBits)&(eStopBits < STOP_BITS_LIMIT))
  {
    if(eStopBits == STOP_BITS_1)
      uiStopBits = UART_STOPBITS_1;
    else if(eStopBits == STOP_BITS_2)
      uiStopBits = UART_STOPBITS_2;
  }

  return uiStopBits;
}

static uint32_t   uiGetUartFlowControl(UART_FLOW_CONTROL eControl)
{
  uint32_t uiFlowControl = 0xFFFFFFFF;

  if((FLOW_CONTROL_INVALID < eControl)&(eControl < FLOW_CONTROL_LIMIT))
  {
    if(eControl == FLOW_CONTROL_NONE)
      uiFlowControl = UART_HWCONTROL_NONE;
    else if(eControl == FLOW_CONTROL_RTS)
      uiFlowControl = UART_HWCONTROL_RTS;
    else if(eControl == FLOW_CONTROL_CTS)
      uiFlowControl = UART_HWCONTROL_CTS;
    else if(eControl == FLOW_CONTROL_RTS_CTS)
      uiFlowControl = UART_HWCONTROL_RTS_CTS;
  }
  return uiFlowControl;
}

static uint32_t uiGetUartMode(UART_MODE eMode)
{
  uint32_t uiMode = 0xFFFFFFFF;

  if((MODE_NONE < eMode)&(eMode < MODE_LIMIT))
  {
    if(eMode == MODE_TX)
      uiMode = UART_MODE_TX;
    else if(eMode == MODE_RX)
      uiMode = UART_MODE_RX;
    else if(eMode == MODE_TX_RX)
      uiMode = UART_MODE_TX_RX;
  }

  return uiMode;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: eBSP_CAMERA_POWER_ENABLE
******************************************************************************/
ERROR_CODE eBSP_CAMERA_POWER_ENABLE(void)
{
  ERROR_CODE eEC = ER_OK;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
  return eEC;
}

/******************************************************************************
* name: eBSP_CAMERA_POWER_DISABLE
******************************************************************************/
ERROR_CODE eBSP_CAMERA_POWER_DISABLE(void)
{
  ERROR_CODE eEC = ER_OK;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
  return eEC;
}

/******************************************************************************
* name: eBSP_CAMERA_RESET_SET
******************************************************************************/
ERROR_CODE eBSP_CAMERA_RESET_SET(void)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", false);
  return eEC;
}

/******************************************************************************
* name: eBSP_CAMERA_RESET_CLEAR
******************************************************************************/
ERROR_CODE eBSP_CAMERA_RESET_CLEAR(void)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", false);
  return eEC;
}

/******************************************************************************
* name: eBSP_Camera_Intf_Send
******************************************************************************/
ERROR_CODE eBSP_Camera_Intf_Send(pBSP_Camera_Send pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", false);
  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_Camera_Intf_Receive(pBSP_Camera_Receive pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("", false);
  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_POWER_ENABLE(void)
{
  ERROR_CODE eEC = ER_FAIL;

  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
  eEC = ER_OK;

  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_POWER_DISABLE(void)
{
  ERROR_CODE eEC = ER_FAIL;

  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
  eEC = ER_OK;

  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_CONFIG_MODE_ENABLE(void)
{
  ERROR_CODE eEC = ER_FAIL;

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
  eEC = ER_OK;

  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_CONFIG_MODE_DISABLE(void)
{
  ERROR_CODE eEC = ER_FAIL;

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
  eEC = ER_OK;

  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_INTF_SEND(pBSP_BT_Send pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  if(BT_UART_HANDLE.hdmatx == NULL)
  {
    eHAL_Status = HAL_UART_Transmit(&BT_UART_HANDLE, pParam->pBuff, pParam->uiLen, 3000);
  }
  else
  {
    if(BSP_AS_t.bIs_BT_Intf_Sending == true)
    {
      eEC = ER_BUSY;
    }
    else
    {
      eHAL_Status = HAL_UART_Transmit_DMA(&BT_UART_HANDLE, pParam->pBuff, pParam->uiLen);
      BSP_AS_t.bIs_BT_Intf_Sending = true;
    }
  }

  if(eHAL_Status == HAL_OK)
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
void vBSP_BT_INTF_SEND_IT_HANDLER(void)
{
  if(BT_UART_HANDLE.hdmatx == NULL)
  {

  }
  else
  {
    BSP_AS_t.bIs_BT_Intf_Sending = false;
  }

  return;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_INTF_RCV(pBSP_BT_Rcv pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  if(BSP_AS_t.bIs_BT_Intf_Receiving == false)
  {
    eHAL_Status = HAL_UART_Receive_IT(&BT_UART_HANDLE, (uint8_t *)pParam->pBuff, pParam->uiLen);
    if(eHAL_Status == HAL_OK)
    {
      BSP_AS_t.bIs_BT_Intf_Receiving = true;
      eEC = ER_OK;
    }
    else if(eHAL_Status == HAL_BUSY)
    {
      eEC = ER_TIMEOUT;
    }
    else
    {
      eEC = ER_FAIL;
    }
  }
  else
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* name: eBSP_BT_INTF_RCV_IT
******************************************************************************/
ERROR_CODE eBSP_BT_INTF_RCV_IT(pBSP_BT_Rcv pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  if(BSP_AS_t.bIs_BT_Intf_Receiving == true)
  {
    eHAL_Status = HAL_UART_Receive_IT(&BT_UART_HANDLE, (uint8_t *)pParam->pBuff, pParam->uiLen);
    if(eHAL_Status == HAL_OK)
    {
      BSP_AS_t.bIs_BT_Intf_Receiving = true;
      eEC = ER_OK;
    }
    else
    {
//      vDEBUG_ASSERT("",false);
    }
  }
  else
  {
    eEC = ER_FAIL;
  }

  return eEC;
}

/******************************************************************************
* name: eBSP_BT_INTF_DMA_RCV
******************************************************************************/
ERROR_CODE eBSP_BT_INTF_DMA_RCV(pBSP_BT_Rcv pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  eHAL_Status = HAL_UART_Receive_DMA(&BT_UART_HANDLE, (uint8_t *)pParam->pBuff, pParam->uiLen);
  if(eHAL_Status == HAL_OK)
  {
    BSP_AS_t.bIs_BT_Intf_Receiving = true;
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* name: vBSP_BT_INTF_RECEIVE_DMA_IT_HANDLER
******************************************************************************/
void vBSP_BT_INTF_RECEIVE_DMA_IT_HANDLER()
{
//  BSP_BT_Rcv_t  BT_Rcv_t;
//  BSP_AS_t.bIs_BT_Intf_Receiving = false;
//  if(HC05_AS_t.eState == HC05_STATE_READY)
//  {
//    eBTAppRcvCallBack(NULL, 8);
//  }
  return;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_INTF_RCV_CANCEL(void)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  if(BSP_AS_t.bIs_BT_Intf_Receiving == true)
  {
    eHAL_Status = HAL_UART_Abort(&BT_UART_HANDLE);
    if(eHAL_Status == HAL_OK)
    {
      BSP_AS_t.bIs_BT_Intf_Receiving = false;
      eEC = ER_OK;
    }
    else
    {
//      vDEBUG_ASSERT("",false);
    }
  }
  else
  {
    eEC = ER_OK;
  }

  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_INTF_CONFIG(pUART_Config pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  ERROR_CODE eEC_MX = ER_NONE;
  ERROR_CODE eEC_CB = ER_NONE;
  ERROR_CODE eEC_RCV_TO_CB = ER_NONE;
  ERROR_CODE eEC_SEND_TO_CB = ER_NONE;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;
  extern UART_HandleTypeDef BT_UART_HANDLE;

  if(((DATA_BITS_INVALID    <  pParam->eDataBits    )&(pParam->eDataBits     < DATA_BITS_LIMIT   )) &
     ((PARITY_INVALID       <  pParam->eParity      )&(pParam->eParity       < PARITY_LIMIT      )) &
     ((STOP_BITS_INVALID    <  pParam->eStopBits    )&(pParam->eStopBits     < STOP_BITS_LIMIT   )) &
     ((FLOW_CONTROL_INVALID <  pParam->eControl     )&(pParam->eControl      < FLOW_CONTROL_LIMIT)) &
     ((MODE_NONE            <  pParam->eMode        )&(pParam->eMode         < MODE_LIMIT        )) &
     ((HANDLING_NONE        <= pParam->eDataHandling)&(pParam->eDataHandling < HANDLING_LIMIT))     &
      (                   0 <  pParam->uiBaud))
  {
    BT_UART_HANDLE.Init.WordLength = uiGetUartBits       (pParam->eDataBits);
    BT_UART_HANDLE.Init.Parity     = uiGetUartParity     (pParam->eParity);
    BT_UART_HANDLE.Init.StopBits   = uiGetUartStopBits   (pParam->eStopBits);
    BT_UART_HANDLE.Init.HwFlowCtl  = uiGetUartFlowControl(pParam->eControl);
    BT_UART_HANDLE.Init.Mode       = uiGetUartMode       (pParam->eMode);
    BT_UART_HANDLE.Init.BaudRate   = pParam->uiBaud;

//    eHAL_Status = HAL_UART_DeInit(&BT_UART_HANDLE);
//    if(eHAL_Status == HAL_OK)
      eHAL_Status = HAL_UART_Init(&BT_UART_HANDLE);
//    else
//      eEC_MX = ER_FAIL;

    if(eHAL_Status == HAL_OK)
      eEC_MX = ER_OK;
    else
      eEC_MX = ER_FAIL;
  }

  if(pParam->vUART_ISR != NULL)
  {

  }

  if(pParam->vUART_Rcv_Timeout_ISR != NULL)
  {

  }

  if(pParam->vUART_Send_Timeout_ISR != NULL)
  {

  }

  if(((eEC_MX == ER_NONE)        |(eEC_MX == ER_OK))        &
     ((eEC_CB == ER_NONE)        |(eEC_CB == ER_OK))        &
     ((eEC_RCV_TO_CB == ER_NONE) |(eEC_RCV_TO_CB == ER_OK)) &
     ((eEC_SEND_TO_CB == ER_NONE)|(eEC_SEND_TO_CB == ER_OK)))
  {
    //set the receiving control variable to false since the uart was reconfigured.
    BSP_AS_t.bIs_BT_Intf_Receiving = false;
    eEC = ER_OK;
  }
  else
  {
    if(eEC_MX == ER_FAIL)
      eEC = ER_PARAM;
    else if(eEC_CB == ER_FAIL)
      eEC = ER_PARAM1;
    else if(eEC_RCV_TO_CB == ER_FAIL)
      eEC = ER_PARAM2;
    else if(eEC_SEND_TO_CB == ER_FAIL)
      eEC = ER_PARAM3;
  }

  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_INTF_DMA_DISABLE(void)
{
  ERROR_CODE eEC = ER_OK;

  HAL_NVIC_DisableIRQ(DMA1_Stream4_IRQn);

  return eEC;
}

/******************************************************************************
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_BT_INTF_DMA_ENABLE(void)
{
  ERROR_CODE eEC = ER_OK;

  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

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
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_Wifi_En_Set(void)
{
  ERROR_CODE eEC = ER_OK;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

  return eEC;
}

/******************************************************************************
* todo: name:
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
* todo: name:
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
* name: eBSP_Wifi_Intf_Receive
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Receive(pBSP_Wifi_Receive pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  HAL_StatusTypeDef eHAL_Status = HAL_ERROR;

  if(pParam->bBlocking == true)
  {
    eHAL_Status = HAL_UART_Receive(&WIFI_UART_HANDLE, pParam->pBuff, pParam->uiBuff_Len, pParam->uiTimeout);
    if(eHAL_Status == HAL_OK)
      eEC = ER_OK;
    else if(eHAL_Status == HAL_TIMEOUT)
      eEC = ER_TIMEOUT;
    else
      eEC = ER_FAIL;
  }
  else
  {
    eHAL_Status = HAL_UART_Receive_IT(&WIFI_UART_HANDLE, pParam->pBuff, pParam->uiBuff_Len);
    if(eHAL_Status == HAL_OK)
      eEC = ER_OK;
    else if(eHAL_Status == HAL_BUSY)
      eEC = ER_TIMEOUT;
    else
      eEC = ER_FAIL;
  }

  return eEC;
}

/******************************************************************************
* name: eBSP_FLASH_READ
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
* name: eBSP_FLASH_WRITE
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
* name: eBSP_FLASH_GET_START_ADDR
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
* name: eBSP_FLASH_ERASE
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
* name: eBSP_SystemClock_Config
* description:
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
* todo: name:
******************************************************************************/
ERROR_CODE eBSP_Inc_ms_count(void)
{
  ERROR_CODE eEC = ER_OK;



  return eEC;
}

/******************************************************************************
* name: eBSP_Get_Current_ms_count
******************************************************************************/
ERROR_CODE eBSP_Get_Current_ms_count(uint32_t * uiSystem_total_ms_count)
{
  ERROR_CODE eEC = ER_OK;

  *uiSystem_total_ms_count = HAL_GetTick();

  return eEC;
}

/******************************************************************************
* name: eBSP_Board_Init
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

#endif //PROJ_CONFIG_PLATFORM_STM32F4DISC
#endif //__BOARD_C__
