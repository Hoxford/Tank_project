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

typedef struct tBSP_Camera_Send
{
  uint16_t uiBuffLen;
  uint8_t * pBuff;
}tBSP_Camera_Send;

typedef struct tBSP_Camera_Receive
{
  uint16_t uiBuffLen;
  uint8_t * pBuff;
}tBSP_Camera_Receive;

typedef struct tBSP_tWifi_Transmit
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
}tBSP_tWifi_Transmit;

typedef struct tBSP_Wifi_Receive
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
}tBSP_Wifi_Receive;

typedef struct tBSP_Flash_Read
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
  uint32_t uiStart_Addr;
  uint32_t uiEnd_Addr;
}tBSP_Flash_Read;

typedef struct tBSP_Flash_Write
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
}tBSP_Flash_Write;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eBSP_Camera_Intf_Send(tBSP_Camera_Send * pParam);
ERROR_CODE eBSP_Camera_Intf_Receive(tBSP_Camera_Receive * pParam);
ERROR_CODE eBSP_Wifi_Rst_Clr(void);
ERROR_CODE eBSP_Wifi_Rst_Set(void);
ERROR_CODE eBSP_Wifi_Rst(void);
ERROR_CODE eBSP_Wifi_Intf_Send(tBSP_tWifi_Transmit * pParam);
ERROR_CODE eBSP_Wifi_Intf_Receive(tBSP_Wifi_Receive * pParam);
ERROR_CODE eBSP_FLASH_READ(tBSP_Flash_Read * pParam);
ERROR_CODE eBSP_FLASH_WRITE(tBSP_Flash_Write * pParam);
ERROR_CODE eBSP_FLASH_GET_START_ADDR(uint32_t * pStartAddr);
ERROR_CODE eBSP_FLASH_ERASE(void);
ERROR_CODE eBSP_Get_Current_ms_count(uint32_t *uiSystem_total_ms_count);
ERROR_CODE eBSP_Board_Init(void);

#endif //__FILE_NAME_H__
