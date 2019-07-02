#include "HAL_device.h"
#include "..\..\MODULE\fsm\compiler.h"
#include ".\led.h"

#define TOP     (0x01FF)

void gpio_init(void)
{
    RCC ->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN; 	 
    GPIOA ->CRL &= 0XFFFFFFF0;
    GPIOA ->CRH &= 0X0FFFFFFF;
    GPIOB ->CRL &= 0XFF000FFF;
    GPIOB ->CRH &= 0XFFFF00FF;
    GPIOC ->CRH &= 0XFF0FFFFF;

    GPIOA ->CRL |= GPIO_CRL_CNF0_1;  
	GPIOA ->CRH |= GPIO_CRH_MODE15;  
    GPIOA ->ODR |= GPIO_ODR_ODR15;

    GPIOB ->CRL |= GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5;
    GPIOB ->ODR |= GPIO_ODR_ODR3 | GPIO_ODR_ODR4 | GPIO_ODR_ODR5;
    GPIOB ->CRH |= GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1;   
    GPIOB ->ODR |= GPIO_ODR_ODR10 | GPIO_ODR_ODR11;

    GPIOC ->CRH |= GPIO_CRH_CNF13_1;   
    GPIOC ->ODR |= GPIO_ODR_ODR13;
}



/*! \brief set the 16-level led gradation
 *! \param hwLevel gradation
 *! \return none
 */
static void set_led_gradation(uint16_t hwLevel)
{
    static uint16_t s_hwCounter = 0;

    if (hwLevel >= s_hwCounter) {
        LED1_ON();
    } else {
        LED1_OFF();
    }

    s_hwCounter++;
    s_hwCounter &= TOP;
}
 
void breath_led(void)
{
    static uint16_t s_hwCounter = 0;
    static int16_t s_nGray = (TOP >> 1);
    
    s_hwCounter++;
    if (!(s_hwCounter & (_BV(10)-1))) {
        s_nGray++; 
        if (s_nGray == TOP) {
            s_nGray = 0;
        }
    }
    set_led_gradation(ABS(s_nGray - (TOP >> 1)));
}

