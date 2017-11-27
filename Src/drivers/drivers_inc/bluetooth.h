/******************************************************************************
*
* bluetooth.h - include file for the bluetooth driver module
* Copyright (c) notice
*
******************************************************************************/
#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__
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
#if (PROJ_CONFIG_USE_DRVR_BLUETOOTH == 0)
  #warning "Bluetooth driver not implemented"

#define eBluetooth_Request_Param_Init(p)   ER_NOT_ENABLED
#define eBluetooth_Request(p)              ER_NOT_ENABLED

#define Bluetooth_Request_t                void *//
#else

/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
//Bluetooth request IDs
typedef enum BLUETOOTH_REQUEST_ID
{
  BLUETOOTH_REQUEST_NONE = 0,
  BLUETOOTH_REQUEST_TASK_PARAMETERS,
  BLUETOOTH_REQUEST_PROVISION,
  BLUETOOTH_REQUEST_CONNECT,
  BLUETOOTH_REQUEST_SEND,
  BLUETOOTH_REQUEST_LIMIT,
}BLUETOOTH_REQUEST_ID, * pBLUETOOTH_REQUEST_ID;

typedef enum BLUETOOTH_CONN_STATE
{
  BT_CONN_STATE_NONE,
  BT_CONN_STATE_BONDED,
  BT_CONN_STATE_PAIRED,
}BLUETOOTH_CONN_STATE, * pBLUETOOTH_CONN_STATE;

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct Bluetooth_App_API
{
  ERROR_CODE (* eBTAppRcvCallBack)(uint8_t * pBuff, uint32_t u32BuffLen);
  ERROR_CODE (* eBTAppConnStatusCallBack)(BLUETOOTH_CONN_STATE eState);
}Bluetooth_App_API_t, * pBluetooth_App_API;

typedef struct Bluetooth_Device_API
{
  ERROR_CODE (* eBTDeviceSetup)(pBluetooth_App_API pApi);
  ERROR_CODE (* eBTDeviceSend)(uint8_t * pBuff, uint32_t u32Len);
  ERROR_CODE (* eBTDeviceRcv)(uint8_t * pBuff, uint32_t u32BuffLen);
  ERROR_CODE (* eBTDeviceConnStatus)(void);
}Bluetooth_Device_API_t, * pBluetooth_Device_API;

//Bluetooth_Request structure
typedef struct Bluetooth_Request
{
  BLUETOOTH_REQUEST_ID  eRequestID;
  pOSAL_Task_Parameters pBluetooth_Task_Param;
  uint8_t *             pBuff;
  uint32_t              ui32Len;
}Bluetooth_Request_t, * pBluetooth_Request;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

ERROR_CODE eBluetooth_Request_Param_Init(pBluetooth_Request pParam);

ERROR_CODE eBluetooth_Request(pBluetooth_Request pRequest);

#endif //PROJ_CONFIG_USE_DRVR_BLUETOOTH
#endif //__BLUETOOTH_H__
