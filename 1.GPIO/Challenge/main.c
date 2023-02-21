/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of GPIO Challenge
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

void _2switch_3leds_init(void){
  volatile uint8 delay = 0;             
  SYSCTL_REGCGC2_REG |= 1 << 5;
  delay = SYSCTL_REGCGC2_REG;
  GPIO_PORTF_DEN_REG |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
  GPIO_PORTF_DIR_REG |= (1 << 3) |(1 << 1)| (1 << 2);
  GPIO_PORTF_DIR_REG &= 0xEE;
  GPIO_PORTF_LOCK_REG = 0x4C4F434B;
  GPIO_PORTF_CR_REG |= 1 << 0;
  GPIO_PORTF_LOCK_REG = 0;
}

int main(){
  _2switch_3leds_init();
  while(1)
  {
    if(BIT_IS_CLEAR(GPIO_PORTF_DATA_REG,4))
    {
      Delay_MS(10);
      if(BIT_IS_CLEAR(GPIO_PORTF_DATA_REG,4))
      {
        for(uint8 counter = 1; counter < 4 ; counter += 2)
        {
          GPIO_PORTF_DATA_REG ^= counter << 1 ;
          Delay_MS(500);
        }
        GPIO_PORTF_DATA_REG &= ~(3<<1);
      }
    }
    if(BIT_IS_CLEAR(GPIO_PORTF_DATA_REG,0))
    {
      Delay_MS(10);
      if(BIT_IS_CLEAR(GPIO_PORTF_DATA_REG,0))
      {
        for(uint8 counter = 1; counter < 4 ; counter += 2)
        {
          GPIO_PORTF_DATA_REG ^= counter << 2 ;
          Delay_MS(500);
        }
        GPIO_PORTF_DATA_REG &= ~(3<<2);
      }
    }
  }
  
  return (0);
}
