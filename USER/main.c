#include "sys.h"

#define ABS(__N)    ((__N) < 0 ? -(__N) : (__N))
#define _BV(__N)    ((uint32_t)1<<(__N))
#define TOP         (0x01FF)

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
 
static void breath_led(void)
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
    INIT_EVENT(&s_tPrintTrgo,RESET,AUTO);
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
                if (false != s_tCheckWorld.bFlag) {
                    CHECK_WORLD_RESET_FSM();
                    return fsm_rt_cpl;
                }
                CHECK_WORLD_RESET_FSM();
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
