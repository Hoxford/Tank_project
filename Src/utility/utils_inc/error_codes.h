//*****************************************************************************
//
// error_codes.h - system wide function error codes
//
// Copyright (c) notice
//
//*****************************************************************************
#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__
//*****************************************************************************
// includes
//*****************************************************************************

//*****************************************************************************
// defines
//*****************************************************************************

/******************************************************************************
* variables
******************************************************************************/

//*****************************************************************************
// external variables
//*****************************************************************************

//*****************************************************************************
// enums
//*****************************************************************************
//return error codes
typedef enum
{
    ER_OK,
    ER_FAIL,
    ER_TRUE,
    ER_FALSE,
    ER_YES,
    ER_NO,
    ER_ENABLED,
    ER_NOT_ENABLED,
    ER_ON,
    ER_OFF,
    ER_SET,
    ER_NOT_SET,
    ER_SKIP,
    ER_IGNORE,
    ER_CLEAR,
    ER_FULL,
    ER_EVENT,
    ER_SEND,
    ER_RCV,
    ER_TIMEOUT,
    ER_BUSY,
    ER_DONE,
    ER_PARAM,
    ER_PARAM1,
    ER_PARAM2,
    ER_PARAM3,
    ER_BUFF,
    ER_BUFF_SIZE,
    ER_NO_BUFF,
    ER_NO_BUFF_AVAILABLE,
    ER_LEN,
    ER_SIZE,
    ER_NOMEM,
    ER_OVERWRITE,
    ER_CLOSED,
    ER_CLOSE,
    ER_OPENED,
    ER_OPEN,
    ER_NODATA,
    ER_COMMAND,
    ER_REQUEST,
    ER_MODES,
    ER_MODE,
    ER_TRANSMIT,
    ER_RECEIVE,
    ER_VALID,
    ER_INVALID,
    ER_INVALID_RESPONSE,
    ER_RESPONSE,
    ER_CONNECTED,
    ER_NOT_CONNECTED,
    ER_SPEED,
    ER_UNKNOWN,
    ER_UNKNOWN_PARAM,
    ER_UNKNOWN_INTERFACE,
    ER_UNKNOWN_SETTING,
    ER_UNKNOWN_VALUE,
    ER_READY,
    ER_NOT_READY,
    ER_NONE,
    ER_FOUND,
    ER_NOT_FOUND,
    ER_CREATE,
    ER_MAKE,
}ERROR_CODE;

//*****************************************************************************
// structures
//*****************************************************************************

//*****************************************************************************
// external functions
//*****************************************************************************

//*****************************************************************************
// function declarations
//*****************************************************************************

#endif //__ERROR_CODES_H__
