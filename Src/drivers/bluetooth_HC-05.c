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

//Third party includes
  #include "ThirdParty_inc/osal.h"
  /* Third party include files here */

//Driver includes
  #include "drivers_inc/AT_commands.h"
  #include "drivers_inc/bluetooth_HC-05.h"
  /* Driver include files here */

//Platform includes
  #include "board.h"
  /* Platform include files here */

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define SETUP_RCV_BUFF_SIZE   512
#define SETUP_SEND_BUFF_SIZE  128

#define HC05_BAUD_38K         38400
#define HC05_BAUD_57K         57600
#define HC05_BAUD_115K        115200
#define HC05_BAUD_230K        230400
#define HC05_BAUD_460K        460800
#define HC05_BAUD_921K        921600
#define HC05_BAUD_1382K       1382400  //not supported on stm32f4discovery UART4

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static uint8_t uiRcv = 0;

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef enum HC05_STATE
{
  HC05_STATE_NONE,
  HC05_STATE_SETUP,
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
  uint8_t *    pSetupRcvBuff;
  uint8_t *    pSetupSendBuff;
  uint32_t     uiSetupBuffIndex;
}Bluetooth_HC05_Activity_State_t, * pBluetooth_HC05_Activity_State;

static Bluetooth_HC05_Activity_State_t HC05_AS_t =
{
  .bModuleConfigured = false,
  .eState            = HC05_STATE_NONE,
  .pSetupRcvBuff     = NULL,
  .pSetupSendBuff    = NULL,
  .uiSetupBuffIndex  = 0,
};
/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static ERROR_CODE eHC05_Check_Set_Baud(void);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: eHC05_Check_Set_Baud
* description:
*   Checks the baud rate of the HC05 bluetooth module to determine if it is set
*   to the right speed. If it is not set to the project defined baud rate the
*   function will then attempt to set the baude rate.
* parameters:
*   none
*
* return value description:
*   ERROR_CODE - ER_OK: Baud successfully checked and or set.
*              - ER_FAIL: Baud failed to be set.
*              - ER_SPEED: Was unable to determine HC05 baud speed rate with
*                          list of supported bauds.
******************************************************************************/
static ERROR_CODE eHC05_Check_Set_Baud(void)
{
  ERROR_CODE    eEC = ER_FAIL;
  BSP_BT_Send_t BT_Send_t;
  BSP_BT_Rcv_t  BT_Rcv_t;
  UART_Config_t BT_Config_t;
  uint32_t      uiWait = 0;
  uint32_t      uiSupportedBauds[] = {HC05_BAUD_38K, HC05_BAUD_57K, HC05_BAUD_115K, HC05_BAUD_230K, HC05_BAUD_460K, HC05_BAUD_921K};
  uint8_t       uiNumSupportedBauds = sizeof(uiSupportedBauds)/sizeof(uint32_t);
  uint8_t       uiBaudIndex = 0;
  bool          bUartBaudSetNeeded = false;
//  char *        pSend = (char *)HC05_AS_t.pSetupSendBuff;
//  char *        pRcv = (char *)HC05_AS_t.pSetupRcvBuff;

  HC05_AS_t.eState = HC05_STATE_SETUP;
  //Init setup buffer
  vDEBUG_ASSERT("eBluetooth_HC05_setup setup buff not null!", (HC05_AS_t.pSetupSendBuff == NULL)&(HC05_AS_t.pSetupRcvBuff == NULL));
  HC05_AS_t.pSetupRcvBuff = calloc(SETUP_RCV_BUFF_SIZE, sizeof(uint8_t));
  HC05_AS_t.pSetupSendBuff = calloc(SETUP_SEND_BUFF_SIZE, sizeof(uint8_t));
  vDEBUG_ASSERT("eBluetooth_HC05_setup setup buff calloc fail", (HC05_AS_t.pSetupSendBuff != NULL)&(HC05_AS_t.pSetupRcvBuff != NULL));

  eBSP_BT_WAKE();
//  eBSP_BT_SLEEP();
  eBSP_BT_POWER_ENABLE();

  vDEBUG_ASSERT("",false);
  while(eEC == ER_FAIL)
  {
    //kick off data reception from the bluetooth module
    BT_Rcv_t.uiLen = 1;
    BT_Rcv_t.pBuff = &uiRcv;
    eEC = eBSP_BT_INTF_RCV(&BT_Rcv_t);

    memcpy(HC05_AS_t.pSetupSendBuff, AT_CMD_TEST, strlen(AT_CMD_TEST));
    BT_Send_t.pBuff = (uint8_t *)HC05_AS_t.pSetupSendBuff;
    BT_Send_t.uiLen = strlen((char *)HC05_AS_t.pSetupSendBuff);
    uiWait = 0;
    eEC = eBSP_BT_INTF_SEND(&BT_Send_t);

    while(1)
    {
      if(strstr((char *)HC05_AS_t.pSetupRcvBuff, "OK\r\n") != NULL)
      {
        eEC = ER_OK;
        break;
      }
      else
      {
        eOSAL_delay(1, NULL);
        uiWait++;
        if(uiWait > 100)
        {
          eEC = ER_TIMEOUT;
          break;
        }
      }
    }

    //check if the HC-05 responded
    if(eEC == ER_TIMEOUT)
    {
      //the HC-05 failed to respond, cycle through the supported baud rates to determine the modules current baud
      bUartBaudSetNeeded = true;
      //clear the receive buffer
      memset(HC05_AS_t.pSetupRcvBuff, 0x00, SETUP_RCV_BUFF_SIZE);
      HC05_AS_t.uiSetupBuffIndex = 0;

      memset(&BT_Config_t, 0x00, sizeof(UART_Config_t));
      BT_Config_t.eDataBits = DATA_BITS_8;
      BT_Config_t.eParity   = PARITY_NONE;
      BT_Config_t.eStopBits = STOP_BITS_1;
      BT_Config_t.eControl  = FLOW_CONTROL_NONE;
      BT_Config_t.eMode     = MODE_TX_RX;

      if(uiBaudIndex < uiNumSupportedBauds)
      {
        BT_Config_t.uiBaud = uiSupportedBauds[uiBaudIndex];
        uiBaudIndex++;

        //set error code to allow baud loop to try again
        eEC = ER_FAIL;
      }
      else
      {
        eEC = ER_SPEED;
      }

      BT_Config_t.uiTimeout = 3000;
      BT_Config_t.vUART_ISR = &vBluetooth_HC05_intf_isr_callback;
      eBSP_BT_INTF_CONFIG(&BT_Config_t);
    }
  }

  //check if the HC-05's baud current baud rate was found and if it needs to be set to the project
  if((eEC == ER_OK)&(bUartBaudSetNeeded == true))
  {
    BT_Send_t.uiLen = snprintf((char *)HC05_AS_t.pSetupSendBuff, SETUP_SEND_BUFF_SIZE, AT_CMD_SET_UART, AT_CMD_SET_UART_BAUD_921K, AT_CMD_SET_UART_STOP_BIT_1,AT_CMD_SET_UART_PARITY_NONE);
    BT_Send_t.pBuff = (uint8_t *)HC05_AS_t.pSetupSendBuff;
    eEC = eBSP_BT_INTF_SEND(&BT_Send_t);

    memset(&BT_Config_t, 0x00, sizeof(UART_Config_t));
    BT_Config_t.eDataBits = DATA_BITS_8;
    BT_Config_t.eParity   = PARITY_NONE;
    BT_Config_t.eStopBits = STOP_BITS_1;
    BT_Config_t.eControl  = FLOW_CONTROL_NONE;
    BT_Config_t.eMode     = MODE_TX_RX;
    BT_Config_t.uiBaud    = HC05_BAUD_921K;
    BT_Config_t.uiTimeout = 3000;
    BT_Config_t.vUART_ISR = &vBluetooth_HC05_intf_isr_callback;
    eEC = eBSP_BT_INTF_CONFIG(&BT_Config_t);
    if(eEC == ER_OK)
    {
      memset(HC05_AS_t.pSetupRcvBuff, 0x00, SETUP_RCV_BUFF_SIZE);

      memcpy(HC05_AS_t.pSetupSendBuff, AT_CMD_TEST, strlen(AT_CMD_TEST));
      BT_Send_t.pBuff = (uint8_t *)HC05_AS_t.pSetupSendBuff;
      BT_Send_t.uiLen = strlen((char *)HC05_AS_t.pSetupSendBuff);
      uiWait = 0;
      eEC = eBSP_BT_INTF_SEND(&BT_Send_t);

      while(1)
      {
        if(strstr((char *)HC05_AS_t.pSetupRcvBuff, "OK\r\n") != NULL)
        {
          eEC = ER_OK;
          break;
        }
        else
        {
          eOSAL_delay(1, NULL);
          uiWait++;
          if(uiWait > 100)
          {
            eEC = ER_TIMEOUT;
            break;
          }
        }
      }
    }
  }

  free(HC05_AS_t.pSetupRcvBuff);
  HC05_AS_t.pSetupRcvBuff = NULL;
  free(HC05_AS_t.pSetupSendBuff);
  HC05_AS_t.pSetupSendBuff = NULL;

  return eEC;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

void vBluetooth_HC05_intf_isr_callback(void)
{
  BSP_BT_Rcv_t  BT_Rcv_t;
  if(HC05_AS_t.eState == HC05_STATE_SETUP)
  {
    if(HC05_AS_t.pSetupRcvBuff != NULL)
    {
      HC05_AS_t.pSetupRcvBuff[HC05_AS_t.uiSetupBuffIndex] = uiRcv;
      HC05_AS_t.uiSetupBuffIndex++;
    }
    uiRcv = 0;
    BT_Rcv_t.uiLen = 1;
    BT_Rcv_t.pBuff = &uiRcv;
  }
  else
  {
    vDEBUG_ASSERT("vBluetooth_HC05_intf_isr_callback invalid state receive", false);
  }

  eBSP_BT_INTF_RCV(&BT_Rcv_t);
  return;
}

ERROR_CODE eBluetooth_HC05_setup(void)
{
  ERROR_CODE eEC = ER_FAIL;
  BSP_BT_Send_t BT_Send_t;
  BSP_BT_Rcv_t  BT_Rcv_t;
  static char at[] = "AT\r\n";
//  static char at_reset[] = "AT+RESET\r\n";
  UART_Config_t BT_Config_t;
  uint32_t uiWait = 0;

  HC05_AS_t.eState = HC05_STATE_SETUP;

  eEC = eHC05_Check_Set_Baud();

  if(eEC == ER_OK)
  {

  }
//  BT_Send_t.pBuff = (uint8_t *)at_reset;
//  BT_Send_t.uiLen = strlen(at_reset);
//  eEC = eBSP_BT_INTF_SEND(&BT_Send_t);

  return eEC;
}

#endif //(PROJ_CONFIG_PLATFORM_BT == PROJ_CONFIG_PLATFORM_BT_HC05)
#endif //__BLUETOOTH_HC05_C__
