#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include "stdbool.h"
//#include "stdio.h"
#include "event.h"
#include "fsm.h"

#include "compiler.h"
#include "ooc.h"
#include "simple_fsm.h"

typedef struct {
    uint8_t chStates;
    char* pchString;
}print_str_t;

typedef struct {
    uint8_t chByte;
    uint8_t chStates;
    char* pchString;
}check_str_t;

#define PRINT_STR_INIT(__PRINT,__STR)   print_str_init(__PRINT,__STR)
#define CHECK_STR_INIT(__CHECK,__STR)   check_str_init(__CHECK,__STR)
#define PRINT_STRING(__PRINT)   print_string(__PRINT)
#define CHECK_STRING(__CHECK)   check_string(__CHECK)

extern void uart_init(void);
extern bool serial_out(uint8_t chByte);
extern bool serial_in(uint8_t *pchByte);

extern bool print_str_init(print_str_t* ptPRN,char* pchString);
extern bool check_str_init(check_str_t* ptCHK,char* pchString);
extern fsm_rt_t print_string(print_str_t *ptPRN);
extern fsm_rt_t check_string(check_str_t *ptCHK);
#endif


