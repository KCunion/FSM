#ifndef __LED_H
#define __LED_H	 
#include "HAL_device.h"


#define LED4_ON()   do{GPIOA ->BRR = 0x8000;}while(0)    // PA15
#define LED4_OFF()  do{GPIOA ->BSRR = 0x8000;}while(0)
#define LED3_ON()   do{GPIOB ->BRR = 0x0008;}while(0)    // PB3
#define LED3_OFF()  do{GPIOB ->BSRR = 0x0008;}while(0)
#define LED2_ON()   do{GPIOB ->BRR = 0x0020;}while(0)    // PB4
#define LED2_OFF()  do{GPIOB ->BSRR = 0x0020;}while(0)
#define LED1_ON()   do{GPIOB ->BRR = 0x0010;}while(0)    // PB5
#define LED1_OFF()  do{GPIOB ->BSRR = 0x0010;}while(0)


#define IS_KEY_REST_DOWN()	    (!(GPIOC ->IDR >> 13 &1 ))      //PC13
#define IS_KEY_REST_UP()	    (GPIOC ->IDR >> 13 & 1)
#define IS_KEY_WEAKUP_DOWN()	(!(GPIOA ->IDR & 1))            //PA0
#define IS_KEY_WEAKUP_UP()	    (GPIOA ->IDR &1 )
#define IS_KEY1_DOWN()	        (!(GPIOB ->IDR >> 10 & 1))        //PB10
#define IS_KEY1_UP()	        (GPIOB ->IDR >> 10 & 1)
#define IS_KEY2_DOWN()	        (!(GPIOB ->IDR >> 11 & 1))        //PB11
#define IS_KEY2_UP()	        (GPIOB ->IDR >> 11 & 1)

extern void gpio_init(void);
#endif
