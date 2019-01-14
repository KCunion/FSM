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

//! \name finit state machine state
//! @{
typedef enum {
   fsm_rt_err          = -1,    //!< fsm error, error code can be get from other interface
   fsm_rt_cpl          = 0,     //!< fsm complete
   fsm_rt_on_going     = 1,     //!< fsm on-going
} fsm_rt_t;
//! @}


static fsm_rt_t print(void);
static fsm_rt_t check(void);
static fsm_rt_t task_check(void);
static fsm_rt_t task_print(void);
static fsm_rt_t task_echo(void);
static event_t s_tPrintEvevt;
static event_t s_tReceivedEvevt;
static uint8_t s_chRecByte;
int main(void)
{
    system_init();
    INIT_EVENT(&s_tPrintEvevt,RESET,MANUAL);
    INIT_EVENT(&s_tReceivedEvevt,RESET,AUTO);
    while(1) {
        breath_led();
        task_check();
        task_print();
        task_echo();
    }
}
#define TASK_ECHO_RESET_FSM() \
do { \
    s_tState = START; \
} while(0)
static fsm_rt_t task_echo(void)
{
    static enum {
        START = 0,
        WAIT_RECEIVED,
//        WAIT_PRINT,
        PRINT,
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = WAIT_RECEIVED;
            //break;
        case WAIT_RECEIVED:
            if (WAIT_EVENT(&s_tReceivedEvevt)) {
                s_tState = PRINT;
            }
            break;
//        case WAIT_PRINT:
//            if (WAIT_EVENT(&s_tPrintEvevt)) {
//                break;
//            } else {
//                s_tState = PRINT;
//            }
//            //break;
        case PRINT:
            if (WAIT_EVENT(&s_tPrintEvevt)) {
                break;
            } else {
                if (serial_out(s_chRecByte)) {
                    TASK_ECHO_RESET_FSM();
                    return fsm_rt_cpl;
                }
            }
            
    }
    return fsm_rt_on_going;
}

#define TASK_CHECK_RESET_FSM() \
do { \
    s_tState = START; \
} while(0)
static fsm_rt_t task_check(void)
{
    static enum {
        START = 0,
        PRODUCE,
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = PRODUCE;
            //break;
        case PRODUCE:
            if (fsm_rt_cpl == check()) {
                SET_EVENT(&s_tPrintEvevt);
                TASK_CHECK_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;
}
#define TASK_PRINT_RESET_FSM() \
do { \
    s_tState = START; \
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
            if (WAIT_EVENT(&s_tPrintEvevt)) {
                s_tState = PRINT;
            }
            break;
        case PRINT:
            if (fsm_rt_cpl == print()) {
                RESET_EVENT(&s_tPrintEvevt);
                TASK_PRINT_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;
}


#define PRINT_CHECK_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t check(void)
{
    static enum {
        START = 0,
        RCV_W,
        RCV_O,
        RCV_R,
        RCV_L,
        RCV_D,
    } s_tState = START;
    switch (s_tState ) {
        case START:
            s_tState = RCV_W;
            //break;
        case RCV_W:
            if (false != serial_in(&s_chRecByte)) {
                SET_EVENT(&s_tReceivedEvevt);
                if ('w' == s_chRecByte) {
                    s_tState = RCV_O;
                } else {
                    PRINT_CHECK_FSM();
                }
            }
            break;
        case RCV_O:
            if (false != serial_in(&s_chRecByte)) {
                SET_EVENT(&s_tReceivedEvevt);
                if ('o' == s_chRecByte) {
                    s_tState = RCV_R;
                } else {
                    PRINT_CHECK_FSM();
                }
            }
            break;
        case RCV_R:
            if (false != serial_in(&s_chRecByte)) {
                SET_EVENT(&s_tReceivedEvevt);
                if ('r' == s_chRecByte) {
                    s_tState = RCV_L;
                } else {
                    PRINT_CHECK_FSM();
                }
            }
            break;
        case RCV_L:
            if (false != serial_in(&s_chRecByte)) {
                SET_EVENT(&s_tReceivedEvevt);
                if ('l' == s_chRecByte) {
                    s_tState = RCV_D;
                } else {
                    PRINT_CHECK_FSM();
                }
            }
            break;
        case RCV_D:
            if (false != serial_in(&s_chRecByte)) {
                SET_EVENT(&s_tReceivedEvevt);
                if ('d' == s_chRecByte) {
                    PRINT_CHECK_FSM();
                    return fsm_rt_cpl;
                } else {
                    PRINT_CHECK_FSM();
                }
            }
    }
    return fsm_rt_on_going;
}

#define PRINT_RESET_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t print(void)
{
    static enum {
            START = 0,
            PRINT_H,
            PRINT_E,
            PRINT_FIRST_L,
            PRINT_SECOND_L,
            PRINT_O,
            PRINT_ENT_R,
            PRINT_ENT_N,
    } s_tState = START;
    
    switch (s_tState) {
        case START:
            s_tState = PRINT_H;
            //break;
        case PRINT_H:
            if (serial_out('h')) {
                s_tState = PRINT_E;
            }
            break;
        case PRINT_E:
            if (serial_out('e')) {
                s_tState = PRINT_FIRST_L;
            }
            break;
        case PRINT_FIRST_L:
            if (serial_out('l')) {
                s_tState = PRINT_SECOND_L;
            }
            break;
        case PRINT_SECOND_L:
            if (serial_out('l')) {
                s_tState = PRINT_O;
            }
            break;
        case PRINT_O:
            if (serial_out('o')) {
                s_tState = PRINT_ENT_R;
            }
            break;
        case PRINT_ENT_R:
            if (serial_out('\r')) {
                s_tState = PRINT_ENT_N;
            }
         case PRINT_ENT_N:
            if (serial_out('\n')) {
                PRINT_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;
}
