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

static fsm_rt_t print_string(uint8_t *pchString);
static fsm_rt_t print_apple(void);
static fsm_rt_t print_orange(void);
static fsm_rt_t print_world(void);
 
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

static fsm_rt_t print_string(uint8_t *pchString)
{
    static enum {
        START = 0,
        PRINT,
        WATING
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = PRINT;
            //break;
        case PRINT:
            
            break;
        case WATING:
        
            break;
    }
    
    return fsm_rt_cpl;
}
static fsm_rt_t print_apple(void)
{
    return fsm_rt_cpl;
}
static fsm_rt_t print_orange(void)
{
    return fsm_rt_cpl;
}
static fsm_rt_t print_world(void)
{
    return fsm_rt_cpl;
}

