#include ".\drv_uart.h"
#include "..\..\BSP\uart\uart.h"

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
