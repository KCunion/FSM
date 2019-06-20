#include "HAL_device.h"
#include "..\BSP\led\led.h"
#include "..\BSP\uart\uart.h"

void System_Clock_Init(void)
{
    unsigned char temp = 0;   
    RCC ->CR |= RCC_CR_HSION;  
    while(!(RCC ->CR & RCC_CR_HSIRDY));
    RCC ->CFGR = RCC_CFGR_PPRE1_2;
    
    RCC ->CFGR &= ~RCC_CFGR_PLLSRC;
    
    RCC ->CR &= ~(RCC_CR_PLLON);
    
    RCC ->CR &= ~(0x1f << 26);	
    RCC ->CR |= (8 - 1) << 26;
    
    FLASH ->ACR = FLASH_ACR_LATENCY_1 | FLASH_ACR_LATENCY_0 | FLASH_ACR_PRFTBE;
    
    RCC ->CR |= RCC_CR_PLLON;
    while(!(RCC ->CR&RCC_CR_PLLRDY));
    RCC ->CFGR &= ~RCC_CFGR_SW;
    RCC ->CFGR |= RCC_CFGR_SW_PLL;
    while  (temp != 0x02)  {
        temp = RCC->CFGR >> 2;
        temp &= 0x03;
    }   
}	

void system_init(void)
{
    gpio_init();
    uart_init();
}

