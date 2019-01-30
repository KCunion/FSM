#include "sys.h"

void uart_init(void)
{
    uint32_t wtempBaud;	
    RCC ->AHBENR |= 1 << 17;
    RCC ->APB2ENR |= 1 << 14;
    
    RCC ->APB2RSTR |= 1 << 14;
    RCC ->APB2RSTR &= ~(1 << 14);

    wtempBaud = (96000000 / 16) / (9600);
    UART1 ->FRA =(96000000 / 16) % (9600);
    UART1 ->BRR = wtempBaud;
    UART1 ->CCR |=0X30;

    UART1 ->GCR = 0X19;
    UART1 ->CCR |= 1 << 5;
    UART1 ->IER = 0X2;
    
    GPIOA ->CRH &= 0XFFFFF00F;
    GPIOA ->CRH |= 0X000008B0;    
}
bool print_str_init(print_str_t* ptPrint,int8_t* pchString)
{
    if (ptPrint != NULL && pchString != NULL) {
        ptPrint->tStates = print_start;
//        ptPrint->hwPosition = 0;
        ptPrint->pchString = pchString;
        return true;
    }
    return false;
}

print_st_t get_print_str_state(print_str_t* ptPrint)
{
    if (ptPrint != NULL) {
        return ptPrint->tStates;
    }
    return print_err;
}

bool set_print_str_state(print_str_t* ptPrint,print_st_t ptState)
{
    if (ptPrint != NULL) {
        ptPrint->tStates = ptState;
        return true;
    }
    return false;
}
bool serial_out(uint8_t chByte)
{
    if ((UART1 ->CSR & ((uint16_t)0x0001)) != 0) {
        UART1 ->TDR = (chByte & (uint16_t)0x00FF);      
        return true;
    }
    return false;
}

bool serial_in(uint8_t *pchByte)
{
    if (pchByte != NULL) {
        if ((UART1 ->CSR & ((uint16_t)0x0002)) != 0) {
            *pchByte = UART1 ->RDR & (uint8_t)0xFF;
            return true;
        }
    }
    return false;
}
