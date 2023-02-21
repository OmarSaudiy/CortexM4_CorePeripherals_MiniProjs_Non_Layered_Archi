/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of Systick Exercise 2
 *
 * Author: Omar Ahmed
 *
 *******************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "tm4c123gh6pm_registers.h"

void systick_init(uint16 Time_ms)
{
  SYSTICK_CTRL_REG = (SYSTICK_CTRL_REG & 0) | (1 << 2);
  SYSTICK_RELOAD_REG = ((Time_ms * (16000)) - 1);
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
  GPIO_PORTF_DEN_REG |= (1 << 0) |(1 << 1)| (1 << 2);
  GPIO_PORTF_DIR_REG |= (1 << 0) |(1 << 1)| (1 << 2);
  GPIO_PORTF_LOCK_REG = 0x4C4F434B;
  GPIO_PORTF_CR_REG |= 1 << 0;
  GPIO_PORTF_LOCK_REG = 0;
}

int main(){
  _3leds_init();
  systick_init(1000);
  while(1)
  {
    for(uint8 counter = 0 ; counter < 3 ; counter++)
    {
      GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF8) | (1 << counter );
      systick_poll();
    }
  }
  
  return (0);
}