#include "uart.h"

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
        ptPrint->chStates = 0;
        ptPrint->pchString = pchString;
        return true;
    }
    return false;
}

/*! \brief serial out a byte
 *! \param chByte Bytes to be sent
 *! \return true or false
 */
bool serial_out(uint8_t chByte)
{
    if ((UART1 ->CSR & ((uint16_t)0x0001)) != 0) {
        UART1 ->TDR = (chByte & (uint16_t)0x00FF);      
        return true;
    }
    return false;
}
/*! \brief serial in a byte
 *! \param pchByte Byte Pointers
 *! \return true or false
 */
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

/*! \brief print the string
 *! \param ptPrintStr  string
 *! \return fsm state
 */
fsm_rt_t print_string(print_str_t* ptPrintStr)
{
    if (ptPrintStr == NULL) {
        return fsm_rt_err;
    }
    enum {
        START = 0,
        PRINT_ON,
        PRINT_CPL
    };
    
    switch (ptPrintStr->chStates ) {
        case START:
            ptPrintStr->chStates = PRINT_ON;
            //break;
        case PRINT_ON:
            if (serial_out(*ptPrintStr->pchString)) {
                ptPrintStr->chStates = PRINT_CPL;
            } else {
                break;
            }
            //break;
        case PRINT_CPL:
             if ('\0' == *(++ ptPrintStr->pchString)) {
                    return fsm_rt_cpl;
                } else {
                    ptPrintStr->chStates = PRINT_ON;
                }
            break;
    }
    return fsm_rt_on_going;
 }
