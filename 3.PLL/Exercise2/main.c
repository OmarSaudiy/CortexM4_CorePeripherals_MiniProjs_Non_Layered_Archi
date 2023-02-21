/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of Systick Exercise 1
 *
 * Author: Omar Ahmed
 *
 *******************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "tm4c123gh6pm_registers.h"

#define SYSCTL_RCC2_USERCC2_MASK     0x80000000  /* USERCC2 Bit MASK */
#define SYSCTL_RCC2_BYPASS2_MASK     0x00000800  /* PLL BYPASS2 Bit MASK */
#define SYSCTL_RCC_XTAL_MASK         0x000007C0  /* XTAL Bits MASK */
#define SYSCTL_RCC_XTAL_8MHZ         0x00000380  /* 8 MHz Crystal Value */
#define SYSCTL_RCC_XTAL_16MHZ        0x00000540  /* 16 MHz Crystal Value */
#define SYSCTL_RCC2_OSCSRC2_MASK     0x00000070  /* OSCSRC2 Bits MASK */
#define SYSCTL_RCC2_OSCSRC2_MOSC     0x00000000  /* MOSC(Main Oscillator) value */
#define SYSCTL_RCC2_PWRDN2_MASK      0x00002000  /* PWRDN2 Bit MASK */
#define SYSCTL_RCC2_DIV400_MASK      0x40000000  /* DIV400 Bit MASK to Divide PLL as 400 MHz vs. 200 */
#define SYSCTL_RCC2_SYSDIV2_MASK     0x1FC00000  /* SYSDIV2 Bits MASK */
#define SYSCTL_RIS_PLLLRIS_MASK      0x00000040  /* PLLLRIS Bit MASK */
#define SYSCTL_RCC2_SYSDIV2_BIT_POS     22       /* SYSDIV2 Bits Postion start from bit number 22 */
#define SYSDIV2_VALUE 39

/* configure the system to get its clock from the PLL with Frquency 80Mhz */ 
void PLL_Init(void)
{
    /* 1) Configure the system to use RCC2 for advanced features
          such as 400 MHz PLL and non-integer System Clock Divisor */
    SYSCTL_RCC2_REG |= SYSCTL_RCC2_USERCC2_MASK;
  
    /* 2) Bypass PLL while initializing, Don’t use PLL while init. Precision internal oscillator is used as OSCSRC2 = 0x01 */
    SYSCTL_RCC2_REG |= SYSCTL_RCC2_BYPASS2_MASK;
  
    /* 3) Select the crystal value and oscillator source */
    SYSCTL_RCC_REG  &= ~SYSCTL_RCC_XTAL_MASK;     /* clear XTAL field */
    SYSCTL_RCC_REG  |= SYSCTL_RCC_XTAL_16MHZ;     /* Set the XTAL bits for 16 MHz crystal */
  
    SYSCTL_RCC2_REG &= ~SYSCTL_RCC2_OSCSRC2_MASK; /* clear oscillator source field (OSCSRC2 bits) */
    SYSCTL_RCC2_REG |= SYSCTL_RCC2_OSCSRC2_MOSC;  /* configure for main oscillator source */
  
    /* 4) Activate PLL by clearing PWRDN2 */
    SYSCTL_RCC2_REG &= ~SYSCTL_RCC2_PWRDN2_MASK;
  
    /* 5) Set the desired system divider and the system divider least significant bit */
    SYSCTL_RCC2_REG |= SYSCTL_RCC2_DIV400_MASK;  /* use 400 MHz PLL */
    
    SYSCTL_RCC2_REG  = (SYSCTL_RCC2_REG & ~SYSCTL_RCC2_SYSDIV2_MASK)        /* clear system clock divider field */
                       | (SYSDIV2_VALUE<<SYSCTL_RCC2_SYSDIV2_BIT_POS);      /* configure for 80MHz clock */
  
    /* 6) Wait for the PLL to lock by polling PLLLRIS bit */
    while(!(SYSCTL_RIS_REG & SYSCTL_RIS_PLLLRIS_MASK));
  
    /* 7) Enable use of PLL by clearing BYPASS2 */
    SYSCTL_RCC2_REG &= ~SYSCTL_RCC2_BYPASS2_MASK;
}

void systick_init(uint16 Time_ms)
{
  SYSTICK_CTRL_REG = (SYSTICK_CTRL_REG & 0) | (1 << 2);
  SYSTICK_RELOAD_REG = ((Time_ms * (10000)) - 1);
  SYSTICK_CURRENT_REG;                 /* Clear current count by accessing the register */
}

void systick_poll(void)
{
    SYSTICK_CURRENT_REG;                 /* Clear current count by accessing the register */
    SYSTICK_CTRL_REG |= 1 << 0;
    while(BIT_IS_CLEAR(SYSTICK_CTRL_REG,16));

}

void _3leds_init(void){
  volatile uint8 delay = 0;             
  SYSCTL_REGCGC2_REG |= 1 << 5;
  delay = SYSCTL_REGCGC2_REG;
  GPIO_PORTF_DEN_REG |= (1 << 3) |(1 << 1)| (1 << 2);
  GPIO_PORTF_DIR_REG |= (1 << 3) |(1 << 1)| (1 << 2);
   
}

int main(){
  _3leds_init();
  systick_init(1000);
  while(1)
  {
    for(uint8 counter = 1 ; counter < 4 ; counter++)
    {
      GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (1 << counter );
      systick_poll();
    }
  }
  
  return (0);
}