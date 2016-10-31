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

typedef struct BSP_Wifi_Transmit_t
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
ERROR_CODE eBSP_Wifi_Rst(void);
ERROR_CODE eBSP_Wifi_Intf_Send(pBSP_Wifi_Transmit pParam);
ERROR_CODE eBSP_Wifi_Intf_Receive(pBSP_Wifi_Receive pParam);
ERROR_CODE eBSP_FLASH_READ(pBSP_Flash_Read pParam);
ERROR_CODE eBSP_FLASH_WRITE(pBSP_Flash_Write pParam);
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
