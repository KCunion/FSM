//#include ".\drv_uart.h"
#include "..\..\BSP\uart\uart.h"

#include "..\..\MODULE\fsm\simple_fsm.h"
#include "..\..\MODULE\fsm\compiler.h"
#include "..\..\MODULE\fsm\ooc.h"

declare_simple_fsm(check_string)
declare_simple_fsm(print_string)

//定义check_string状态机
def_simple_fsm( check_string,
    def_params(
        const char *pchStr;              //!< point to the target string
        uint16_t hwIndex;                //!< current index
        uint16_t hwLength;              //!< claimed length of the target string, it is used to prevent buffer overflow 
    )
)

//定义print_hello状态机
/*! fsm used to output specified string */
def_simple_fsm( print_string,
    def_params(
        const char *pchStr;              //!< point to the target string
        uint16_t hwIndex;                //!< current index
        uint16_t hwLength;              //!< claimed length of the target string, it is used to prevent buffer overflow 
    )
)

//初始化print_string状态机
fsm_initialiser(print_string,
    args(
        const char *pchStr,
        uint16_t hwLength
    ))
    init_body (
        if (NULL == pchStr || 0 == hwLength) {
            abort_init();                                       //!< illegal parameter
        } else if (strlen(pchStr) < hwLength) {
            abort_init();                                       //!< buffer overflow
        }
        this.pchStr = pchStr;
        this.hwIndex = 0;
        this.hwLength = hwLength;
    )

//实现print_string状态机
fsm_implementation(print_string)
{
    def_states(
        CHECK_EMPTY,
        PRINT_ON
    )
    body_begin();
    state (CHECK_EMPTY) {
        if (this.hwIndex == this.hwLength) {
            fsm_cpl();                
        }
        transfer_to(PRINT_ON);
    }
    state (PRINT_ON) {
        if (serial_out(*(this.pchStr + this.hwIndex))) {
            this.hwIndex ++;
            transfer_to(CHECK_EMPTY);
        }
    }
    body_end();
}

//初始化check_string状态机
fsm_initialiser(check_string,
    args(
        const char *pchStr,
        uint16_t hwLength
    ))
    init_body (
        if (NULL == pchStr || 0 == hwLength) {
            abort_init();                                       //!< illegal parameter
        } else if (strlen(pchStr) < hwLength) {
            abort_init();                                       //!< buffer overflow
        }
        this.pchStr = pchStr;
        this.hwIndex = 0;
        this.hwLength = hwLength;
    )
//实现check_string状态机
fsm_implementation(check_string)
{
    def_states(
        CHECK_EMPTY,
        RECEIVE,
        CHECK_ON
    )
    uint8_t chByte;
    body_begin();
    state (CHECK_EMPTY) {
        if (this.hwIndex == this.hwLength) {
            fsm_cpl();                
        }
        transfer_to(RECEIVE);
    }
    state (RECEIVE) {
            if (serial_in(&chByte)) {
                update_state_to(CHECK_ON);
            } else {
            fsm_on_going();
        }
    }
    state (CHECK_ON) {
        if (*(this.pchStr + this.hwIndex) == chByte) {
            this.hwIndex ++;
            transfer_to(CHECK_EMPTY);
        } else {
            reset_fsm();
        }
    }
    body_end();
}

