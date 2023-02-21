/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of NIVC Exercise 2
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

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 762

volatile uint8 flag = 0;


void Delay_MS(unsigned long n)
{
    volatile unsigned long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

void init_PINF123(void)
{
    /*Initialization of GPIOF PIN1,PIN2 and PIN3 as O/P PIN*/
  volatile uint8 delay = 0;
  SYSCTL_REGCGC2_REG |= 1 << 5 ;
  delay = SYSCTL_REGCGC2_REG;
  GPIO_PORTF_AFSEL_REG &= ~(7<<1);      
  GPIO_PORTF_DEN_REG |= 7 << 1;
  GPIO_PORTF_DIR_REG |= (7 << 1) ;
  GPIO_PORTF_DATA_REG &= ~(7 << 1) ;

}

void External_INT_PINF0(void)
{
    /*Initialization of GPIOF PIN0 as I/P PIN*/
  GPIO_PORTF_AFSEL_REG &= ~(1<<0);      
  GPIO_PORTF_DEN_REG |= 1 << 0;
  GPIO_PORTF_DIR_REG &= ~(1 << 0) ;
  GPIO_PORTF_PUR_REG   |= (1<<0);  
  
    /*Initialization of GPIOF PIN0 as External Interrupt*/
  GPIO_PORTF_IS_REG  &= ~(1 << 0);
  GPIO_PORTF_IBE_REG &= ~(1 << 0);
  GPIO_PORTF_IEV_REG &= ~(1 << 0);
  GPIO_PORTF_IM_REG |= (1 << 0);
  
  NVIC_PRI7_REG = (NVIC_PRI7_REG & (~(0b111 << 21))) | ( 5 << 21); 
  NVIC_EN0_REG         |= 1 << 30;   
  
  GPIO_PORTF_LOCK_REG = 0x4C4F434B;
  GPIO_PORTF_CR_REG  |= 1 << 0;
  GPIO_PORTF_LOCK_REG = 0;


}

void GPIOPortF_Handler (void)
{
  flag = 1;
}

int main()
{
  init_PINF123();
  External_INT_PINF4();
  Enable_Exceptions();
  
  while(1)
  {
    if(flag == 0)
    {
    for(uint8 counter = 0 ; counter < 3 ; counter++)
      {
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF8) | (1 << counter );
        Delay_MS(1000);
      }
    }
    else
    {
      GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF8) | (7 << 1 );
      Delay_MS = 5000;
      flag = 0;
    }
  }
  return 0;
}
