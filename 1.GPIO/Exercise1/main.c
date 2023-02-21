/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of GPIO Exercise 1
 *
 * Author: Omar Ahmed
 *
 *******************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "tm4c123gh6pm_registers.h"

void switch_led_init(void){
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
}

int main()
{
  switch_led_init();
  while(1){
    
    if(BIT_IS_CLEAR(GPIO_PORTF_DATA_REG,0)){
      GPIO_PORTF_DATA_REG |= (1<<1);
    }
    else{
      GPIO_PORTF_DATA_REG &= ~(1<<1);
    }
  }
  return 0;
}
