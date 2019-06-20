#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "HAL_device.h"
#include "..\SYSTEM\sys.h"
#include "..\BSP\led\led.h"

#include "..\DRIVE\uart\drv_uart.h"
#include "..\MODULE\event\event.h"
#include "..\MODULE\critical\critical.h"
#include "..\MODULE\mailbox\mailbox.h"
#include "simple_fsm.h"

static fsm_rt_t print_hello(void);
static fsm_rt_t check_world(void);

static fsm_rt_t task_check(void);
static fsm_rt_t task_print(void);
static print_str_t s_tPrintHello;
static check_str_t s_tCheckWorld;
static event_t s_tPrintTrgo;
int main(void)
{
    system_init();
    INIT_EVENT(&s_tPrintTrgo,EVENT_RESET,EVENT_AUTO);
    while(1) {
        breath_led();
        task_check();
        task_print();
    }
}

#define PRINT_HELLO_RESET_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t print_hello(void)
{
    static enum {
        START = 0,
        PRINT
    } s_tState = START;
    switch (s_tState) {
        case START:
            if (!PRINT_STR_INIT(&s_tPrintHello,"hello\r\n")) {
                return fsm_rt_err;
            }
            s_tState = PRINT;
            //break;
        case PRINT:
            if (fsm_rt_cpl == PRINT_STRING(&s_tPrintHello)) {
                PRINT_HELLO_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;
}

#define CHECK_WORLD_RESET_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t check_world(void)
{
    static enum {
        START = 0,
        CHECK
    } s_tState = START;
    switch (s_tState) {
        case START:
            if (!CHECK_STR_INIT(&s_tCheckWorld,"world")) {
                return fsm_rt_err;
            }
            s_tState = CHECK;
            //break;
        case CHECK:
            if (fsm_rt_cpl == CHECK_STRING(&s_tCheckWorld)) {
                CHECK_WORLD_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;
}

#define TASK_CHECK_RESET_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t task_check(void)
{
    static enum {
        START = 0,
        CHECK
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = CHECK;
            //break;
        case CHECK:
            if (fsm_rt_cpl == check_world()) {
                SET_EVENT(&s_tPrintTrgo);
                TASK_CHECK_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;    
}

#define TASK_PRINT_RESET_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t task_print(void)
{
    static enum {
        START = 0,
        WATING,
        PRINT
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = WATING;
            //break;
        case WATING:
            if (WAIT_EVENT(&s_tPrintTrgo)) {
                s_tState = PRINT;
            }
            break;
        case PRINT:
            if (fsm_rt_cpl == print_hello()) {
                TASK_PRINT_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;
}
