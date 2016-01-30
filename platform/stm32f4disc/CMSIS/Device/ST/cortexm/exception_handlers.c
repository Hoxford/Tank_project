//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

//todo: find a better file name for this instead of exception handler i.e. stm32f4xx_it.c .h

// ----------------------------------------------------------------------------

#include "cortexm/ExceptionHandlers.h"
#include "cmsis_device.h"
#include "core_cm4.h"

// ----------------------------------------------------------------------------

extern void
__attribute__((noreturn))
_start(void);

// ----------------------------------------------------------------------------
// Default exception handlers. Override the ones here by defining your own
// handler routines in your application code.
// ----------------------------------------------------------------------------

#if defined(DEBUG)

// The DEBUG version is not naked, to allow breakpoints at Reset_Handler
void __attribute__ ((section(".after_vectors"),noreturn))
Reset_Handler (void)
  {
    _start ();
  }

#else

// The Release version is optimised to a quick branch to _start.
void __attribute__ ((section(".after_vectors"),naked))
Reset_Handler(void)
{
  asm volatile
  (
      " ldr     r0,=_start \n"
      " bx      r0"
      :
      :
      :
  );
}

#endif

void __attribute__ ((section(".after_vectors"),weak))
NMI_Handler(void)
{
  while (1)
    {
    }
}

void __attribute__ ((section(".after_vectors"),weak))
HardFault_Handler(void)
{
//  __ASM volatile("BKPT #01");
  __ASM volatile("BKPT 0x00A8");
  while (1){}
}

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

void __attribute__ ((section(".after_vectors"),weak))
MemManage_Handler(void)
  {
    while (1)
      {
      }
  }

void __attribute__ ((section(".after_vectors"),weak))
BusFault_Handler(void)
  {
    while (1)
      {
      }
  }

void __attribute__ ((section(".after_vectors"),weak))
UsageFault_Handler(void)
  {
    while (1)
      {
      }
  }

#endif

//void __attribute__ ((section(".after_vectors"),weak))
//SVC_Handler(void)
//{
////  while (1)
//    {
//    }
//  __ASM volatile("BKPT 0x00A8");
//}

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

void __attribute__ ((section(".after_vectors"),weak))
DebugMon_Handler(void)
{
  while (1)
    {
    }
}

#endif

//void __attribute__ ((section(".after_vectors"),weak))
//PendSV_Handler(void)
//{
//  while (1)
//    {
//    }
//}

//void __attribute__ ((section(".after_vectors"),weak))
//SysTick_Handler(void)
//{
//  // DO NOT loop, just return.
//  // Useful in case someone (like STM HAL) inadvertently enables SysTick.
//  ;
//}
