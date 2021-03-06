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
  DATA_BITS_9,
  DATA_BITS_8,
  DATA_BITS_LIMIT,
}UART_DATA_BITS, * pUART_DATA_BITS;

typedef enum UART_PARITY
{
  PARITY_INVALID,
  PARITY_NONE,
  PARITY_EVEN,
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
  FLOW_CONTROL_RTS,
  FLOW_CONTROL_CTS,
  FLOW_CONTROL_RTS_CTS,
  FLOW_CONTROL_LIMIT,
}UART_FLOW_CONTROL, * pUART_FLOW_CONTROL;

typedef enum UART_MODE
{
  MODE_NONE,
  MODE_TX,
  MODE_RX,
  MODE_TX_RX,
  MODE_LIMIT,
}UART_MODE, * pUART_MODE;

typedef enum UART_DATA_HANDLING
{
  HANDLING_NONE,
  HANDLING_DMA,
  HANDLING_LIMIT,
}UART_DATA_HANDLING, * pUART_DATA_HANDLING;

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

typedef struct BSP_BT_Send
{
  uint32_t  uiLen;
  uint8_t * pBuff;
}BSP_BT_Send_t, * pBSP_BT_Send;

typedef struct BSP_BT_Rcv
{
  uint32_t  uiLen;
  volatile uint8_t * pBuff;
}BSP_BT_Rcv_t, * pBSP_BT_Rcv;

typedef struct UART_Config_t
{
  UART_DATA_BITS                    eDataBits;//
  UART_PARITY                       eParity;//
  UART_STOP_BITS                    eStopBits;//
  UART_FLOW_CONTROL                 eControl;//
  UART_MODE                         eMode;
  UART_DATA_HANDLING                eDataHandling; //Handling of the data, DMA or not
  uint32_t                          uiBaud;
  uint32_t                          uiTimeout;
  void (* vUART_ISR)                (void);//ISR to call for UART isr
  void (* vUART_Rcv_Timeout_ISR)    (void);//ISR to call for data rcv timeout
  void (* vUART_Send_Timeout_ISR)   (void);//ISR to call for data send timeout
}UART_Config_t, *pUART_Config;

typedef struct BSP_Wifi_Transmit
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
}BSP_Wifi_Transmit_t, * pBSP_Wifi_Transmit;

