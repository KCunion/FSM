#ifndef __DRV_UART_H
#define __DRV_UART_H

#include "simple_fsm.h"
#include "ooc.h"
#include "compiler.h"

/*! fsm used to output specified string */
extern_simple_fsm( print_string,
    def_params(
        const char *pchStr;              //!< point to the target string
        uint16_t hwIndex;                //!< current index
        uint16_t hwLength;              //!< claimed length of the target string, it is used to prevent buffer overflow 
    )
)
/*! fsm used to output specified string */
extern_simple_fsm( check_string,
    def_params(
        const char *pchStr;              //!< point to the target string
        uint16_t hwIndex;                //!< current index
        uint16_t hwLength;              //!< claimed length of the target string, it is used to prevent buffer overflow 
    )
)
//declare_simple_fsm(print_string);

extern_fsm_implementation(print_string);

extern_fsm_initialiser(print_string,
    args(
        const char *pchStr,
        uint16_t hwLength
    ));
extern_fsm_implementation(check_string);

extern_fsm_initialiser(check_string,
    args(
        const char *pchStr,
        uint16_t hwLength
    ));
#endif
