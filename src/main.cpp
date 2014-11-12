//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include "diag/Trace.h"
#include "i2c.hpp"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
#include "math.h"

#define ADDRESS       0x81   // READ ADDRESS
#define DISTANCE_REG  0x5E
#define SHIFT         0x35

uint8_t distance_raw[2]={0};
int shift[2]={0};	// Value in shift bit register
int distance_cm=0;

// ----------------------------------------------------------------------------
//
// Semihosting STM32F4 empty sample (trace via STDOUT).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the STDOUT output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.

  I2Cx_COM::Instance().I2Cx_COM_INIT();

//  while(1){
//
//  }

  I2Cx_COM::Instance().ReadBytes((uint16_t)ADDRESS, SHIFT, I2C_MEMADD_SIZE_8BIT, (uint8_t *)shift, 1);


//  while( HAL_I2C_Mem_Read_DMA(&I2cHandle, (uint16_t)ADDRESS, SHIFT, I2C_MEMADD_SIZE_8BIT, (uint8_t *)shift, 1 ) != HAL_OK ){
//
//  }

  // Infinite loop
  while (1)
    {
//      /*##-3- Wait for the end of the transfer ###################################*/
//      /*  Before starting a new communication transfer, you need to check the current
//          state of the peripheral; if it’s busy you need to wait for the end of current
//          transfer before starting a new one. */
//      if(HAL_I2C_GetState(&I2cHandle) == HAL_I2C_STATE_READY){
//	while( HAL_I2C_Mem_Read_DMA(&I2cHandle, (uint16_t)ADDRESS, DISTANCE_REG, I2C_MEMADD_SIZE_8BIT, distance_raw, 2 ) != HAL_OK ){
//
//	}
//      }

      I2Cx_COM::Instance().ReadBytes((uint16_t)ADDRESS, DISTANCE_REG, I2C_MEMADD_SIZE_8BIT, distance_raw, 2);

      distance_cm = (distance_raw[0] * 16 + distance_raw[1])/16/(int)pow(2,shift[0]); // Calculate the range in CM
      trace_printf("%d\n", distance_cm);
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