typedef struct BSP_Wifi_Receive_t
{
  uint8_t * pBuff;
  uint32_t uiBuff_Len;
  uint32_t uiTimeout;
  bool     bBlocking;
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

ERROR_CODE eBSP_CAMERA_POWER_ENABLE(void);
ERROR_CODE eBSP_CAMERA_POWER_DISABLE(void);
ERROR_CODE eBSP_CAMERA_RESET_SET(void);
ERROR_CODE eBSP_CAMERA_RESET_CLEAR(void);
ERROR_CODE eBSP_Camera_Intf_Send(pBSP_Camera_Send pParam);
ERROR_CODE eBSP_Camera_Intf_Receive(pBSP_Camera_Receive pParam);
ERROR_CODE eBSP_BT_POWER_ENABLE(void);
ERROR_CODE eBSP_BT_POWER_DISABLE(void);
ERROR_CODE eBSP_BT_CONFIG_MODE_ENABLE(void);
ERROR_CODE eBSP_BT_CONFIG_MODE_DISABLE(void);
ERROR_CODE eBSP_BT_INTF_SEND(pBSP_BT_Send pParam);
void       vBSP_BT_INTF_SEND_IT_HANDLER(void);
ERROR_CODE eBSP_BT_INTF_RCV(pBSP_BT_Rcv pParam);
ERROR_CODE eBSP_BT_INTF_RCV_IT(pBSP_BT_Rcv pParam);
ERROR_CODE eBSP_BT_INTF_DMA_RCV(pBSP_BT_Rcv pParam);
void       vBSP_BT_INTF_RECEIVE_DMA_IT_HANDLER();
ERROR_CODE eBSP_BT_INTF_RCV_CANCEL(void);
ERROR_CODE eBSP_BT_INTF_CONFIG(pUART_Config pParam);
ERROR_CODE eBSP_BT_INTF_DMA_DISABLE(void);
ERROR_CODE eBSP_BT_INTF_DMA_ENABLE(void);
ERROR_CODE eBSP_Wifi_Rst_Clr(void);
ERROR_CODE eBSP_Wifi_Rst_Set(void);
ERROR_CODE eBSP_Wifi_Rst(void);
ERROR_CODE eBSP_Wifi_En_Clr(void);
ERROR_CODE eBSP_Wifi_En_Set(void);
ERROR_CODE eBSP_Wifi_Intf_Config(pUART_Config pConfig);

/******************************************************************************
* Name: eBSP_Wifi_Intf_Send
* Description:
*   Sends data to the wifi interface.
*
* Parameters:
*   pBSP_Wifi_Transmit pParam:
*     ->(type) name: description if member of struct
*       = value(optional): description if member of struct value set
*   example:
*   (bool) bVar: do action option
*     = false: do action when set to false
*     = true: do other action when set to true
*   (pPub_Example_Struct) pParam: pointer to the param structure
*     ->(int)iVar1: description
*     ->(int *)pVar2: description
*     ->(bool)bVar3: description
*     ->(PUB_EXAMPLE_ENUM)eENUM: description
*       = PUB_ENUM_A: do action when set to _A
*       = PUB_ENUM_B: do action when set to _B
*       = PUB_ENUM_C: do action when set to _C
*     ->void (* pCallback_Fcn)(void * param): Function pointer for callback.
*
*todo: Returns:
*   (type): description
*     = value (optional): value description
*   examples:
*   (bool):
*     = true: did function action and result is true
*     = false: did function action and result is false
*   (int): integer value description after function action
*   (pPub_Example_Struct):
*     = (uint32_t *): address of the created object
*     =             - NULL: created object fail
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Send(pBSP_Wifi_Transmit pParam);

/******************************************************************************
* name: eBSP_Wifi_Intf_Receive
*
* description: Wifi hardware module interface receive function. Performs either
*   a blocking or non-blocking (ISR based) receive from the external wifi.
*   hardware module.
* parameters: pBSP_Wifi_Receive pParam: pointer to the bsp read structure
*               ->(uint8_t *)pBuff;
*               ->(uint32_t)uiBuff_Len;
*               ->(uint32_t)uiTimeout;
*               ->(bool)bBlocking;
*
* Returns: ERROR_CODE: Enum error code
*            = ER_FAIL:
*            = ER_OK:
******************************************************************************/
ERROR_CODE eBSP_Wifi_Intf_Receive(pBSP_Wifi_Receive pParam);

ERROR_CODE eBSP_Wifi_Intf_Receive_IT(pBSP_Wifi_Receive pParam);

/******************************************************************************
* Name: eBSP_FLASH_READ
*
* Description: Reads the settings and data stored in NV ram. The section &
*   addresses of NV ram read from is determined by the project settings and is
*   not dynamic or modifiable. When this function is called it will
*   automatically begin reading NV ram and filling the provided buffer with the
*   contents of NV memory for the length as defined by the parameters.
*
* Parameters: pBSP_Flash_Read pParam: pointer to the bsp read NV memory
*               parameter structure.
*               ->(uint8_t *)pBuff: Pointer to the buffer to fill
*               ->(uint32_t)uiBuff_Len: Length in bytes to read from NV memory
*               ->(uint32_t)uiStart_Addr: Start address in NV memory to read
*                 from.
*               ->(uint32_t)uiEnd_Addr: End address in NV memory to read from.
*
* Returns: ERROR_CODE:
*            = ER_OK:
*            = ER_FAIL:
******************************************************************************/
ERROR_CODE eBSP_FLASH_READ(pBSP_Flash_Read pParam);

/******************************************************************************
* Name: eBSP_FLASH_WRITE
*
* Description: Writes the settings and data passed in via the parameter to
*   NV ram. The section & addresses of NV ram written to is determined by
*   the project settings and is not modifiable. When this function is called
*   it will automatically begin filling persistent memory from the project
*   defined start address for the length as defined by the parameters.
*
* Parameters: pBSP_Flash_Write pParam: Pointer to the BSP_Flash_Write_t
*               parameter structure.
*               ->(uint8_t * )pBuff: Pointer to the buffer to write to
*                 NV ram.
*               ->(uint32_t)uiBuff_Len: Length in bytes of the data to
*                 write to NV ram.
*
* Returns: ERROR_CODE:
*            ER_FAIL: Failed to write to NV Ram due to lock/un-lock permissions
*              or write error.
*            ER_OK: Flash write successful.
*
* Example:
* System_Settings_Save(void)
* {
*   uint8_t uiSettings[256];
*   BSP_Flash_Write_t Write_t;
*   ERROR_CODE eEC;
*
*   get_settings(uiSettings);
*
*   Write_t.pBuff = uiSettings;
*   Write_t.uiBuff_Len = 256;
*
*   eEC = eBSP_FLASH_WRITE(&Write_t);
*   //Check error code
* }
******************************************************************************/
ERROR_CODE eBSP_FLASH_WRITE(pBSP_Flash_Write pParam);

/******************************************************************************
* name: eBSP_FLASH_GET_START_ADDR
*
* description: Returns the start address of the persistent settings storage in
*   the pointer from the parameter.
*
* Parameters: uint32_t * pStartAddr: Pointer to an unsigned 32 bit int to store
*   the persistent setting start address in.
*
* Returns: ERROR_CODE:
*            ER_FAIL: Failed to get the start address due to parameter or
*              hardware read error.
*            ER_OK: Persistent settings start address successfully retrieved.
******************************************************************************/
ERROR_CODE eBSP_FLASH_GET_START_ADDR(uint32_t * pStartAddr);

/******************************************************************************
* name: eBSP_FLASH_ERASE
*
* description: Erases the project defined persistent setting storage. Any
*   previous settings that were stored in memory will not be preserved in any
*   way. This function should only be used in the case of factory defaults
*   reset or similar.
*
* param description: none
*
* returns: ERROR_CODE:
*            ER_FAIL: Flash erase fail.
*            ER_OK: Flash erase successful.
*
* Example:
* Factory_Reset(void)
* {
*   ERROR_CODE eEC;
*
*   eEC = eBSP_FLASH_ERASE();
*   if(eEC == ER_OK)
*   {
*     //perform reset
*   }
*   else
*   {
*     //log error
*   }
* }
******************************************************************************/
ERROR_CODE eBSP_FLASH_ERASE(void);

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name:
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eBSP_SystemClock_Config(void);

/******************************************************************************
* Name: eBSP_Get_Current_ms_count
*
* Description: Gets the current millisecond count since last reset. The count
* is returned in the pointer given via the parameter.
*
* Parameters:
*   uint32_t * uiSystem_total_ms_count: Pointer to a local uint32_t variable to
*     contain the current MS count.
*
* Returns: ERROR_CODE:
*          = ER_OK: ms count successfully retrieved.
*
* Example:
* foo(void)
* {
*   uint32_t uiCurrentTick, uiEndTick;
*   eBSP_Get_Current_ms_count(&uiCurrentTick);
*   uiEndTick = uiCurrentTick + 5;
*
*   while(uiEndTick > uiCurrentTick)
*   {
*     //Do something
*     eBSP_Get_Current_ms_count(&uiCurrentTick);
*   }
* }
*
******************************************************************************/
ERROR_CODE eBSP_Get_Current_ms_count(uint32_t * uiSystem_total_ms_count);

/******************************************************************************
* name: eBSP_Board_Init
*
* description: Performs low level initialization of the platform interfaces and
* features required by the project. Call this function after any proprietary
* Initialization takes place and before task and OS instantiation.
*
* Parameters: void
*
* Returns: ERROR_CODE:
*            = ER_OK: board init successful
*            = ER_FAIL: board init fail
******************************************************************************/
ERROR_CODE eBSP_Board_Init(void);

#endif //__FILE_NAME_H__
