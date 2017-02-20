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

#define CONFIG_MODE_TIMEOUT   1000

#define SETUP_RCV_BUFF_SIZE   512
#define SETUP_SEND_BUFF_SIZE  128

#define HC05_BAUD_38K         38400
#define HC05_BAUD_57K         57600
#define HC05_BAUD_115K        115200
#define HC05_BAUD_230K        230400
#define HC05_BAUD_460K        460800
#define HC05_BAUD_921K        921600
#define HC05_BAUD_1382K       1382400  //not supported on stm32f4discovery UART4

#define GET_OK_WAIT_TIME_MS   200

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static volatile uint8_t u8Rcv = 0;
void(* vRcvByte)(volatile uint8_t * pBuff);

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef enum HC05_STATE
{
  HC05_STATE_NONE,
  HC05_STATE_POWER_RESET,
  HC05_STATE_SET_BAUD,
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
  uint8_t      u8BtAddress[6];
  char         cBtAddress[18];
}Bluetooth_HC05_Activity_State_t, * pBluetooth_HC05_Activity_State;

static Bluetooth_HC05_Activity_State_t HC05_AS_t =
{
  .bModuleConfigured = false,
  .eState            = HC05_STATE_NONE,
  .pSetupRcvBuff     = NULL,
  .pSetupSendBuff    = NULL,
  .uiSetupBuffIndex  = 0,
  .u8BtAddress       = {0x00,0x00,0x00,0x00,0x00,0x00},
  .cBtAddress        = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

static ERROR_CODE eHC05_Config_Mode(void);
static ERROR_CODE eHC05_Data_Mode(void);
static ERROR_CODE eHC05_Check_Set_Baud(void);
static ERROR_CODE eHC05_Set_Default_Baud(void);
static ERROR_CODE eHC05_Check_Settings(void);
static ERROR_CODE eHC05_Set_Settings(void);
static ERROR_CODE eHC05_Get_OK(void);
static ERROR_CODE eHC05_Send(uint8_t * pBuff, uint32_t u32BuffLen);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* name: eHC05_Config_Mode
* description:
*   Puts the HC-05 module in configuration mode.
* parameters:
*   none
*
* return value description:
*   ERROR_CODE - ER_OK: Config mode successful.
*              - ER_FAIL: Failed to put the HC-05 in config mode.
******************************************************************************/
static ERROR_CODE eHC05_Config_Mode(void)
{
  ERROR_CODE eEC = ER_FAIL;

  eEC = eBSP_BT_POWER_DISABLE();
  if(eEC == ER_OK)
  {
    eEC = eBSP_BT_CONFIG_MODE_ENABLE();
    if(eEC == ER_OK)
    {
      eEC = eOSAL_delay(10, NULL);
      if(eEC == ER_OK)
      {
        if(eEC == ER_OK)
        {
          eBSP_BT_POWER_ENABLE();
          eOSAL_delay(400, NULL);
        }
      }
    }
  }
  return eEC;
}

/******************************************************************************
* name: eHC05_Data_Mode
* description:
*   Puts the HC-05 module in data mode.
* parameters:
*   none
*
* return value description:
*   ERROR_CODE - ER_OK: Data mode successful.
*              - ER_FAIL: Failed to put the HC-05 in Data mode.
******************************************************************************/
static ERROR_CODE eHC05_Data_Mode(void)
{
  ERROR_CODE eEC = ER_FAIL;

  eEC = eBSP_BT_POWER_DISABLE();
  if(eEC == ER_OK)
  {
    eEC = eBSP_BT_CONFIG_MODE_DISABLE();
    if(eEC == ER_OK)
    {
      eEC = eOSAL_delay(10, NULL);
      if(eEC == ER_OK)
      {
        eBSP_BT_POWER_ENABLE();
        eOSAL_delay(400, NULL);
      }
    }
  }
  return eEC;
}

/******************************************************************************
* name: eHC05_Check_Set_Baud
* description:
*   Checks the baud rate of the HC05 bluetooth module to determine if it is set
*   to the right speed. If it is not set to the project defined baud rate the
*   function will then attempt to set the baud rate. The HC-05 will not change
*   its baud rate to the set baud rate until after the HC-05 is set to data
*   mode.
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
  uint32_t      uiSupportedBauds[] = {HC05_BAUD_38K, HC05_BAUD_57K, HC05_BAUD_115K, HC05_BAUD_230K, HC05_BAUD_460K, HC05_BAUD_921K};
  uint8_t       uiNumSupportedBauds = sizeof(uiSupportedBauds)/sizeof(uint32_t);
  uint8_t       uiBaudIndex = 0;
  bool          bUartBaudSetNeeded = false;
//  char *        pSend = (char *)HC05_AS_t.pSetupSendBuff;
//  char *        pRcv = (char *)HC05_AS_t.pSetupRcvBuff;

  //Init setup buffer
  vDEBUG_ASSERT("eBluetooth_HC05_setup setup buff not null!", (HC05_AS_t.pSetupSendBuff == NULL)&(HC05_AS_t.pSetupRcvBuff == NULL));
  HC05_AS_t.pSetupRcvBuff = calloc(SETUP_RCV_BUFF_SIZE, sizeof(uint8_t));
  HC05_AS_t.pSetupSendBuff = calloc(SETUP_SEND_BUFF_SIZE, sizeof(uint8_t));
  vDEBUG_ASSERT("eBluetooth_HC05_setup setup buff calloc fail", (HC05_AS_t.pSetupSendBuff != NULL)&(HC05_AS_t.pSetupRcvBuff != NULL));

  //Set module to configuration mode
  eEC = eHC05_Config_Mode();

  if(eEC == ER_OK)
  {
    //Loop until the current baud rate of the module is determined
    //
    eEC = ER_FAIL;
    while(eEC == ER_FAIL)
    {
      memcpy(HC05_AS_t.pSetupSendBuff, AT_CMD_TEST, strlen(AT_CMD_TEST));
      BT_Send_t.pBuff = (uint8_t *)HC05_AS_t.pSetupSendBuff;
      BT_Send_t.uiLen = strlen((char *)HC05_AS_t.pSetupSendBuff);
      //Send the AT test command.
      eEC = eHC05_Send(BT_Send_t.pBuff, BT_Send_t.uiLen);

      //Get the AT test command response.
      eEC = eHC05_Get_OK();

      //check if the HC-05 responded to the AT test command at the current baud rate or if the baud needs to change
      //
      if(eEC == ER_TIMEOUT)
      {
        //the HC-05 failed to respond, cycle through the supported baud rates to determine the modules current baud
        bUartBaudSetNeeded = true;
        //clear the receive buffer
        memset(HC05_AS_t.pSetupRcvBuff, 0x00, SETUP_RCV_BUFF_SIZE);
        HC05_AS_t.uiSetupBuffIndex = 0;

        //config uart to a new baud rate.
        if(uiBaudIndex < uiNumSupportedBauds)
        {
          memset(&BT_Config_t, 0x00, sizeof(UART_Config_t));
          BT_Config_t.eDataBits = DATA_BITS_8;
          BT_Config_t.eParity   = PARITY_NONE;
          BT_Config_t.eStopBits = STOP_BITS_1;
          BT_Config_t.eControl  = FLOW_CONTROL_NONE;
          BT_Config_t.eMode     = MODE_TX_RX;
          BT_Config_t.uiBaud = uiSupportedBauds[uiBaudIndex++];
          BT_Config_t.uiTimeout = 3000;
          BT_Config_t.vUART_ISR = &vBluetooth_HC05_intf_isr_callback;
          eEC = eBSP_BT_INTF_CONFIG(&BT_Config_t);

          if(eEC == ER_OK)
          {
            eOSAL_delay(100, NULL);
            BT_Rcv_t.uiLen = 1;
            BT_Rcv_t.pBuff = (uint8_t *)&u8Rcv;
            //kick off UART ISR reception.
            eEC = eBSP_BT_INTF_RCV(&BT_Rcv_t);
            if(eEC == ER_OK)
            {
              //set error code to fail to allow the baud rate determination routine to run again
              eEC = ER_FAIL;
            }
          }
        }
        else
        {
          //Exhausted the list of supported baud rates
          eEC = ER_SPEED;
        }
      }
    }
  }

  //check if the HC-05's current baud rate was found and if it needs to be set to the project baud rate
  if((eEC == ER_OK)&(bUartBaudSetNeeded == true))
  {
    memset(HC05_AS_t.pSetupRcvBuff, 0x00, SETUP_RCV_BUFF_SIZE);
    memset(HC05_AS_t.pSetupSendBuff, 0x00, SETUP_SEND_BUFF_SIZE);
    HC05_AS_t.uiSetupBuffIndex = 0;

    //build the set uart command
    BT_Send_t.uiLen = snprintf((char *)HC05_AS_t.pSetupSendBuff, SETUP_SEND_BUFF_SIZE,
                                                                 AT_CMD_HC05_SET_UART,
                                                                 AT_CMD_HC05_SET_UART_BAUD_921K,//AT_CMD_HC05_SET_UART_BAUD_38K,//
                                                                 AT_CMD_HC05_SET_UART_STOP_BIT_1,
                                                                 AT_CMD_HC05_SET_UART_PARITY_NONE);
    BT_Send_t.pBuff = (uint8_t *)HC05_AS_t.pSetupSendBuff;

//    memcpy(HC05_AS_t.pSetupSendBuff, "\r\n", 2);
//    BT_Send_t.uiLen = 2;
//    BT_Send_t.pBuff = (uint8_t *)HC05_AS_t.pSetupSendBuff;

//    memcpy(HC05_AS_t.pSetupSendBuff, AT_CMD_TEST, strlen(AT_CMD_TEST));
//    BT_Send_t.pBuff = (uint8_t *)HC05_AS_t.pSetupSendBuff;
//    BT_Send_t.uiLen = strlen((char *)HC05_AS_t.pSetupSendBuff);

    eEC = eHC05_Send(BT_Send_t.pBuff, BT_Send_t.uiLen);

    //Get the okay response from the module
    eEC = eHC05_Get_OK();

//    //Verify the new baud rate was set.
//    if(eEC == ER_OK)
//    {
//
//      //Set the baud rate to the project default.
//      //
//      memset(&BT_Config_t, 0x00, sizeof(UART_Config_t));
//      BT_Config_t.eDataBits = DATA_BITS_8;
//      BT_Config_t.eParity   = PARITY_NONE;
//      BT_Config_t.eStopBits = STOP_BITS_1;
//      BT_Config_t.eControl  = FLOW_CONTROL_NONE;
//      BT_Config_t.eMode     = MODE_TX_RX;
//      BT_Config_t.uiBaud    = HC05_BAUD_921K;//HC05_BAUD_38K;//
//      BT_Config_t.uiTimeout = 3000;
//      BT_Config_t.vUART_ISR = &vBluetooth_HC05_intf_isr_callback;
//      eEC = eBSP_BT_INTF_CONFIG(&BT_Config_t);
//
//      //Ensure the module is running at the project default baud.
//      //
//      if(eEC == ER_OK)
//      {
//        eEC = eHC05_Data_Mode();
//        if(eEC == ER_OK)
//        {
//          eEC = eHC05_Config_Mode();
//          if(eEC == ER_OK)
//          {
//            BT_Rcv_t.uiLen = 1;
//            BT_Rcv_t.pBuff = (uint8_t *)&u8Rcv;
//            memset(HC05_AS_t.pSetupRcvBuff, 0x00, SETUP_RCV_BUFF_SIZE);
//            eOSAL_delay(400, NULL);
//            eEC = eBSP_BT_INTF_RCV(&BT_Rcv_t);
//
//            if(eEC == ER_OK)
//            {
//              memset(HC05_AS_t.pSetupSendBuff, 0x00, SETUP_SEND_BUFF_SIZE);
//              memcpy(HC05_AS_t.pSetupSendBuff, AT_CMD_TEST, strlen(AT_CMD_TEST));
//              HC05_AS_t.uiSetupBuffIndex = 0;
//              BT_Send_t.pBuff = (uint8_t *)HC05_AS_t.pSetupSendBuff;
//              BT_Send_t.uiLen = strlen((char *)HC05_AS_t.pSetupSendBuff);
//              uiWait = 0;
//              eEC = eBSP_BT_INTF_SEND(&BT_Send_t);
//
//              //Get the okay response from the module
//              eEC = eHC05_Get_OK();
//            }
//          }
//        }
//      }
//    }
  }

  free(HC05_AS_t.pSetupRcvBuff);
  HC05_AS_t.pSetupRcvBuff = NULL;
  free(HC05_AS_t.pSetupSendBuff);
  HC05_AS_t.pSetupSendBuff = NULL;

  return eEC;
}

static ERROR_CODE eHC05_Set_Default_Baud(void)
{
  ERROR_CODE eEC = ER_FAIL;
  UART_Config_t BT_Config_t;

  memset(&BT_Config_t, 0x00, sizeof(UART_Config_t));
  BT_Config_t.eDataBits = DATA_BITS_8;
  BT_Config_t.eParity   = PARITY_NONE;
  BT_Config_t.eStopBits = STOP_BITS_1;
  BT_Config_t.eControl  = FLOW_CONTROL_NONE;
  BT_Config_t.eMode     = MODE_TX_RX;
  BT_Config_t.uiBaud = HC05_BAUD_921K;
  BT_Config_t.uiTimeout = 3000;
  BT_Config_t.vUART_ISR = &vBluetooth_HC05_intf_isr_callback;
  eEC = eBSP_BT_INTF_CONFIG(&BT_Config_t);

  return eEC;
}

/******************************************************************************
* name: eHC05_Check_Settings
* description:
*   Checks the baud rate of the HC05 bluetooth module to determine if it is set
*   to the right speed. If it is not set to the project defined baud rate the
*   function will then attempt to set the baud rate.
* parameters:
*   none
*
* return value description:
*   ERROR_CODE - ER_OK: Baud successfully checked and or set.
*              - ER_FAIL: Baud failed to be set.
*              - ER_SPEED: Was unable to determine HC05 baud speed rate with
*                          list of supported bauds.
******************************************************************************/
static ERROR_CODE eHC05_Check_Settings(void)
{
  ERROR_CODE eEC = ER_FAIL;
  BSP_BT_Send_t BT_Send_t;
  char * pChar;
  int index = 0;

  HC05_AS_t.pSetupRcvBuff = calloc(SETUP_RCV_BUFF_SIZE, sizeof(uint8_t));
  HC05_AS_t.pSetupSendBuff = calloc(SETUP_SEND_BUFF_SIZE, sizeof(uint8_t));
  HC05_AS_t.uiSetupBuffIndex = 0;

//  AT_CMD_GET_ADDR
  BT_Send_t.uiLen = strlen(AT_CMD_GET_ADDR);
  BT_Send_t.pBuff = (uint8_t *)HC05_AS_t.pSetupSendBuff;
  memcpy(BT_Send_t.pBuff, AT_CMD_GET_ADDR, BT_Send_t.uiLen);

  eEC = eHC05_Send(BT_Send_t.pBuff, BT_Send_t.uiLen);
  if(eEC == ER_OK)
  {
    eEC = eHC05_Get_OK();
    if(eEC == ER_OK)
    {
      pChar = strchr((char *)HC05_AS_t.pSetupRcvBuff, ':');
      pChar++;
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = ':';
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = ':';
      pChar++;
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = ':';
      pChar++;
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = ':';
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = ':';
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = *pChar++;
      HC05_AS_t.cBtAddress[index++] = ':';
    }
  }


  free(HC05_AS_t.pSetupRcvBuff);
  HC05_AS_t.pSetupRcvBuff = NULL;
  free(HC05_AS_t.pSetupSendBuff);
  HC05_AS_t.pSetupSendBuff = NULL;

  return eEC;
}

static ERROR_CODE eHC05_Set_Settings(void)
{
  ERROR_CODE eEC = ER_FAIL;

  //todo: determine settings to set

  //
  //  BT_Send_t.pBuff = (uint8_t *)at_reset;
  //  BT_Send_t.uiLen = strlen(at_reset);
  //  eEC = eBSP_BT_INTF_SEND(&BT_Send_t);

  return eEC;
}

static ERROR_CODE eHC05_Get_OK(void)
{
  ERROR_CODE eEC = ER_FAIL;
  uint32_t u32Wait = 0;
  uint32_t u32PreviousLen = 0;
  uint32_t u32Len = 0;

  //wait to receive the response from the module
  while(1)
  {
    if(strstr((char *)HC05_AS_t.pSetupRcvBuff, "OK\r\n") != NULL)
    {
      //Received okay response to setting the new BAUD rate.
      eEC = ER_OK;
      break;
    }
    else
    {
      //check if any characters were received
      u32Len = strlen((char *)HC05_AS_t.pSetupRcvBuff);
      if(u32Len > u32PreviousLen)
      {
        //if any new characters were received reset the wait time and set the new length.
        u32Wait = 0;
        u32PreviousLen = u32Len;
      }

      eOSAL_delay(1, NULL);
      u32Wait++;
      //check if the wait time was exceeded
      if(u32Wait > GET_OK_WAIT_TIME_MS)
      {
        //Never received the okay response to setting the new baud rate.
        eEC = ER_TIMEOUT;
        //assert received length is 0 to catch any partial reception.
        vDEBUG_ASSERT("eHC05_Get_OK received partial OK", u32Len == 0);
        break;
      }
    }
  }

  return eEC;
}

static ERROR_CODE eHC05_Send(uint8_t * pBuff, uint32_t u32BuffLen)
{
  ERROR_CODE eEC = ER_FAIL;
  BSP_BT_Send_t Send_t;

  Send_t.pBuff = pBuff;
  Send_t.uiLen = u32BuffLen;

  while(1)
  {
    eEC =  eBSP_BT_INTF_SEND(&Send_t);

    if(eEC == ER_BUSY)
    {
      eOSAL_delay(1, NULL);
    }
    else
    {
      break;
    }
  }

  return eEC;
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

void vBluetooth_HC05_intf_isr_callback(void)
{
  BSP_BT_Rcv_t  BT_Rcv_t;
  if(HC05_AS_t.eState == HC05_STATE_SET_BAUD)
  {
    if(HC05_AS_t.pSetupRcvBuff != NULL)
    {
      HC05_AS_t.pSetupRcvBuff[HC05_AS_t.uiSetupBuffIndex] = u8Rcv;
      HC05_AS_t.uiSetupBuffIndex++;
    }
    u8Rcv = 0;
    BT_Rcv_t.uiLen = 1;
    BT_Rcv_t.pBuff = &u8Rcv;
  }
  else if(HC05_AS_t.eState == HC05_STATE_SETUP)
  {
    if(HC05_AS_t.pSetupRcvBuff != NULL)
    {
      HC05_AS_t.pSetupRcvBuff[HC05_AS_t.uiSetupBuffIndex] = u8Rcv;
      HC05_AS_t.uiSetupBuffIndex++;
    }
    u8Rcv = 0;
    BT_Rcv_t.uiLen = 1;
    BT_Rcv_t.pBuff = &u8Rcv;
  }
  else
  {
    vRcvByte(&u8Rcv);
  }

  eBSP_BT_INTF_RCV_IT(&BT_Rcv_t);
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

  //kick off data reception from the bluetooth module
  BT_Rcv_t.uiLen = 1;
  BT_Rcv_t.pBuff = &u8Rcv;
  eEC = eBSP_BT_INTF_RCV(&BT_Rcv_t);
  if(eEC == ER_OK)
  {
    HC05_AS_t.eState = HC05_STATE_SET_BAUD;

    eEC = eHC05_Check_Set_Baud();

    if(eEC == ER_OK)
    {
      HC05_AS_t.eState = HC05_STATE_SETUP;

      //Check settings
      eEC = eHC05_Check_Settings();

      if(eEC != ER_OK)
      {
        //set settings
        eEC = eHC05_Set_Settings();
        vDEBUG_ASSERT("eBluetooth_HC05_setup failed to set uart defaults", (eEC == ER_OK));
      }

      if(eEC == ER_OK)
      {
        eHC05_Set_Default_Baud();
        //todo: set the baud rate to project settings before leaving setup
        HC05_AS_t.eState = HC05_STATE_READY;
        eHC05_Data_Mode();
      }
    }
  }

  return eEC;
}

ERROR_CODE eBluetooth_HC05_Register_Receive(pHC05_Register_Receive pRegRcv)
{
  ERROR_CODE eEC = ER_FAIL;

  vRcvByte = pRegRcv->vRcvByte;

  eEC = ER_OK;

  return eEC;
}

#endif //(PROJ_CONFIG_PLATFORM_BT == PROJ_CONFIG_PLATFORM_BT_HC05)
#endif //__BLUETOOTH_HC05_C__
