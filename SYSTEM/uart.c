#include "HAL_device.h"
#include "uart.h"
#include <stdio.h>
#include "compiler.h"
#include "ooc.h"
#include "simple_fsm.h"

void uart_init(void)
{
    uint32_t wtempBaud;	
    RCC->AHBENR |= 1 << 17;                 //使能GPIOA时钟
    RCC->APB2ENR |= 1 << 14;                //使能UART1
    
    RCC->APB2RSTR |= 1 << 14;
    RCC->APB2RSTR &= ~(1 << 14);

    wtempBaud = (96000000 / 16) / (9600);
    UART1->FRA =(96000000 / 16) % (9600);
    UART1->BRR = wtempBaud;
    UART1->CCR |=0X30;

    UART1->GCR = 0X19;
    UART1->CCR |= 1 << 5;
    UART1->IER = 0X2;
    
    GPIOA ->CRH &= 0XFFFFF00F;
    GPIOA ->CRH |= 0X000008B0;
}

/*! \brief serial out a byte
 *! \param chByte Bytes to be sent
 *! \return true or false
 */
bool serial_out(uint8_t chByte)
{
    if ((UART1->CSR & ((uint16_t)0x0001)) != 0) {
        UART1->TDR = (chByte & (uint16_t)0x00FF);      
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
        if ((UART1->CSR & ((uint16_t)0x0002)) != 0) {
            *pchByte = UART1->RDR & (uint8_t)0xFF;
            return true;
        }
    }
    return false;
}

bool print_str_init(print_str_t* ptPRN,char* pchString)
{
    if (ptPRN != NULL && pchString != NULL) {
        ptPRN->chStates = 0;
        ptPRN->pchString = pchString;
        return true;
    }
    return false;
}

#define PRINT_STRING_RESET_FSM() \
do {\
    ptPRN->chStates = START;\
} while(0)

fsm_rt_t print_string(print_str_t *ptPRN)
{
    if (ptPRN == NULL) {
        return fsm_rt_err;
    }
    enum {
        START = 0,
        CHECK_EMPTY,
        PRINT_ON
    };
    
    switch (ptPRN->chStates ) {
        case START:
            ptPRN->chStates = CHECK_EMPTY;
            //break;
        case CHECK_EMPTY:
            if ('\0' == *ptPRN->pchString) {
                PRINT_STRING_RESET_FSM();
                return fsm_rt_cpl;
            } else {
                ptPRN->chStates = PRINT_ON;
            }
            //break;
        case PRINT_ON:
            if (serial_out(*ptPRN->pchString)) {
                ptPRN->pchString ++;
                ptPRN->chStates = CHECK_EMPTY;
            }
    }
    return fsm_rt_on_going;
 }

bool check_str_init(check_str_t* ptCHK,char* pchString)
{
    if (ptCHK != NULL && pchString != NULL) {
        ptCHK->chStates = 0;
        ptCHK->pchString = pchString;
        return true;
    }
    return false;
}

#define CHECK_STRING_RESET_FSM() \
do {\
    ptCHK->chStates = START;\
} while(0)
fsm_rt_t check_string(check_str_t *ptCHK)
{
    if (ptCHK == NULL) {
        return fsm_rt_err;
    }
    enum {
        START = 0,
        CHECK_EMPTY,
        RECEIVE,
        CHECK_ON
    };
    switch (ptCHK->chStates) {
        case START:
            ptCHK->chStates = CHECK_EMPTY;
            //break;
        case CHECK_EMPTY:
            if ('\0' == *ptCHK->pchString) {
                CHECK_STRING_RESET_FSM();
                return fsm_rt_cpl;
            } else {
                ptCHK->chStates = RECEIVE;
            }
            //break;
        case RECEIVE:
            if (serial_in(&ptCHK->chByte)) {
                ptCHK->chStates = CHECK_ON;
            } else {
                break;
            }
            //break;
        case CHECK_ON:
            if (*ptCHK->pchString == ptCHK->chByte) {
                ptCHK->pchString ++;
                ptCHK->chStates = CHECK_EMPTY;
            } else {
                CHECK_STRING_RESET_FSM();
            }
    }
    return fsm_rt_on_going;
}
