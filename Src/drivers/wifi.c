/******************************************************************************
*
* wifi.c - Source code for the wifi driver.
* Copyright (c) notice
*
******************************************************************************/
#ifndef __WIFI_C__
#define __WIFI_C__
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
#if (PROJ_CONFIG_USE_DRVR_WIFI == 0)
//wifi not implemented
#else

#include "utils_inc/error_codes.h"
#include "ThirdParty_inc/osal.h"
#include "utils_inc/util_debug.h"

#include "drivers_inc/wifi.h"
#include "drivers_inc/nvram.h"
#include "drivers_inc/AT_commands.h"
#include "board.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define TASK_WIFI_PRIORITY       2
#define TASK_WIFI_STACK_SIZE 4096/sizeof(uint32_t) //size in bytes divided by RTOS stack type, portSTACK_TYPE

#define WIFI_SEND_BUF_LEN    256
#define WIFI_RCV_BUF_LEN    256

#define WIFI_SEND_ASSERT()  vDEBUG_ASSERT("Wifi send fail", eEC == ER_OK)

#define WIFI_XMIT_1S_TIMEOUT  10

#define GET_AP_ID()       "NETGEAR76"
#define GET_AP_PW()       "modernroad450"

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

volatile bool bRcv = false;

char cWifi_Task_Name[] = "WIFI";

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* enums ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

//wifi message ID's
typedef enum WIFI_MSG_ID
{
  WIFI_MSG_NONE,
  WIFI_MSG_TEST,
  WIFI_MSG_AP_CONNECT,
  WIFI_MSG_LIMIT,
}WIFI_MSG_ID;

typedef enum WIFI_STATE
{
  WIFI_STATE_NONE,
  WIFI_STATE_INIT,
  WIFI_STATE_INIT_GET_READY,
  WIFI_STATE_INIT_ECHO,
  WIFI_STATE_INIT_MODE,
  WIFI_STATE_LIMIT,

}WIFI_STATE, * pWIFI_STATE;


/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//tWifi activity state for command, control and status of the task
typedef struct Wifi_Activity_State_t
{
  bool         bIs_Wifi_Ready;
  bool         bIs_Wifi_Task_Ready;
  WIFI_STATE   eSTATE;
  uint32_t     uiSendLen;
  uint8_t      uiSendBuff[WIFI_SEND_BUF_LEN];
  uint32_t     uiRcvLen;
  uint8_t      uiISR_RcvBuff;
  uint8_t *    pRcvBuff;
  bool         bISR_Rcv;
  uint8_t      uiRcvBuff[WIFI_RCV_BUF_LEN];
  uint8_t      uiIP_Address[4];
  char         cIP_Address[16];
  uint8_t      uiMAC_Address[6];
  char         cMAC_Address[18];
  uint32_t     uiWifi_NVID;
}Wifi_Activity_State_t, * pWifi_Activity_State;

static Wifi_Activity_State_t Wifi_AS_t =
{
  .bIs_Wifi_Ready      = false,
  .bIs_Wifi_Task_Ready = false,
  .eSTATE              = WIFI_STATE_NONE,
  .uiSendLen           = 0,
  .uiSendBuff          =
  {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  },
  .uiRcvLen            = 0,
  .uiISR_RcvBuff       = 0,
  .pRcvBuff            = Wifi_AS_t.uiRcvBuff,
  .bISR_Rcv            = false,
  .uiRcvBuff           =
  {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  },
  .uiIP_Address        =
  {
    0,0,0,0,
  },
  .cIP_Address         =
  {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  },
  .uiMAC_Address       =
  {
    0,0,0,0,0,0,
  },
  .cMAC_Address        =
  {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  },
  0x74BEAD94,//uint32_t uiWifi_NVID;
};

typedef struct tWifi_Message_Struct
{
    WIFI_MSG_ID eMSG;
}tWifi_Message_Struct;

typedef struct Wifi_Send_t
{
  BSP_Wifi_Transmit_t BSP_Send_t;
  bool                bAT_OK_Resp;
  int32_t             iTimeout;
}Wifi_Send_t, * pWifi_Send;

