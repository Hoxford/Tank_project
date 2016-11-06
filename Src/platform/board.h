/******************************************************************************
*
* board.h - platform board include file
* Copyright (c) notice
*
******************************************************************************/
#ifndef __BOARD_H__
#define __BOARD_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#include "proj_inc/project_config.h"

#if ((PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_STM32F4DISC)  | \
     (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF401RE) |\
     (PROJ_CONFIG_PLATFORM == PROJ_CONFIG_PLATFORM_NUCLEOF412ZG))
  #include "stm32f4xx_hal.h"
#else
  #error "Platform not defined!"
#endif //PROJ_CONFIG_PLATFORM
/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
#define FLASH_SETTINGS_INDEX    ((uint32_t)0x080E0000) /* Base of Sector flash settings index */

/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef enum UART_DATA_BITS
{
  DATA_BITS_INVALID,
  DATA_BITS_8,
  DATA_BITS_7,
  DATA_BITS_6,
  DATA_BITS_5,
  DATA_BITS_LIMIT,
}UART_DATA_BITS, * pUART_DATA_BITS;

typedef enum UART_PARITY
{
  PARITY_INVALID,
  PARITY_NONE,
  PARITY_ODD,
  PARITY_MARK,
  PARITY_LIMIT,
}UART_PARITY, * pUART_PARITY;

typedef enum UART_STOP_BITS
{
  STOP_BITS_INVALID,
  STOP_BITS_1,
  STOP_BITS_2,
  STOP_BITS_LIMIT,
}UART_STOP_BITS, * pUART_STOP_BITS;

typedef enum UART_FLOW_CONTROL
{
  FLOW_CONTROL_INVALID,
  FLOW_CONTROL_NONE,
  FLOW_CONTROL_RTS_CTS,
  FLOW_CONTROL_LIMIT,
}UART_FLOW_CONTROL, * pUART_FLOW_CONTROL;

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct BSP_Camera_Send_t
{
  uint16_t uiBuffLen;
  uint8_t * pBuff;
}BSP_Camera_Send_t, *pBSP_Camera_Send;

typedef struct BSP_Camera_Receive_t
{
  uint16_t uiBuffLen;
  uint8_t * pBuff;
}BSP_Camera_Receive_t, * pBSP_Camera_Receive;

typedef struct UART_Config_t
{
  UART_DATA_BITS                      eDataBits;//
  UART_PARITY                         eParity;//
  UART_STOP_BITS                      eStopBits;//
  UART_FLOW_CONTROL                   eControl;//
  uint32_t                            uiBaud;
  uint32_t                            uiTimeout;
  void (* vUART_ISR)                  (void);//ISR to call for UART isr
  void (* vUART_Rcv_Timeout_ISR)      (void);//ISR to call for data rcv timeout
  void (* vUART_Send_Timeout_ISR)     (void);//ISR to call for data send timeout
}UART_Config_t, *pUART_Config;

typedef struct tBSP_tWifi_Transmit
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
}BSP_Wifi_Transmit_t, * pBSP_Wifi_Transmit;

typedef struct BSP_Wifi_Receive_t
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
}BSP_Wifi_Receive_t, * pBSP_Wifi_Receive;

typedef struct BSP_Flash_Read_t
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
  uint32_t uiStart_Addr;
  uint32_t uiEnd_Addr;
}BSP_Flash_Read_t, * pBSP_Flash_Read;

typedef struct BSP_Flash_Write_t
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
}BSP_Flash_Write_t, * pBSP_Flash_Write;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eBSP_Camera_Intf_Send(pBSP_Camera_Send pParam);
ERROR_CODE eBSP_Camera_Intf_Receive(pBSP_Camera_Receive pParam);
ERROR_CODE eBSP_Wifi_Rst_Clr(void);
ERROR_CODE eBSP_Wifi_Rst_Set(void);
ERROR_CODE eBSP_Wifi_En_Clr(void);
ERROR_CODE eBSP_Wifi_En_Set(void);
ERROR_CODE eBSP_Wifi_Intf_Config(pUART_Config pConfig);
ERROR_CODE eBSP_Wifi_Intf_Send(tBSP_tWifi_Transmit * pParam);
ERROR_CODE eBSP_Wifi_Intf_Receive(pBSP_Wifi_Receive pParam);
ERROR_CODE eBSP_Wifi_Intf_Receive_IT(pBSP_Wifi_Receive pParam);
ERROR_CODE eBSP_FLASH_READ(tBSP_Flash_Read * pParam);
ERROR_CODE eBSP_FLASH_WRITE(tBSP_Flash_Write * pParam);
ERROR_CODE eBSP_FLASH_GET_START_ADDR(uint32_t * pStartAddr);
ERROR_CODE eBSP_FLASH_ERASE(void);
ERROR_CODE eBSP_Get_Current_ms_count(uint32_t * uiSystem_total_ms_count);

/******************************************************************************
* name: eBSP_Board_Init
*
* description: Performs low level initialization of the platform interfaces and
* features required by the project. Call this function after any proprietary
* Initialization takes place and before task and OS instantiation.
*
* param description: void
*
* return value description: ERROR_CODE - ER_OK: board init successful
*                                        ER_FAIL: board init fail
*
******************************************************************************/
ERROR_CODE eBSP_Board_Init(void);

#endif //__FILE_NAME_H__
