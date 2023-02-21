/******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: Implementation of GPIO Exercise 3
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
  while(1)
  {
    for(uint8 counter = 0 ; counter < 3 ; counter++)
    {
      GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF8) | (1 << counter );
      Delay_MS(1000);
    }
  }
  
  return (0);
}
