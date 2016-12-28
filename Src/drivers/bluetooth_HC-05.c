/******************************************************************************
*
* bluetooth_HC-05.c - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __BLUETOOTH_HC05_C__
#define __BLUETOOTH_HC05_C__
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
#if(PROJ_CONFIG_PLATFORM_BT == PROJ_CONFIG_PLATFORM_BT_HC05)
  /* Project specific include files here */

//Utility includes
  #include "utils_inc/error_codes.h"
  #include "utils_inc/util_debug.h"
  /* Utility include files here */

//Driver includes
  #include "drivers_inc/bluetooth_HC-05.h"
  /* Driver include files here */

//Platform includes
  #include "board.h"
  /* Platform include files here */

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static uint8_t uiRcv_Buff;

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef enum HC05_STATE
{
  HC05_STATE_NONE,
  HC05_STATE_INIT,
  HC05_STATE_READY,
  HC05_STATE_LIMIT,
}HC05_STATE, * pHC05_STATE;

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
typedef struct Bluetooth_HC05_Activity_State
{
  bool         bModuleConfigured;
  HC05_STATE   eState;
}Bluetooth_HC05_Activity_State_t, * pBluetooth_HC05_Activity_State;

Bluetooth_HC05_Activity_State_t Bluetooth_HC05_AS_t =
{
  .bModuleConfigured = false,
  .eState = HC05_STATE_NONE,
};
/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

void vBluetooth_HC05_intf_isr_callback(void)
{
  BSP_BT_Rcv_t  BT_Rcv_t;
  BT_Rcv_t.uiLen = 1;
  BT_Rcv_t.pBuff = &uiRcv_Buff;
  eBSP_BT_INTF_RCV(&BT_Rcv_t);
  return;
}

ERROR_CODE eBluetooth_HC05_setup(void)
{
  ERROR_CODE eEC = ER_FAIL;
  BSP_BT_Send_t BT_Send_t;
  BSP_BT_Rcv_t  BT_Rcv_t;
  static char at_reset[] = "AT+RESET\r\n";
  UART_Config_t BT_Config_t;

  memset(&BT_Config_t, 0x00, sizeof(UART_Config_t));
  BT_Config_t.eDataBits = DATA_BITS_8;
  BT_Config_t.eParity = PARITY_NONE;
  BT_Config_t.eStopBits = STOP_BITS_1;
  BT_Config_t.eControl = FLOW_CONTROL_NONE;
  BT_Config_t.uiBaud = 57600;
  BT_Config_t.uiTimeout = 3000;
  BT_Config_t.vUART_ISR = &vBluetooth_HC05_intf_isr_callback;
  eEC = eBSP_BT_INTF_CONFIG(&BT_Config_t);

  //kick off data reception from the bluetooth module
  BT_Rcv_t.uiLen = 1;
  BT_Rcv_t.pBuff = &uiRcv_Buff;
  eEC = eBSP_BT_INTF_RCV(&BT_Rcv_t);

  BT_Send_t.pBuff = (uint8_t *)at_reset;
  BT_Send_t.uiLen = strlen(at_reset);
  eEC = eBSP_BT_INTF_SEND(&BT_Send_t);

  return eEC;
}

#endif //(PROJ_CONFIG_PLATFORM_BT == PROJ_CONFIG_PLATFORM_BT_HC05)
#endif //__BLUETOOTH_HC05_C__
