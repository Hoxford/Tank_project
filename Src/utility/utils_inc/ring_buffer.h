/******************************************************************************
*
* ring_bufer.h - Ring buffer utility include file.
* Copyright (c) notice
*
******************************************************************************/
#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define RB_HANDLE    uint32_t

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

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* Name: eRingBuff_Create
* Description:
*   Creates a ring buffer based upon the function parameters. This function
*   will return an error code depending if it was able to complete the
*   creation of the ring buffer system or not. For this type of ring buffer the
*   data management is handled entirely by the utility. Writing to the
*   individual buffers is handled by the utility. Getting a buffer from the
*   ring will copy it into the callers local buffer and free the buffer in the
*   ring for next use.
*
* Parameters:
*   (RB_HANDLE *) pHandle: Pointer to the applications local type RB_HANDLE
*     that the creator function will assign a handle if successful. NULL
*     if otherwise.
*   (uint32_t) ui32NumOfBuffers: Number of individual buffers the ring buffer
*     will have.
*   (uint32_t) ui32BuffSize: Size in bytes each buffer will be.
*
* Returns:
*   (ERROR_CODE):
*     = ER_OK:
*     = ER_FAIL:
*     = ER_NO_MEM:
*     = ER_PARAM1:
*     = ER_PARAM2,
*     = ER_PARAM3,
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE eRingBuff_Create(RB_HANDLE * pHandle, uint32_t ui32NumOfBuffers, uint32_t ui32BuffSize);

/******************************************************************************
* Name: eRingBuff_Reset_All
* Description:
*   Erases every buffer in the ring.
*
* Parameters:
*   (RB_HANDLE) Handle: Handle of the ring buffer to reset.
*
* Returns:
*   (ERROR_CODE):
*     = ER_OK: Ring buffer reset.
*     = ER_FAIL: Ring buffer failed to reset.
*     = ER_PARAM: Invalid ring buffer handle.
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE eRingBuff_Reset_All(RB_HANDLE Handle);

/******************************************************************************
* Name: eRingBuff_Destroy
* Description:
*   Destroys and deallocates the ring buffer.
*
* Parameters:
*   (RB_HANDLE) Handle: The ring buffer handle to destroy.
*
* Returns:
*   (ERROR_CODE):
*     = ER_OK:
*     = ER_FAIL:
*     = ER_PARAM:
*
* Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE eRingBuff_Destroy(RB_HANDLE Handle);

/******************************************************************************
* Name: eRingBuff_Push
* Description:
*   Fills the next available buffer with the data provided via the parameters.
*
* Parameters:
*   (RB_HANDLE) Handle: Handle of the ring buffer to fill
*   (uint8_t *) pBuffer: pointer to the buffer containing the data to be copied
*     into the ring buffer.
*
* Returns:
*   (ERROR_CODE): description
*     = ER_OK:
*     = ER_FAIL:
*     = ER_INVALID: Wrong api call for the handle
*     = ER_PARAM1:
*     = ER_PARAM2:
*     = ER_PARAM3:
*     = ER_LEN:
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE eRingBuff_Push(RB_HANDLE Handle, uint8_t * pBuffer, uint32_t ui32Len);

/******************************************************************************
* Name: eRingBuff_Pop
* todo:Description:
*
* Parameters:
*   (RB_HANDLE) Handle: The ring buffer handle
*   (uint8_t *) pBuffer: Pointer to applications buffer to fill from the ring
*     buffer.
*
* Returns:
*   (ERROR_CODE):
*     = ER_OK:
*     = ER_FAIL:
*     = ER_NO_BUFF: No buffer to get, ring buffer is empty
*     = ER_PARAM: The ring buffer handle is not valid
*     = ER_BUFF: The pointer to the buffer to fill is invalid.
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE eRingBuff_Pop(RB_HANDLE Handle, uint8_t * pBuffer);

/******************************************************************************
* Name: eRingBuff_Peek
* todo:Description:
*
* Parameters:
*   (RB_HANDLE) Handle: The ring buffer handle
*   (uint8_t *) pBuffer: Pointer to buffer to fill from the ring buffer.
*
* Returns:
*   (ERROR_CODE):
*     = ER_OK:
*     = ER_FAIL:
*     = ER_NO_BUFF: No buffer to get, ring buffer is empty
*     = ER_PARAM: The ring buffer handle is not valid
*     = ER_BUFF: The pointer to the buffer to fill is invalid.
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE eRingBuff_Peek(RB_HANDLE Handle, uint8_t * pBuffer);

/******************************************************************************
* Name: eRingBuff_App_Create
* Description:
*   Creates an application reliant ring buffer based upon the function
*   parameters. This function will return an error code depending if it was
*   able to complete the creation of the ring buffer system or not. For an
*   app dependent ring buffer, the data write/read/free of the buffer is
*   handled by the app that created it. This is so the app can have full
*   control of the contents of each individual buffer but the utility still
*   handles buffer creation/cycling/reset/destruction.
*
*todo: Parameters:
*   (type) name: description (in order from left to right)
*     = value(optional): description if value set
*     -or-
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
ERROR_CODE eRingBuff_App_Create(RB_HANDLE * pHandle, uint32_t ui32NumOfBuffers, uint32_t ui32BuffSize);

/******************************************************************************
* Name: eRingBuff_App_Get_Buff
* Description:
*   Returns the address to the next available buffer in the pointer to a
*   pointer parameter. If no buffer is available or otherwise, the pointer
*   pointer is set to null and a corresponding error code is returned. Each
*   time this function is called the ring will cycle to the next free ring
*   buffer. The contents of the returned buffer address are managed by the
*   calling app.
*
* Parameters:
*   (RB_HANDLE) Handle:
*   (uint8_t **) ppBuffer
*
* Returns:
*   (ERROR_CODE): description
*     = ER_OK:
*     = ER_FAIL:
*     = ER_INVALID: Wrong api call for the handle.
*     = ER_PARAM1:
*     = ER_PARAM2:
*     = ER_FULL: All buffers are currently full.
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE eRingBuff_App_Get_Buff(RB_HANDLE Handle, uint8_t ** ppBuffer);

/******************************************************************************
* Name: eRingBuff_App_Read_Buff
*todo: Description:
*   [put description here]
*
*todo: Parameters:
*   (RB_HANDLE) Handle:
*   (uint8_t *) pBuffer:
*
* Returns:
*   (ERROR_CODE): description
*     = ER_OK:
*     = ER_FAIL:
*     = ER_INVALID: Wrong api call for the handle.
*     = ER_PARAM1:
*     = ER_PARAM2:
*     = ER_NO_BUFF: No data to read out of the ring buffer.
*
* todo:Example:
* void foo(void)
* {
*   //Function usage
*   iFilename_or_abreviation_funciton()
* }
******************************************************************************/
ERROR_CODE eRingBuff_App_Read_Buff(RB_HANDLE Handle, uint8_t * pBuffer);

/******************************************************************************
* Name: eRingBuff_App_Peek_Buff
* Description:
*   Returns the address to the next full buffer in the pointer to a pointer
*   parameter. If no buffer is available or otherwise, the pointer pointer is
*   set to null and a corresponding error code is returned. This function is
*   will not cause the ring buffer to cycle. The contents of the returned
*   buffer address are managed by the calling app.
*
* Parameters:
*   (RB_HANDLE) Handle:
*   (uint8_t **) ppBuffer
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
ERROR_CODE eRingBuff_App_Peek_Buff(RB_HANDLE Handle, uint8_t ** ppBuffer);

#endif //__RING_BUFFER_H__
