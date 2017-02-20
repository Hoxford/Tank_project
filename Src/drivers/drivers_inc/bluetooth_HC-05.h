/******************************************************************************
*
* bluetooth_HC-05.h - Bluetooth module specific interface file.
* Copyright (c) notice
*
******************************************************************************/
#ifndef __BLUETOOTH_HC05_H__
#define __BLUETOOTH_HC05_H__
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
#if(PROJ_CONFIG_PLATFORM_BT != PROJ_CONFIG_PLATFORM_BT_HC05)
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
//PUB_EXAMPLE_ENUM description
typedef enum PUB_EXAMPLE_ENUM
{
    PUB_ENUM_A,
    PUB_ENUM_B,
    PUB_ENUM_C,
}PUB_EXAMPLE_ENUM, * pPUB_EXAMPLE_ENUM;

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

typedef struct HC05_Register_Receive
{
  void(* vRcvByte)(volatile uint8_t * pBuff);
}HC05_Register_Receive_t, * pHC05_Register_Receive;

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/
void   vBluetooth_HC05_intf_isr_callback (void);

/******************************************************************************
* Name: eBluetooth_HC05_setup
* Description:
*   Performs bluetooth module specific setup. When this function is called the
*   module will be reset to factory defaults and initialized to project
*   requirements.
*
* Parameters:
*   none
*
* Returns:
*   (ERROR_CODE): description
*     = ER_OK: Bluetooth setup success
*     = ER_FAIL: Bluetooth setup failure
* Example:
* void Bluetooth_Hardware_Setup(void)
* {
*   ERROR_CODE eEC = ER_FAIL;
*   //Function usage
*   eEC = eBluetooth_HC05_setup();
*   ASSERT(eEC == ER_OK);
* }
******************************************************************************/
ERROR_CODE eBluetooth_HC05_setup(void);

ERROR_CODE eBluetooth_HC05_Register_Receive(pHC05_Register_Receive pRegRcv);

#endif //PROJ_CONFIG_PLATFORM_BT_HC05
#endif //__BLUETOOTH_HC05_H__




