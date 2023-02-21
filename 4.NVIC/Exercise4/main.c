/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of NIVC Exercise 4
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

volatile uint8 tick = 0;

void PLL_Init(void)
{
    SYSCTL_RCC_REG = (SYSCTL_RCC_REG &(~(0b11111 << 6)))) | (0x15 << 6);
    SYSCTL_RCC2_REG |= 1 << 31;
    SYSCTL_RCC2_REG = (SYSCTL_RCC2_REG & (~(0b1111111 << 22))) | (4 << 22);
    SYSCTL_RCC2_REG &= ~(1 << 13);
    /* Bypass PLL while initializing, Don’t use PLL while init. Precision internal oscillator is used as OSCSRC2 = 0x01 */
    SYSCTL_RCC2_REG |= 1 << 11;
    SYSCTL_RCC2_REG = (SYSCTL_RCC2_REG & (~(7 << 4))) 
  
    
    while(BIT_IS_CLEAR(SYSCTL_RIS_REG,6));
  
    SYSCTL_RCC2_REG &= ~(1 << 11));
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

void init_Systick(void)
{
  SYSTICK_CTRL_REG &= ~1;
  SYSTICK_CURRENT_REG  
  SYSTICK_RELOAD_REG = 15999999; /*Half Second*/
  
  NVIC_SYSTEM_PRI3_REG =  (NVIC_PRI3_REG & (~(7 << 29))) | (2 << 29) ;
    
  SYSTICK_CTRL_REG |= 7;
}

void SysTick_Handler(void)
{
  if(tick == 15)
  {
    tick = 0;
  }
  tick++;
}

int main()
{
  PLL_Init();
  init_PINF123();
  Enable_Exceptions();
  init_Systick();

  
  while(1)
  {
    if(tick == 5 || tick == 10 || tick == 15 )
    {
    for(uint8 static counter = 0 ; counter < 3 ; counter++)
      {
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF8) | (1 << counter );
        if( counter == 2)
        {
          counter = 0;
        }
      }
    }
   
  }
  return 0;
}
