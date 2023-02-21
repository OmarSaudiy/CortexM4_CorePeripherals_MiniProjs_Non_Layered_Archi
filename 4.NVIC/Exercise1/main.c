
/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of NIVC Exercise 1
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

void init_PINF2(void)
{
    /*Initialization of GPIOF PIN2 as O/P PIN*/
  volatile uint8 delay = 0;
  SYSCTL_REGCGC2_REG |= 1 << 5 ;
  delay = SYSCTL_REGCGC2_REG;
  GPIO_PORTF_AFSEL_REG &= ~(1<<2);      
  GPIO_PORTF_DEN_REG |= 1 << 2;
  GPIO_PORTF_DIR_REG |= (1 << 2) ;
  GPIO_PORTF_DATA_REG &= ~(1 << 2) ;

}

void External_INT_PINF4(void)
{
    /*Initialization of GPIOF PIN4 as I/P PIN*/
  GPIO_PORTF_AFSEL_REG &= ~(1<<4);      
  GPIO_PORTF_DEN_REG |= 1 << 4;
  GPIO_PORTF_DIR_REG &= ~(1 << 4) ;
  GPIO_PORTF_PUR_REG   |= (1<<4);  
  
    /*Initialization of GPIOF PIN4 as External Interrupt*/
  GPIO_PORTF_IS_REG  &= ~(1 << 4);
  GPIO_PORTF_IBE_REG &= ~(1 << 4);
  GPIO_PORTF_IEV_REG &= ~(1 << 4);
  GPIO_PORTF_IM_REG |= (1 << 4);
  
  NVIC_PRI7_REG = (NVIC_PRI7_REG & (~(0b111 << 21))) | ( 5 << 21); 
  NVIC_EN0_REG         |= 1 << 30;   

}



void GPIOPortF_Handler (void)
{
    GPIO_PORTF_DATA_REG |= (1 << 2) ;
    GPIO_PORTF_ICR_REG  |= (1 << 2) ;
}

int main(void)
{
  Enable_Exceptions();
  init_PINF2();
  External_INT_PINF4();
  
  while(1);

  return 0;
}
