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

typedef struct {
    uint8_t* pchString;
    uint16_t hwLength;
    uint16_t hwIncrease;
}print_str_t;

static fsm_rt_t print_string(print_str_t* tPrintStr);
static fsm_rt_t print_apple(void);
static fsm_rt_t print_orange(void);
static fsm_rt_t print_world(void);
static print_str_t s_tPrintApple;
static print_str_t s_tPrintOrange;
static print_str_t s_tPrintWorld;
int main(void)
{
    system_init();
    while(1) {
        breath_led();
        print_apple();
        print_orange();
        print_world();
    }
}

#define PRINT_RESET_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t print_string(print_str_t* ptPrintStr)
{
    static enum {
        START = 0,
        PRINT,
        WATING
    } s_tState = START;
    if (ptPrintStr == NULL) {
        return fsm_rt_err;
    }
    switch (s_tState) {
        case START:
            s_tState = WATING;
            //break;
        case WATING:
            if (ptPrintStr->hwLength > 0 && ptPrintStr->hwIncrease < ptPrintStr->hwLength) {
                s_tState = PRINT;
            }
            break;
        case PRINT:
            if (serial_out(*(ptPrintStr->pchString + ptPrintStr->hwIncrease))) {
                if (++ ptPrintStr->hwIncrease >= ptPrintStr->hwLength) {
                    PRINT_RESET_FSM();
                    return fsm_rt_cpl;
                }
            }
    }
    return fsm_rt_on_going;
 }
static uint8_t s_chApple[] = "apple\r\n"; 
 #define PRINT_APPLE_RESET_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t print_apple(void)
{
    static enum {
        START = 0,
        PRINT
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tPrintApple.pchString = s_chApple;
            s_tPrintApple.hwLength = sizeof(s_chApple);
            s_tPrintApple.hwIncrease = 0;
            s_tState = PRINT;
            //break;
        case PRINT:
            if (fsm_rt_cpl == print_string(&s_tPrintApple)) {
                PRINT_APPLE_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;
}
static uint8_t s_chOrange[] = "orange\r\n"; 
#define PRINT_ORANGE_RESET_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t print_orange(void)
{
    static enum {
        START = 0,
        PRINT
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tPrintOrange.pchString = s_chOrange;
            s_tPrintOrange.hwLength = sizeof(s_chOrange);
            s_tPrintOrange.hwIncrease = 0;
            s_tState = PRINT;
            //break;
        case PRINT:
            if (fsm_rt_cpl == print_string(&s_tPrintOrange)) {
                PRINT_APPLE_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;
}
static uint8_t s_chWorld[] = "world\r\n"; 
#define PRINT_WORLD_RESET_FSM() \
do {\
    s_tState = START;\
} while(0)
static fsm_rt_t print_world(void)
{
    static enum {
        START = 0,
        PRINT
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tPrintWorld.pchString = s_chWorld;
            s_tPrintWorld.hwLength = sizeof(s_chWorld);
            s_tPrintWorld.hwIncrease = 0;
            s_tState = PRINT;
            //break;
        case PRINT:
            if (fsm_rt_cpl == print_string(&s_tPrintWorld)) {
                PRINT_APPLE_RESET_FSM();
                return fsm_rt_cpl;
            }
    }
    return fsm_rt_on_going;
}

