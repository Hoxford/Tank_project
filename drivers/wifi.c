/******************************************************************************
*
* wifi.c - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __WIFI_C__
#define __WIFI_C__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "utils_inc/error_codes.h"
#include "utils_inc/osal.h"
#include "utils_inc/proj_debug.h"

#include "drivers_inc/wifi.h"
#include "drivers_inc/nvram.h"
#include "drivers_inc/AT_commands.h"
#include "board.h"

/******************************************************************************
* defines /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define TASK_WIFI_PRIORITY       2

#define WIFI_SEND_BUF_LEN    256
#define WIFI_RCV_BUF_LEN    256

#define WIFI_SEND_ASSERT()  vDEBUG_ASSERT("Wifi send fail", eEC == ER_OK)

#define WIFI_XMIT_1S_TIMEOUT  10

#define GET_AP_ID()       "NETGEAR76"
#define GET_AP_PW()       "modernroad450"

/******************************************************************************
* variables ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

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

/******************************************************************************
* structures //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//tWifi activity state for command, control and status of the task
typedef struct tWifi_Activity_State
{
  bool     bIs_Wifi_Ready;
  bool     bIs_Wifi_Yask_Ready;
  uint8_t  uiSendBuff[WIFI_SEND_BUF_LEN];
  uint8_t  uiRcvBuff[WIFI_RCV_BUF_LEN];
  uint8_t  uiIP_Address[4];
  char     cIP_Address[16];
  uint8_t  uiMAC_Address[6];
  char     cMAC_Address[18];
  uint32_t uiWifi_NVID;
}tWifi_Activity_State;

tWifi_Activity_State tWifi_AS =
{
  false,//bool    bIs_Wifi_Ready;
  false,//bool    bIs_Wifi_Yask_Ready;
  //uint8_t uiSendBuff[WIFI_SEND_BUF_LEN];
  {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  },
  //uint8_t uiRcvBuff[WIFI_RCV_BUF_LEN];
  {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  },
  //uint8_t uiIP_Address[4];
  {
    0,0,0,0,
  },
  //char    cIP_Address[16];
  {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  },
  //uint8_t uiMAC_Address[6];
  {
    0,0,0,0,0,0,
  },
  //char    cMAC_Address[18];
  {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  },
  0x74BEAD94,//uint32_t uiWifi_NVID;
};

typedef struct tWifi_Message_Struct
{
    WIFI_MSG_ID eMSG;
}tWifi_Message_Struct;

typedef struct tWifi_Send
{
  tBSP_tWifi_Transmit tBSP_Send;
  int32_t iTimeout;
}tWifi_Send;

typedef struct tWifi_Receive
{
  tBSP_Wifi_Receive tBSP_Receive;
  int32_t iTimeout;
  bool bReceive_Until_OK;
}tWifi_Receive;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* private function declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eWifi_Setup(void);
ERROR_CODE eWifi_Default_Settings(void);
ERROR_CODE eWifi_get_ready(void);
ERROR_CODE eWifi_clear_intf(void);
ERROR_CODE eWifi_disable_echo(void);
ERROR_CODE eWifi_rcv_OK(void);
ERROR_CODE eWifi_AP_connect(void);
ERROR_CODE eWifi_send(tWifi_Send * pParam);
ERROR_CODE eWifi_rcv(tWifi_Receive * pParam);
void       vWifi_Driver_Task(void * pvParameters);

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
  tWifi_Send tSend;
  int index;

  eEC = eWifi_get_ready();
  if(eEC == ER_OK)
  {
    //Disable echo
    eEC = eWifi_disable_echo();
  }

  if(eEC == ER_OK)
  {
    //Set wifi mode
    tSend.tBSP_Send.pBuff = tWifi_AS.uiSendBuff;
    index = tSend.tBSP_Send.uiBuff_Len = snprintf((char *)tSend.tBSP_Send.pBuff, WIFI_SEND_BUF_LEN, AT_CMD_SET_WIFI_MODE, AT_CMD_WIFI_MODE);
    tSend.tBSP_Send.pBuff[index] = 0;

    eEC = eWifi_send(&tSend);
    WIFI_SEND_ASSERT();

    eEC = eWifi_rcv_OK();
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
  char cReady[8];
  uint32_t uiCurrent_Tick;
  uint32_t uiStarted_Tick;

  tWifi_Receive  tRcv;

  memset(cReady, 0x00, 8);
  eWifi_clear_intf();

  eBSP_Wifi_Rst_Set();

  eOSAL_delay(100, NULL);

  eBSP_Wifi_Rst_Clr();

  tRcv.tBSP_Receive.uiBuff_Len = 6;
  tRcv.tBSP_Receive.pBuff = (uint8_t *)&cReady[1];
  tRcv.iTimeout = WIFI_XMIT_1S_TIMEOUT * 3;
  eWifi_rcv(&tRcv);

  tRcv.tBSP_Receive.uiBuff_Len = 1;
  tRcv.tBSP_Receive.pBuff = (uint8_t *)&cReady[7];

  eBSP_Get_Current_ms_count(&uiStarted_Tick);
  do
  {
    eEC = eWifi_rcv(&tRcv);
    if(eEC == ER_FAIL)
    {
      break;
    }
    else if(eEC == ER_OK)
    {
      eBSP_Get_Current_ms_count(&uiStarted_Tick);
    }

    memcpy(&cReady[0], &cReady[1], 7);
    cReady[7] = 0;

    eBSP_Get_Current_ms_count(&uiCurrent_Tick);
    if((uiCurrent_Tick - uiStarted_Tick) > 3000)
    {
      eEC = ER_TIMEOUT;
      break;
    }
  }while(strstr(cReady, "ready\r\n") == NULL);

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
  tWifi_Receive  tRcv;

  tRcv.tBSP_Receive.uiBuff_Len = 1;
  tRcv.tBSP_Receive.pBuff = &uiData;
  tRcv.iTimeout = 0;

  do
  {
    uiData = 0;
    eEC = eBSP_Wifi_Intf_Receive(&tRcv.tBSP_Receive);
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
  tWifi_Send tSend;
  tWifi_Receive  tRcv;
  uint32_t uiCurrent_Tick;
  uint32_t uiStarted_Tick;
  char cEco_Buff[7];
  bool bIs_Echo_On = false;

  memset(cEco_Buff, 0x00, 7);

  tSend.tBSP_Send.uiBuff_Len = strlen(AT_CMD_TEST);
  memset(tWifi_AS.uiSendBuff, 0x00, WIFI_SEND_BUF_LEN);
  memcpy(tWifi_AS.uiSendBuff, AT_CMD_TEST, strlen(AT_CMD_TEST));
  tSend.tBSP_Send.pBuff = tWifi_AS.uiSendBuff;

  eEC = eWifi_send(&tSend);
  WIFI_SEND_ASSERT();

  tRcv.tBSP_Receive.uiBuff_Len = 5;
  tRcv.tBSP_Receive.pBuff = (uint8_t *)&cEco_Buff[1];
  eEC = eWifi_rcv(&tRcv);

  tRcv.tBSP_Receive.uiBuff_Len = 1;
  tRcv.tBSP_Receive.pBuff = (uint8_t *)&cEco_Buff[6];

  eBSP_Get_Current_ms_count(&uiStarted_Tick);
  do
  {
    eEC = eWifi_rcv(&tRcv);
    if(eEC != ER_OK)
    {
      break;
    }

    memcpy(&cEco_Buff[0], &cEco_Buff[1], 6);
    cEco_Buff[6] = 0;

    eBSP_Get_Current_ms_count(&uiCurrent_Tick);
    if((uiCurrent_Tick - uiStarted_Tick) > 3000)
    {
      eEC = ER_TIMEOUT;
      break;
    }

    if(strstr(cEco_Buff, AT_CMD_TEST) != NULL)
    {
      bIs_Echo_On = true;
      eWifi_clear_intf();
      break;
    }
  }while(strstr(cEco_Buff, AT_OK) == NULL);

  if(bIs_Echo_On == true)
  {
    memset(cEco_Buff, 0x00, 7);
    tRcv.tBSP_Receive.uiBuff_Len = 5;
    tRcv.tBSP_Receive.pBuff = (uint8_t *)&cEco_Buff[1];

    tSend.tBSP_Send.uiBuff_Len = strlen(AT_ECHO_OFF);
    memset(tWifi_AS.uiSendBuff, 0x00, WIFI_SEND_BUF_LEN);
    memcpy(tWifi_AS.uiSendBuff, AT_ECHO_OFF, strlen(AT_ECHO_OFF));
    tSend.tBSP_Send.pBuff = tWifi_AS.uiSendBuff;

    eBSP_Get_Current_ms_count(&uiStarted_Tick);


    eEC = eWifi_send(&tSend);
    WIFI_SEND_ASSERT();

    eEC = eWifi_rcv(&tRcv);
    tRcv.tBSP_Receive.uiBuff_Len = 1;
    tRcv.tBSP_Receive.pBuff = (uint8_t *)&cEco_Buff[6];
    do
    {
      eEC = eWifi_rcv(&tRcv);
      if(eEC != ER_OK)
      {
        break;
      }

      memcpy(&cEco_Buff[0], &cEco_Buff[1], 6);
      cEco_Buff[6] = 0;

      eBSP_Get_Current_ms_count(&uiCurrent_Tick);
      if((uiCurrent_Tick - uiStarted_Tick) > 3000)
      {
        eEC = ER_TIMEOUT;
        break;
      }

    }while(strstr(cEco_Buff, AT_OK) == NULL);
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
  tWifi_Receive  tRcv;

  memset(tWifi_AS.uiSendBuff, 0x00, strlen(AT_ERROR)+1);
  tRcv.tBSP_Receive.uiBuff_Len = strlen(AT_OK);
  tRcv.tBSP_Receive.pBuff = tWifi_AS.uiSendBuff;
  tRcv.iTimeout = WIFI_XMIT_1S_TIMEOUT * 10;

  eEC =  eWifi_rcv(&tRcv);
  if(eEC == ER_OK)
  {
    if(strstr((char *)tRcv.tBSP_Receive.pBuff, AT_OK) == NULL)
    {
      tRcv.tBSP_Receive.uiBuff_Len = 1;
      tRcv.tBSP_Receive.pBuff = &tWifi_AS.uiSendBuff[strlen(AT_OK)];
      tRcv.iTimeout = WIFI_XMIT_1S_TIMEOUT;
      eEC = eWifi_rcv(&tRcv);
      if(strstr((char *)tRcv.tBSP_Receive.pBuff, AT_ERROR) == NULL)
      {
        vDEBUG_ASSERT("eWifi_rcv_OK non OK ERROR resp", 0);
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
  tWifi_Send tSend;
  tWifi_Receive  tRcv;
  char * cAP_list = NULL;
  char * cStartAddress = NULL;
  char * cEndAddress = NULL;
  char cDiscard;
  uint8_t uiAddress[12];
  int iTimeout;
  int index = 0;
  tRcv.tBSP_Receive.uiBuff_Len = WIFI_RCV_BUF_LEN;
  tRcv.tBSP_Receive.pBuff = tWifi_AS.uiRcvBuff;

  //list AP's
  //setup the send buffer
  index = tSend.tBSP_Send.uiBuff_Len = strlen(AT_CMD_LST_AP);
  tSend.tBSP_Send.pBuff = tWifi_AS.uiSendBuff;
  memcpy(tSend.tBSP_Send.pBuff, AT_CMD_LST_AP, tSend.tBSP_Send.uiBuff_Len);
  tSend.tBSP_Send.pBuff[index] = 0;

  //setup the receive buffer
  tRcv.tBSP_Receive.uiBuff_Len = 1024 * 3;
  cAP_list = calloc(1024 * 3, sizeof(uint8_t));
  vDEBUG_ASSERT("eWifi_Setup cAP_list malloc NULL", cAP_list == NULL);
  iTimeout = WIFI_XMIT_1S_TIMEOUT * 3;
  tRcv.iTimeout = iTimeout;
  tRcv.tBSP_Receive.pBuff = (uint8_t *)cAP_list;

  //send the AP list command
  eEC = eWifi_send(&tSend);
  WIFI_SEND_ASSERT();

  //receive the AP list
  eEC = eWifi_rcv(&tRcv);

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

  if(strstr((char *)tRcv.tBSP_Receive.pBuff, AT_OK) == NULL)
  {
    eWifi_clear_intf();
    vDEBUG_ASSERT("cAP_list not big enough", 0);
  }

  free(cAP_list);

  if(eEC == ER_OK)
  {
    //connect to the AP
    tSend.tBSP_Send.pBuff = tWifi_AS.uiSendBuff;
    index = tSend.tBSP_Send.uiBuff_Len = snprintf((char *)tSend.tBSP_Send.pBuff, WIFI_SEND_BUF_LEN, AT_CMD_JOIN_AP, GET_AP_ID(), GET_AP_PW());
    tSend.tBSP_Send.pBuff[index] = 0;

    eEC = eWifi_send(&tSend);
    WIFI_SEND_ASSERT();

    eEC = eWifi_rcv_OK();
  }

  if(eEC == ER_OK)
  {
    //Get the IP address
    tSend.tBSP_Send.pBuff = tWifi_AS.uiSendBuff;
    index = tSend.tBSP_Send.uiBuff_Len = strlen(AT_CMD_GET_IP_ADD);
    memcpy(tSend.tBSP_Send.pBuff, AT_CMD_GET_IP_ADD, index);
    tSend.tBSP_Send.pBuff[index] = 0;

    tRcv.tBSP_Receive.uiBuff_Len = WIFI_RCV_BUF_LEN;
    iTimeout = WIFI_XMIT_1S_TIMEOUT * 3;
    tRcv.iTimeout = iTimeout;
    tRcv.bReceive_Until_OK = true;
    tRcv.tBSP_Receive.pBuff = tWifi_AS.uiRcvBuff;
    memset(tRcv.tBSP_Receive.pBuff, 0x00, WIFI_RCV_BUF_LEN);

    eEC = eWifi_send(&tSend);
    WIFI_SEND_ASSERT();

    eEC = eWifi_rcv(&tRcv);

    cStartAddress = strchr((char *)tWifi_AS.uiRcvBuff, '"');
    cEndAddress = strchr(cStartAddress + 1, '"');
    index = cEndAddress - cStartAddress;
    memcpy(tWifi_AS.cIP_Address, cStartAddress + 1, index - 1);
    cStartAddress = tWifi_AS.cIP_Address;
    index = sscanf(cStartAddress, "%d.%d.%d.%d",
           &uiAddress[0],
           &uiAddress[1],
           &uiAddress[2],
           &uiAddress[3]);

    memcpy(tWifi_AS.uiIP_Address, uiAddress, 4);
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
ERROR_CODE eWifi_send(tWifi_Send * pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  tBSP_tWifi_Transmit tSend;

  memcpy(&tSend, &pParam->tBSP_Send, sizeof(tBSP_tWifi_Transmit));

  eEC = eBSP_Wifi_Intf_Send(&tSend);

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
ERROR_CODE eWifi_rcv(tWifi_Receive * pParam)
{
  ERROR_CODE eEC = ER_FAIL;
  int iTimeout = pParam->iTimeout;
  tBSP_Wifi_Receive tRcv;
  int32_t iIndex = 0;
  int32_t iLen = 0;
  uint8_t * pBuff = NULL;

  memcpy(&tRcv, &pParam->tBSP_Receive, sizeof(tBSP_Wifi_Receive));

  if(pParam->bReceive_Until_OK == false)
  {
    do
    {
      eEC = eBSP_Wifi_Intf_Receive(&tRcv);
      iTimeout--;
      if(iTimeout <= 0)
      {
        break;
      }
    }while(eEC == ER_TIMEOUT);
  }
  else
  {
    iLen = tRcv.uiBuff_Len;
    iIndex = 0;
    pBuff = tRcv.pBuff;
    do
    {
      tRcv.pBuff = &pBuff[iIndex];
      tRcv.uiBuff_Len = 1;
      iIndex++;
      if(iIndex == iLen)
      {
        eEC = ER_LEN;
        break;
      }
      eEC = eBSP_Wifi_Intf_Receive(&tRcv);
      if(strstr((char *)pBuff, AT_OK) != NULL)
      {
        eEC = ER_OK;
        break;
      }
    }while(eEC == ER_OK);
  }

  return eEC;
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
  tOSAL_Queue_Parameters tWifi_Queue_Param;
  tOSAL_Queue_Handle * pWifi_Queue_Handle;
  tWifi_Activity_State * pActivity_State;
  tWifi_Message_Struct tMsg;
  tWifi_Send tSend;
  tNvram_Request tNVReq;
  char cHelloWorld[] = "HELLO WORLD";
//  tWifi_Receive  tRcv;

  eOSAL_Queue_Params_Init(&tWifi_Queue_Param);
  tWifi_Queue_Param.uiNum_Of_Queue_Elements = 3;
  tWifi_Queue_Param.uiSize_Of_Queue_Element = sizeof(tWifi_Message_Struct);
  tWifi_Queue_Param.pMsgBuff = &tMsg;
  tWifi_Queue_Param.iTimeout = OSAL_QUEUE_TIMEOUT_WAITFOREVER;

  eEC = eOSAL_Queue_Create(&tWifi_Queue_Param, &pWifi_Queue_Handle);
  vDEBUG_ASSERT("vWifi_Driver_Task queue create fail", eEC == ER_OK);

  //check persistant settings
  eEC = eNvram_Request_Param_Init(&tNVReq);
  vDEBUG_ASSERT("eNvram_Request_Param_Init fail", eEC == ER_OK);
  tNVReq.eRequestID = NVRAM_REQUEST_IS_ID_REGISTERED;
  tNVReq.uiSettings_ID = tWifi_AS.uiWifi_NVID;
  eEC = eNvram_Request(&tNVReq);
  if(eEC == ER_OK)
  {
    //if wifi persistent settings exist copy over what is needed
    pActivity_State = calloc(sizeof(tWifi_Activity_State), sizeof(uint8_t));
    eNvram_Request_Param_Init(&tNVReq);
    tNVReq.eRequestID = NVRAM_REQUEST_READ;
    tNVReq.uiSettings_ID = tWifi_AS.uiWifi_NVID;
    tNVReq.pBuff = (uint8_t *)pActivity_State;
    tNVReq.uiSize = sizeof(tWifi_Activity_State);
    eNvram_Request(&tNVReq);
    memcpy(tWifi_AS.uiMAC_Address, pActivity_State->uiMAC_Address,
          (sizeof(tWifi_AS.uiMAC_Address) + sizeof(tWifi_AS.cMAC_Address)));
    free(pActivity_State);
  }

  //perform wifi setup
  eEC = eWifi_Setup();
  vDEBUG_ASSERT("vWifi_Driver_Task driver setup fail", eEC == ER_OK);


//  tMsg.eMSG = WIFI_MSG_TEST;
//  eEC = eOSAL_Queue_Post_msg(pWifi_Queue_Handle, &tMsg);
//  vDEBUG_ASSERT("vWifi_Driver_Task msg post fail", eEC != ER_OK);

  tSend.iTimeout = 1000;
  tSend.tBSP_Send.pBuff = (uint8_t *)cHelloWorld;
  tSend.tBSP_Send.uiBuff_Len = strlen(cHelloWorld);
  memset(&tMsg, 0x00, sizeof(tWifi_Message_Struct));

  while(1)
  {
    eOSAL_delay(1000, NULL);
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
ERROR_CODE eWifi_Request_Param_Init(tWifi_Request * pRequest)
{
  ERROR_CODE eEC = ER_FAIL;
  if(pRequest == NULL)
  {
    eEC = ER_PARAM;
  }
  else
  {
    pRequest->eRequestID = WIFI_REQUEST_NONE;
    pRequest->pWifi_Task_Param = NULL;

    if((pRequest->eRequestID       == WIFI_REQUEST_NONE) &
       (pRequest->pWifi_Task_Param == NULL))
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
ERROR_CODE eWifi_Request(tWifi_Request * pRequest)
{
  ERROR_CODE eEC = ER_FAIL;

  if(pRequest->eRequestID == WIFI_REQUEST_TASK_PARAMETERS)
  {
    pRequest->pWifi_Task_Param->pTaskFcn = &vWifi_Driver_Task;
    pRequest->pWifi_Task_Param->pName = cWifi_Task_Name;
    pRequest->pWifi_Task_Param->uiStack_Size = 4096;
    pRequest->pWifi_Task_Param->pParameters = NULL;
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

#endif //__WIFI_C__
