/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of NIVC Exercise 3
 *
 * Author: Omar Ahmed
 *
 *******************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "tm4c123gh6pm_registers.h"

/* Enable Exceptions ... This Macro enable IRQ interrupts, Programmble Systems Exceptions and Faults by clearing the I-bit in the PRIMASK. */
#define Enable_Exceptions()    __asm("CPSIE I")

/* Disable Exceptions ... This Macro disable IRQ interrupts, Programmble Systems Exceptions and Faults by clearing the I-bit in the PRIMASK. */
#define Disable_Exceptions()   __asm("CPSID I")

/* Enable Faults ... This Macro enable Faults by clearing the F-bit in the FAULTMASK */
#define Enable_Faults()        __asm("CPSIE F")

/* Disable Faults ... This Macro disable Faults by setting the F-bit in the FAULTMASK */
#define Disable_Faults()       __asm("CPSID F") 

void init_PINF1(void)
{
    /*Initialization of GPIOF PIN1 as O/P PIN*/
  volatile uint8 delay = 0;
  SYSCTL_REGCGC2_REG |= 1 << 5 ;
  delay = SYSCTL_REGCGC2_REG;
  GPIO_PORTF_AFSEL_REG &= ~(1<<1);      
  GPIO_PORTF_DEN_REG |= 1 << 1;
  GPIO_PORTF_DIR_REG |= (1 << 1) ;
  GPIO_PORTF_DATA_REG &= ~(1 << 1) ;
}

void init_Systick(void)
{
  SYSTICK_CTRL_REG &= ~1;
  SYSTICK_CURRENT_REG  
  SYSTICK_RELOAD_REG = 7999999; /*Half Second*/
  
  NVIC_SYSTEM_PRI3_REG =  (NVIC_PRI3_REG & (~(7 << 29))) | (2 << 29) ;
    
  SYSTICK_CTRL_REG |= 7;
}

void SysTick_Handler(void)
{
  GPIO_PORTF_DATA_REG ^= 2;
}

int main(void)
{
  init_PINF1();
  init_Systick();
  Enable_Exceptions();
  
  while(1);
  
  return 0;
}