typedef struct Wifi_Receive_t
{
  BSP_Wifi_Receive_t tBSP_Receive;
  int32_t           iTimeout;
  bool              bReceive_Until_OK;
}Wifi_Receive_t, *pWifi_Receive;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eWifi_Setup               (void);
ERROR_CODE eWifi_Default_Settings    (void);
ERROR_CODE eWifi_get_ready           (void);
ERROR_CODE eWifi_clear_intf          (void);
ERROR_CODE eWifi_disable_echo        (void);
ERROR_CODE eWifi_rcv_OK              (void);
ERROR_CODE eWifi_AP_connect          (void);
ERROR_CODE eWifi_send                (Wifi_Send_t * pParam);
ERROR_CODE eWifi_AT_Send             (pWifi_Send pSend);
ERROR_CODE eWIFI_Rcv_Init            (void);
ERROR_CODE eWifi_rcv                 (pWifi_Receive pParam);
void       vWifi_Intf_isr_callback   (void);
void       vWifi_Driver_Rcv_Task     (void * pvParameters);
void       vWifi_Driver_Task         (void * pvParameters);

/******************************************************************************
* private functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name: eWifi_Setup
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eWifi_Setup(void)
{
  ERROR_CODE eEC = ER_FAIL;
  Wifi_Send_t Send_t;
  int index;

  Wifi_AS_t.eSTATE = WIFI_STATE_INIT;
  eWIFI_Rcv_Init();

  Wifi_AS_t.eSTATE = WIFI_STATE_INIT_GET_READY;
  eEC = eWifi_get_ready();
  if(eEC == ER_OK)
  {
    Wifi_AS_t.eSTATE = WIFI_STATE_INIT_ECHO;
    //Disable echo
    eEC = eWifi_disable_echo();
  }

  if(eEC == ER_OK)
  {
    Wifi_AS_t.eSTATE = WIFI_STATE_INIT_MODE;
    //Set wifi mode
    Send_t.BSP_Send_t.pBuff = Wifi_AS_t.uiSendBuff;
    index = Send_t.BSP_Send_t.uiBuff_Len = snprintf((char *)Send_t.BSP_Send_t.pBuff, WIFI_SEND_BUF_LEN, AT_CMD_SET_WIFI_MODE, AT_CMD_WIFI_MODE);
    Send_t.BSP_Send_t.pBuff[index] = 0;
    Send_t.bAT_OK_Resp = true;

//    eWifi_rcv_OK();
    eEC = eWifi_send(&Send_t);
    WIFI_SEND_ASSERT();

  }

//  todo: get mac address

  return eEC;
}

/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name: eWifi_Default_Settings
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eWifi_Default_Settings(void)
{
  ERROR_CODE eEC = ER_FAIL;

  //todo: code goes here

  return eEC;
}

/******************************************************************************
* name: eWifi_get_ready
* description: gets the ready status from the wifi module. call any time after
*              resetting the module to default settings
* param description: none
* return value description: ERROR_CODE - ER_OK: wifi module ready for instructions
*                                      - ER_FAIL: failed to receive any data from wifi module
*                                      - ER_TIMEOUT: Communications with the wifi module timed out
******************************************************************************/
ERROR_CODE eWifi_get_ready(void)
{
  ERROR_CODE eEC = ER_FAIL;
//  char cReady[8];
  uint32_t uiCurrent_Tick;
  uint32_t uiStarted_Tick;
//  char cStart_Garbage = 0;
//  char cHello[] = "valhalla is awesome!";

  Wifi_Receive_t  Rcv_t;
//  tWifi_Send     Send_t;

//  memset(cReady, 0x00, 8);

  Rcv_t.tBSP_Receive.uiBuff_Len = 7;
  Rcv_t.tBSP_Receive.pBuff = (uint8_t *)&Wifi_AS_t.uiRcvBuff[1];
  Rcv_t.tBSP_Receive.bBlocking = false;
//  Rcv_t.iTimeout = WIFI_XMIT_1S_TIMEOUT;
  bRcv = false;
  Wifi_AS_t.uiRcvLen = 7;
//  eEC = eWifi_rcv(&Rcv_t);

  eBSP_Wifi_En_Clr();
  eOSAL_delay(50, NULL);
  eBSP_Wifi_En_Set();
  eOSAL_delay(50, NULL);

  eWifi_clear_intf();

  eBSP_Wifi_Rst_Clr();
  eOSAL_delay(50, NULL);
  eEC = eWifi_rcv(&Rcv_t);
  eBSP_Wifi_Rst_Set();

//  Rcv_t.tBSP_Receive.uiBuff_Len = 1;
//  Rcv_t.tBSP_Receive.pBuff = (uint8_t *)&cReady[0];
//  Rcv_t.iTimeout = 1;
//  eEC = eWifi_rcv(&Rcv_t);
//  while(bRcv == false);

//  Rcv_t.tBSP_Receive.uiBuff_Len = 6;
//  Rcv_t.tBSP_Receive.pBuff = (uint8_t *)&cReady[1];
//  Rcv_t.iTimeout = WIFI_XMIT_1S_TIMEOUT;
//  eEC = eWifi_rcv(&Rcv_t);

//  while(bRcv == false);

//  Rcv_t.tBSP_Receive.uiBuff_Len = 1;
//  Rcv_t.tBSP_Receive.pBuff = (uint8_t *)&Wifi_AS_t.uiRcvBuff[7];

//  eBSP_Get_Current_ms_count(&uiStarted_Tick);
//  do
//  {
////    bRcv = false;
////    eEC = eWifi_rcv(&Rcv_t);
////    while(bRcv == false);
////    if(eEC == ER_FAIL)
////    {
////      break;
////    }
////    else if(eEC == ER_OK)
////    {
////      eBSP_Get_Current_ms_count(&uiStarted_Tick);
////    }
////
////    memcpy(&cReady[0], &cReady[1], 7);
////    cReady[7] = 0;
////
////    eBSP_Get_Current_ms_count(&uiCurrent_Tick);
////    if((uiCurrent_Tick - uiStarted_Tick) > 5000)
////    {
////      eEC = ER_TIMEOUT;
////
////      eWifi_get_ready();
////
////      break;
////    }
//
//    bRcv = false;
//    eEC = eWifi_rcv(&Rcv_t);
//    while(bRcv == false);
//    if(eEC == ER_OK)
//    {
//      eBSP_Get_Current_ms_count(&uiStarted_Tick);
////      memcpy(&cReady[0], &cReady[1], 7);
////      cReady[7] = 0;
//    }
//
//    eBSP_Get_Current_ms_count(&uiCurrent_Tick);
//    if((uiCurrent_Tick - uiStarted_Tick) > 5000)
//    {
//      eEC = ER_TIMEOUT;
//
//      break;
//    }
//  }while(strstr(cReady, "ready\r\n") == NULL);

  eBSP_Get_Current_ms_count(&uiStarted_Tick);
  while(1)
  {
    if(strstr((char *)Wifi_AS_t.uiRcvBuff, "ready\r\n") == NULL)
    {
      eOSAL_delay(1,NULL);
      eBSP_Get_Current_ms_count(&uiCurrent_Tick);
      if((uiCurrent_Tick - uiStarted_Tick) > 5000)
      {
        eEC = ER_TIMEOUT;

        break;
      }
    }
    else
    {
      eEC = ER_OK;

      break;
    }

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
ERROR_CODE eWifi_clear_intf(void)
{
  ERROR_CODE eEC = ER_FAIL;
  uint8_t uiData = 0;
  Wifi_Receive_t  Rcv_t;

  Rcv_t.tBSP_Receive.uiBuff_Len = 1;
  Rcv_t.tBSP_Receive.pBuff = &uiData;
  Rcv_t.iTimeout = 0;

  do
  {
    uiData = 0;
    eEC = eBSP_Wifi_Intf_Receive(&Rcv_t.tBSP_Receive);
  }while(uiData != 0);

  if(eEC == ER_TIMEOUT)
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
ERROR_CODE eWifi_disable_echo(void)
{
  ERROR_CODE eEC = ER_FAIL;
  Wifi_Send_t Send_t;
  Wifi_Receive_t  Rcv_t;
  uint32_t uiCurrent_Tick;
  uint32_t uiStarted_Tick;
  char * pBuff = Wifi_AS_t.uiRcvBuff;
  bool bIs_Echo_On = false;

  //prepare buffers
  memset(Wifi_AS_t.uiSendBuff, 0x00, WIFI_SEND_BUF_LEN);
  memset(Wifi_AS_t.uiRcvBuff, 0x00, WIFI_RCV_BUF_LEN);

  //prep receive
  Rcv_t.tBSP_Receive.uiBuff_Len = 1;//strlen(AT_CMD_TEST) + strlen(AT_OK);
  Rcv_t.tBSP_Receive.pBuff = (uint8_t *)&Wifi_AS_t.uiRcvBuff;
  Rcv_t.tBSP_Receive.bBlocking = false;

  //prep to send AT command test
  Send_t.BSP_Send_t.uiBuff_Len = strlen(AT_CMD_TEST);
  memcpy(Wifi_AS_t.uiSendBuff, AT_CMD_TEST, strlen(AT_CMD_TEST));
  Send_t.BSP_Send_t.pBuff = Wifi_AS_t.uiSendBuff;

  eEC = eWifi_rcv(&Rcv_t);
  eEC = eWifi_send(&Send_t);

  eBSP_Get_Current_ms_count(&uiStarted_Tick);
  do
  {
    eOSAL_delay(1,NULL);

    eBSP_Get_Current_ms_count(&uiCurrent_Tick);
    if((uiCurrent_Tick - uiStarted_Tick) > 3000)
    {
      eEC = ER_TIMEOUT;
      break;
    }

    if(strstr(pBuff, AT_CMD_TEST) != NULL)
    {
      bIs_Echo_On = true;
//      eWifi_clear_intf();
//      break;
    }
  }while(strstr(pBuff, AT_OK) == NULL);

  if(bIs_Echo_On == true)
  {
    memset(pBuff, 0x00, 7);
    Rcv_t.tBSP_Receive.uiBuff_Len = strlen(AT_ECHO_OFF) + strlen(AT_OK);
    Rcv_t.tBSP_Receive.pBuff = pBuff;

    Send_t.BSP_Send_t.uiBuff_Len = strlen(AT_ECHO_OFF);
    memset(Wifi_AS_t.uiSendBuff, 0x00, WIFI_SEND_BUF_LEN);
    memcpy(Wifi_AS_t.uiSendBuff, AT_ECHO_OFF, strlen(AT_ECHO_OFF));
    Send_t.BSP_Send_t.pBuff = Wifi_AS_t.uiSendBuff;

    eBSP_Get_Current_ms_count(&uiStarted_Tick);

    eEC = eWifi_rcv(&Rcv_t);
    eEC = eWifi_send(&Send_t);
    WIFI_SEND_ASSERT();

    do
    {
      eOSAL_delay(1,NULL);
      eBSP_Get_Current_ms_count(&uiCurrent_Tick);
      if((uiCurrent_Tick - uiStarted_Tick) > 3000)
      {
        eEC = ER_TIMEOUT;
        break;
      }

    }while(strstr(pBuff, AT_OK) == NULL);
  }

  return eEC;
}

/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name: eWifi_rcv_OK
* description: receives the OK from the wifi module. Used for receiving responses
*              from wifi module while in command mode.
* param description: none
* return value description: ERROR_CODE - ER_OK: OK received
*                                      - ER_FAIL: failed to receive OK from module
******************************************************************************/
ERROR_CODE eWifi_rcv_OK(void)
{
  ERROR_CODE eEC = ER_FAIL;
  Wifi_Receive_t  Rcv_t;

  if(Wifi_AS_t.bISR_Rcv == true)
  {
    while(strstr((char *)Wifi_AS_t.uiRcvBuff, AT_OK) == NULL)
    {
      if(strstr((char *)Rcv_t.tBSP_Receive.pBuff, AT_ERROR) != NULL)
      {
        vDEBUG_ASSERT("eWifi_rcv_OK ERROR!", 0);
      }
      else
      {
        eOSAL_delay(1, NULL);
        eEC = ER_OK;
      }
    }
  }
  else
  {
    vDEBUG_ASSERT("Currently broken", 0);
    memset(Wifi_AS_t.uiRcvBuff, 0x00, strlen(AT_ERROR)+1);
  //  Rcv_t.tBSP_Receive.uiBuff_Len = strlen(AT_OK) + 1;
    Rcv_t.tBSP_Receive.uiBuff_Len = strlen(AT_ERROR) + 1;
  //  Rcv_t.tBSP_Receive.pBuff = Wifi_AS_t.uiRcvBuff;
  //  Rcv_t.iTimeout = WIFI_XMIT_1S_TIMEOUT * 10;
    eEC =  eWifi_rcv(&Rcv_t);
    if(eEC == ER_OK)
    {
      while(strstr((char *)Wifi_AS_t.uiRcvBuff, AT_OK) == NULL)
      {
        if(strstr((char *)Rcv_t.tBSP_Receive.pBuff, AT_ERROR) != NULL)
        {
          vDEBUG_ASSERT("eWifi_rcv_OK non OK ERROR resp", 0);
        }
        else
        {
          eOSAL_delay(1, NULL);
          eEC = ER_OK;
        }
      }

  //    if(strstr((char *)Rcv_t.tBSP_Receive.pBuff, AT_OK) == NULL)
  //    {
  //      Rcv_t.tBSP_Receive.uiBuff_Len = 1;
  //      Rcv_t.tBSP_Receive.pBuff = &Wifi_AS_t.uiSendBuff[strlen(AT_OK)];
  //      Rcv_t.iTimeout = WIFI_XMIT_1S_TIMEOUT;
  //      eEC = eWifi_rcv(&Rcv_t);
  //      if(strstr((char *)Rcv_t.tBSP_Receive.pBuff, AT_ERROR) == NULL)
  //      {
  //        vDEBUG_ASSERT("eWifi_rcv_OK non OK ERROR resp", 0);
  //      }
  //      else
  //      {
  //        eEC = ER_FAIL;
  //      }
  //    }
  //    else
  //    {
  //      eEC = ER_OK;
  //    }
    }
  }

  return eEC;
}

/******************************************************************************
* todo: DESCRIPTION, PARAM, RETURN
* name: eWifi_AP_connect
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eWifi_AP_connect(void)
{
  ERROR_CODE eEC = ER_FAIL;
  Wifi_Send_t Send_t;
  Wifi_Receive_t  Rcv_t;
  char * cAP_list = NULL;
  char * cStartAddress = NULL;
  char * cEndAddress = NULL;
  char cDiscard;
  uint8_t uiAddress[12];
  int iTimeout;
  int index = 0;
  Rcv_t.tBSP_Receive.uiBuff_Len = WIFI_RCV_BUF_LEN;
  Rcv_t.tBSP_Receive.pBuff = Wifi_AS_t.uiRcvBuff;

  //list AP's
  //setup the send buffer
  index = Send_t.BSP_Send_t.uiBuff_Len = strlen(AT_CMD_LST_AP);
  Send_t.BSP_Send_t.pBuff = Wifi_AS_t.uiSendBuff;
  memcpy(Send_t.BSP_Send_t.pBuff, AT_CMD_LST_AP, Send_t.BSP_Send_t.uiBuff_Len);
  Send_t.BSP_Send_t.pBuff[index] = 0;

  //setup the receive buffer
  Rcv_t.tBSP_Receive.uiBuff_Len = 1024 * 3;
  cAP_list = calloc(1024 * 3, sizeof(uint8_t));
  vDEBUG_ASSERT("eWifi_Setup cAP_list malloc NULL", cAP_list == NULL);
  iTimeout = WIFI_XMIT_1S_TIMEOUT * 3;
  Rcv_t.iTimeout = iTimeout;
  Rcv_t.tBSP_Receive.pBuff = (uint8_t *)cAP_list;

  //send the AP list command
  eEC = eWifi_send(&Send_t);
  WIFI_SEND_ASSERT();

  //receive the AP list
  eEC = eWifi_rcv(&Rcv_t);

  //Find AP
  if(strstr((char *)cAP_list, GET_AP_ID()) == NULL)
  {
    vDEBUG_ASSERT("AP not found", 1);
    eEC = ER_NOT_FOUND;
  }
  else
  {
    eEC = ER_OK;
  }

  if(strstr((char *)Rcv_t.tBSP_Receive.pBuff, AT_OK) == NULL)
  {
    eWifi_clear_intf();
    vDEBUG_ASSERT("cAP_list not big enough", 0);
  }

  free(cAP_list);

  if(eEC == ER_OK)
  {
    //connect to the AP
    Send_t.BSP_Send_t.pBuff = Wifi_AS_t.uiSendBuff;
    index = Send_t.BSP_Send_t.uiBuff_Len = snprintf((char *)Send_t.BSP_Send_t.pBuff, WIFI_SEND_BUF_LEN, AT_CMD_JOIN_AP, GET_AP_ID(), GET_AP_PW());
    Send_t.BSP_Send_t.pBuff[index] = 0;

    eEC = eWifi_send(&Send_t);
    WIFI_SEND_ASSERT();

    eEC = eWifi_rcv_OK();
  }

  if(eEC == ER_OK)
  {
    //Get the IP address
    Send_t.BSP_Send_t.pBuff = Wifi_AS_t.uiSendBuff;
    index = Send_t.BSP_Send_t.uiBuff_Len = strlen(AT_CMD_GET_IP_ADD);
    memcpy(Send_t.BSP_Send_t.pBuff, AT_CMD_GET_IP_ADD, index);
    Send_t.BSP_Send_t.pBuff[index] = 0;

    Rcv_t.tBSP_Receive.uiBuff_Len = WIFI_RCV_BUF_LEN;
    iTimeout = WIFI_XMIT_1S_TIMEOUT * 3;
    Rcv_t.iTimeout = iTimeout;
    Rcv_t.bReceive_Until_OK = true;
    Rcv_t.tBSP_Receive.pBuff = Wifi_AS_t.uiRcvBuff;
    memset(Rcv_t.tBSP_Receive.pBuff, 0x00, WIFI_RCV_BUF_LEN);

    eEC = eWifi_send(&Send_t);
    WIFI_SEND_ASSERT();

    eEC = eWifi_rcv(&Rcv_t);

    cStartAddress = strchr((char *)Wifi_AS_t.uiRcvBuff, '"');
    cEndAddress = strchr(cStartAddress + 1, '"');
    index = cEndAddress - cStartAddress;
    memcpy(Wifi_AS_t.cIP_Address, cStartAddress + 1, index - 1);
    cStartAddress = Wifi_AS_t.cIP_Address;
    index = sscanf(cStartAddress, "%d.%d.%d.%d",
           (int *)&uiAddress[0],
           (int *)&uiAddress[1],
           (int *)&uiAddress[2],
           (int *)&uiAddress[3]);

    memcpy(Wifi_AS_t.uiIP_Address, uiAddress, 4);
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
ERROR_CODE eWifi_send(Wifi_Send_t * pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  BSP_Wifi_Transmit_t Send_t;

  memcpy(&Send_t, &pParam->BSP_Send_t, sizeof(BSP_Wifi_Transmit_t));

  if(pParam->bAT_OK_Resp == true)
  {
    Wifi_AS_t.uiRcvLen = strlen(AT_OK);
    Wifi_AS_t.pRcvBuff = Wifi_AS_t.uiRcvBuff;
  }
  eEC = eBSP_Wifi_Intf_Send(&Send_t);

  return eEC;
}

//todo: AT send with expected AT_OK response
ERROR_CODE eWifi_AT_Send(pWifi_Send pSend)
{
  ERROR_CODE eEC = ER_FAIL;
  vDEBUG_ASSERT("eWifi_AT_Send not implemented yet", eEC == ER_OK);
  return eEC;
}

ERROR_CODE eWIFI_Rcv_Init(void)
{
  ERROR_CODE eEC = ER_FAIL;
  BSP_Wifi_Receive_t Bsp_Rcv_t;

  Bsp_Rcv_t.bBlocking = false;
  Bsp_Rcv_t.pBuff = &Wifi_AS_t.uiISR_RcvBuff;
  Bsp_Rcv_t.uiBuff_Len = 1;

  Wifi_AS_t.bISR_Rcv = true;

  eEC = eBSP_Wifi_Intf_Receive(&Bsp_Rcv_t);

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
ERROR_CODE eWifi_rcv(pWifi_Receive pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  int iTimeout = pParam->iTimeout;
  BSP_Wifi_Receive_t Bsp_Rcv_t;
  int32_t iIndex = 0;
  int32_t iLen = 0;
  uint8_t * pBuff = NULL;

  memcpy(&Bsp_Rcv_t, &pParam->tBSP_Receive, sizeof(BSP_Wifi_Receive_t));

  if(Wifi_AS_t.bISR_Rcv == true)
  {
    Wifi_AS_t.uiRcvLen = pParam->tBSP_Receive.uiBuff_Len;
    Wifi_AS_t.pRcvBuff = Wifi_AS_t.uiRcvBuff;
    eEC = ER_OK;

  }
  else
  {
    if(pParam->bReceive_Until_OK == false)
    {
  //    do
  //    {
        eEC = eBSP_Wifi_Intf_Receive(&Bsp_Rcv_t);
  //      iTimeout--;
  //      if(iTimeout <= 0)
  //      {
  //        break;
  //      }
  //    }while(eEC == ER_TIMEOUT);
    }
    else
    {
      iLen = Bsp_Rcv_t.uiBuff_Len;
      iIndex = 0;
      pBuff = Bsp_Rcv_t.pBuff;
      do
      {
        Bsp_Rcv_t.pBuff = &pBuff[iIndex];
        Bsp_Rcv_t.uiBuff_Len = 1;
        iIndex++;
        if(iIndex == iLen)
        {
          eEC = ER_LEN;
          break;
        }
        eEC = eBSP_Wifi_Intf_Receive(&Bsp_Rcv_t);
        if(strstr((char *)pBuff, AT_OK) != NULL)
        {
          eEC = ER_OK;
          break;
        }
      }while(eEC == ER_OK);
    }
  }


  return eEC;
}

void vWifi_Intf_isr_callback(void)
{
  Wifi_Receive_t  Rcv_t;
  int i;
  static int x = 0;

  if(Wifi_AS_t.eSTATE == WIFI_STATE_INIT_GET_READY)
  {
    i = 6;
    if(Wifi_AS_t.uiRcvBuff[i] == 0)
    {
      if(Wifi_AS_t.uiRcvBuff[0] == 0)
      {
        Wifi_AS_t.uiRcvBuff[0] = Wifi_AS_t.uiISR_RcvBuff;
      }
      else
      {
        for(i = 6; i > 0; i--)
        {
          if(Wifi_AS_t.uiRcvBuff[i - 1] != 0)
          {
            Wifi_AS_t.uiRcvBuff[i] = Wifi_AS_t.uiISR_RcvBuff;
            break;
          }
        }
      }
    }
    else
    {
      memcpy(&Wifi_AS_t.uiRcvBuff[0], &Wifi_AS_t.uiRcvBuff[1], 7);
      Wifi_AS_t.uiRcvBuff[6] = Wifi_AS_t.uiISR_RcvBuff;
    }
    Wifi_AS_t.uiISR_RcvBuff = 0;

    Rcv_t.tBSP_Receive.pBuff = &Wifi_AS_t.uiISR_RcvBuff;
    Rcv_t.tBSP_Receive.bBlocking = false;
    Rcv_t.tBSP_Receive.uiBuff_Len = 1;
    eBSP_Wifi_Intf_Receive(&Rcv_t.tBSP_Receive);
//    eWifi_rcv(&Rcv_t);
  }
  else if(Wifi_AS_t.eSTATE == WIFI_STATE_INIT_ECHO)
  {
//    x++;
//    if(x == 2)
//    {
//      x = 2;
//    }
    *Wifi_AS_t.pRcvBuff++ = Wifi_AS_t.uiISR_RcvBuff;

    Wifi_AS_t.uiISR_RcvBuff = 0;
    Rcv_t.tBSP_Receive.pBuff = &Wifi_AS_t.uiISR_RcvBuff;
    Rcv_t.tBSP_Receive.bBlocking = false;
    Rcv_t.tBSP_Receive.uiBuff_Len = 1;
    eBSP_Wifi_Intf_Receive(&Rcv_t.tBSP_Receive);
//    eWifi_rcv(&Rcv_t);
  }
  else
  {
    *Wifi_AS_t.pRcvBuff++ = Wifi_AS_t.uiISR_RcvBuff;

    Wifi_AS_t.uiISR_RcvBuff = 0;
    Rcv_t.tBSP_Receive.pBuff = &Wifi_AS_t.uiISR_RcvBuff;
    Rcv_t.tBSP_Receive.bBlocking = false;
    Rcv_t.tBSP_Receive.uiBuff_Len = 1;
    eWifi_rcv(&Rcv_t);
  }
  return;
}

/******************************************************************************
* name: vWifi_Driver_Task
* description: WIFI driver task function
* param description: void - pwParameters: pointer to the task parameters
* return value description: void: should never return
******************************************************************************/
void vWifi_Driver_Task(void * pvParameters)
{
  ERROR_CODE eEC = ER_FAIL;
  OSAL_Queue_Parameters_t tWifi_Queue_Param;
  pOSAL_Queue_Handle pWifi_Queue_Handle;
  tWifi_Message_Struct tMsg;
  Wifi_Send_t Send_t;
#if (PROJ_CONFIG_USE_DRVR_NVRAM >= 1)
  pWifi_Activity_State pActivity_State;
  tNvram_Request tNVReq;
#endif //PROJ_CONFIG_USE_DRVR_WIFI

  char cHelloWorld[] = "HELLO WORLD";

  eOSAL_Queue_Params_Init(&tWifi_Queue_Param);
  tWifi_Queue_Param.uiNum_Of_Queue_Elements = 3;
  tWifi_Queue_Param.uiSize_Of_Queue_Element = sizeof(tWifi_Message_Struct);
  tWifi_Queue_Param.pMsgBuff                = &tMsg;
  tWifi_Queue_Param.iTimeout                = OSAL_QUEUE_TIMEOUT_WAITFOREVER;

  eEC = eOSAL_Queue_Create(&tWifi_Queue_Param, &pWifi_Queue_Handle);
  vDEBUG_ASSERT("vWifi_Driver_Task queue create fail", eEC == ER_OK);

  //check persistent settings
  //
#if (PROJ_CONFIG_USE_DRVR_NVRAM >= 1)
  //Get persistent settings from NVRAM
  //
  eEC = eNvram_Request_Param_Init(&tNVReq);
  vDEBUG_ASSERT("eNvram_Request_Param_Init fail", eEC == ER_OK);
  tNVReq.eRequestID = NVRAM_REQUEST_IS_ID_REGISTERED;
  tNVReq.uiSettings_ID = Wifi_AS_t.uiWifi_NVID;
  eEC = eNvram_Request(&tNVReq);
  vDEBUG_ASSERT("vWifi_Driver_Task eNvram_Request fail", eEC == ER_OK);

  //if wifi persistent settings exist copy over what is needed
  pActivity_State = calloc(sizeof(Wifi_Activity_State_t), sizeof(uint8_t));
  eNvram_Request_Param_Init(&tNVReq);
  tNVReq.eRequestID = NVRAM_REQUEST_READ;
  tNVReq.uiSettings_ID = Wifi_AS_t.uiWifi_NVID;
  tNVReq.pBuff = (uint8_t *)pActivity_State;
  tNVReq.uiSize = sizeof(Wifi_Activity_State_t);
  eNvram_Request(&tNVReq);
  memcpy(Wifi_AS_t.uiMAC_Address, pActivity_State->uiMAC_Address,
        (sizeof(Wifi_AS_t.uiMAC_Address) + sizeof(Wifi_AS_t.cMAC_Address)));
  free(pActivity_State);
#else
  //Use static settings
  //

  //TODO: Static settings
#endif

  //perform wifi setup
  eEC = eWifi_Setup();
  vDEBUG_ASSERT("vWifi_Driver_Task driver setup fail", eEC == ER_OK);
  if(eEC == ER_OK)
  {
    Wifi_AS_t.bIs_Wifi_Ready = true;
  }
  else
  {
    Wifi_AS_t.bIs_Wifi_Ready = false;
  }

//  tMsg.eMSG = WIFI_MSG_TEST;
//  eEC = eOSAL_Queue_Post_msg(pWifi_Queue_Handle, &tMsg);
//  vDEBUG_ASSERT("vWifi_Driver_Task msg post fail", eEC != ER_OK);

  Send_t.iTimeout = 1000;
  Send_t.BSP_Send_t.pBuff = (uint8_t *)cHelloWorld;
  Send_t.BSP_Send_t.uiBuff_Len = strlen(cHelloWorld);
  memset(&tMsg, 0x00, sizeof(tWifi_Message_Struct));

  Wifi_AS_t.bIs_Wifi_Task_Ready = true;

  while(1)
  {
    if(eOSAL_Queue_Get_msg(pWifi_Queue_Handle, &tMsg) == ER_OK)
    {
      switch(tMsg.eMSG)
      {
        case WIFI_MSG_TEST:
        {
          eBSP_Wifi_Intf_Send(NULL);
          break;
        }

        case WIFI_MSG_AP_CONNECT:
        {

          break;
        }
        default:
          vDEBUG_ASSERT("vWifi_Driver_Task invalid msg ID", tMsg.eMSG < WIFI_MSG_LIMIT);
      }
    }
  }
}

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* todo: NAME, DESCRIPTION, PARAM, RETURN
* name: Filename_or_abreviation_funciton
* description:
* param description: type - value: value description (in order from left to right)
*                    bool - true: do action when set to true
* return value description: type - value: value description
******************************************************************************/
ERROR_CODE eWifi_Request_Param_Init(pWifi_Request pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  if(pParam == NULL)
  {
    eEC = ER_PARAM;
  }
  else
  {
    pParam->eRequestID = WIFI_REQUEST_NONE;
    pParam->pWifi_Task_Param = NULL;

    if((pParam->eRequestID       == WIFI_REQUEST_NONE) &
       (pParam->pWifi_Task_Param == NULL))
    {
      eEC = ER_OK;
    }
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
ERROR_CODE eWifi_Request(pWifi_Request pRequest)
{
  ERROR_CODE eEC = ER_FAIL;

  if(pRequest->eRequestID == WIFI_REQUEST_TASK_PARAMETERS)
  {
    pRequest->pWifi_Task_Param->pTaskFcn        = &vWifi_Driver_Task;
    pRequest->pWifi_Task_Param->pName           = cWifi_Task_Name;
    pRequest->pWifi_Task_Param->uiStack_Size    = TASK_WIFI_STACK_SIZE;
    pRequest->pWifi_Task_Param->pParameters     = NULL;
    pRequest->pWifi_Task_Param->uiTask_Priority = TASK_WIFI_PRIORITY;

    eEC = ER_OK;
  }
  else
  {
    switch(pRequest->eRequestID)
    {
      case WIFI_REQUEST_TASK_PARAMETERS:
      {

        eEC = ER_OK;
        break;
      }
      case WIFI_REQUEST_CONNECT:
      {
        break;
      }
      default:
        eEC = ER_FAIL;
    }
  }



  return eEC;
}

#endif //PROJ_CONFIG_USE_DRVR_WIFI
#endif //__WIFI_C__
