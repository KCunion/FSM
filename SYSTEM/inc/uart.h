#ifndef __UART_H
#define __UART_H
#include "HAL_device.h"
#include  "stdio.h"
#include "stdbool.h"
#include "fsm.h"


typedef struct {
    int8_t* pchString;
    uint8_t chStates;
}print_str_t;

#define PRINT_STR_INIT(__PRINT,__STR)    print_str_init(__PRINT,__STR)

extern void uart_init(void);
extern bool serial_out(uint8_t chByte);
extern bool serial_in(uint8_t *pchByte);

extern bool print_str_init(print_str_t* ptPrint,int8_t* pchString);
extern fsm_rt_t print_string(print_str_t* ptPrintStr);

#endif


