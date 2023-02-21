/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of GPIO Exercise 2
 *
 * Author: Omar Ahmed
 *
 *******************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "tm4c123gh6pm_registers.h"
#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 762

void Delay_MS(unsigned long long n){
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

void switch_led_init(void){
  volatile uint8 delay = 0;             
  SYSCTL_REGCGC2_REG |= 1 << 5;
  delay = SYSCTL_REGCGC2_REG;
  GPIO_PORTF_DEN_REG |= (1 << 4) | (1 << 2);
  GPIO_PORTF_PUR_REG |= 1 << 4;
  GPIO_PORTF_DIR_REG |= 1 << 2;
  GPIO_PORTF_DIR_REG &= ~(1 << 4);
}

int main(){
  switch_led_init();
  uint8 flag = 0;
  while(1)
  {
    if(BIT_IS_CLEAR(GPIO_PORTF_DATA_REG,4))
    {
      Delay_MS(10);
          if(BIT_IS_CLEAR(GPIO_PORTF_DATA_REG,4))
          {
            if(flag == 0)
            {
              GPIO_PORTF_DIR_REG ^= 1 << 2;
              flag = 1;
            }
          }

    }
    else
    {
      flag = 0;
    }
    
  }
  
  return (0);
}
