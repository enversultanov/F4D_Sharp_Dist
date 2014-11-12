//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include "diag/Trace.h"
#include "math.h"
#include "i2c.hpp"
#include "uart.hpp"
//#include "stm32f4xx_hal.h"
//#include "stm32f4_discovery.h"

#define ADDRESS       0x81   // READ ADDRESS
#define DISTANCE_REG  0x5E
#define SHIFT         0x35

uint8_t distance_raw[2]={0};
int shift[2]={0};	// Value in shift bit register
uint8_t distance_cm=0;

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

  /* I2C Hardware Init */
  I2Cx_COM::Instance().InitHardware();

  /* UART Hardware Init */
  UART_COM::Instance().InitHardware();

  UART_COM::Instance().WriteBytes((uint8_t*)"This is an initial UART Message!!\r\n", COUNTOF("This is an initial UART Message!!\r\n") );
  while(UART_COM::Instance().UartReady != SET){}
  /* Reset transmission flag */
  UART_COM::Instance().UartReady = RESET;

  I2Cx_COM::Instance().ReadBytes((uint16_t)ADDRESS, SHIFT, I2C_MEMADD_SIZE_8BIT, (uint8_t *)shift, 1);

  // Infinite loop
  while (1)
    {
      char buf[10]={0};
      I2Cx_COM::Instance().ReadBytes((uint16_t)ADDRESS, DISTANCE_REG, I2C_MEMADD_SIZE_8BIT, distance_raw, 2);

      distance_cm = (distance_raw[0] * 16 + distance_raw[1])/16/(int)pow(2,shift[0]); // Calculate the range in CM


      UART_COM::Instance().WriteBytes((uint8_t*)buf, sprintf(buf,"The distance is: %u\r\n", distance_cm) );
      while(UART_COM::Instance().UartReady != SET){}
      /* Reset transmission flag */
      UART_COM::Instance().UartReady = RESET;
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
