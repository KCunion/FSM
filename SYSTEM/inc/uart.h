#ifndef __UART_H
#define __UART_H
#include "HAL_device.h"
#include  "stdio.h"
#include "stdbool.h"
#include "fsm.h"
typedef enum {
    print_start = 0,
    print_on ,
    print_cpl
}print_st_t;

typedef struct {
    int8_t* pchString;
    print_st_t tStates;
//    uint16_t hwPosition;
}print_str_t;

#define PRINT_STR_INIT(__PRINT,__STR)    print_str_init(__PRINT,__STR)
#define SET_PRINT_STR_STATE(__PRINT,__STATE)      set_print_str_state(__PRINT,__STATE)

extern void uart_init(void);
extern bool serial_out(uint8_t chByte);
extern bool serial_in(uint8_t *pchByte);

extern bool print_str_init(print_str_t* ptPrint,int8_t* pchString);
extern bool set_print_str_state(print_str_t* ptPrint,print_st_t ptState);
extern fsm_rt_t print_string(print_str_t* ptPrintStr);

#endif


