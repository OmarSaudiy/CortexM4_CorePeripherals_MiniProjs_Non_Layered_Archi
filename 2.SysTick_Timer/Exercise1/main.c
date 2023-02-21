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

void switch_led_init(void)
{
  volatile uint8 delay = 0;             
  SYSCTL_REGCGC2_REG |= 1 << 5;
  delay = SYSCTL_REGCGC2_REG;
  GPIO_PORTF_DEN_REG |= 3;
  GPIO_PORTF_PUR_REG |= 1 << 0;
  GPIO_PORTF_DIR_REG |= 1 << 1;
  GPIO_PORTF_DIR_REG &= ~(1 << 0);
  GPIO_PORTF_LOCK_REG = 0x4C4F434B;
  GPIO_PORTF_CR_REG  |= 1 << 0;
  GPIO_PORTF_LOCK_REG = 0;
  GPIO_PORTF_DATA_REG &= ~(1 << 1);
}
int main(){
  switch_led_init();
  systick_init(500);
  
  while(1)
  {
     if(BIT_IS_CLEAR(GPIO_PORTF_DATA_REG,0)){
      GPIO_PORTF_DATA_REG ^= (1<<1);
      systick_poll();
    }
    else
    {
      GPIO_PORTF_DATA_REG &= ~(1 << 1);
    }
  }
  
  return (0);
}