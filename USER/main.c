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
#include "compiler.h"
#include "ooc.h"

static event_t s_tPrintTrgo;

static fsm(print_string) s_fsmPrintString;
static fsm(check_string) s_fsmCheckString;
//定义print_hello状态机
simple_fsm( print_hello)
static fsm(print_hello) s_fsmPrintHello;
//初始化print_hello状态机
fsm_initialiser(print_hello)
    init_body ()

//实现print_hello状态机
fsm_implementation(print_hello)
    def_states(
        PRINT
    )
    body(
        on_start(
            init_fsm(   print_string,
                        &s_fsmPrintString,
                        args("hello\r\n", sizeof("hello\r\n") - 1)
            );
            transfer_to(PRINT);
        );
        state ( PRINT,
            if (fsm_rt_cpl == call_fsm(print_string, &s_fsmPrintString)) {
                fsm_cpl();
            }
        )
    )
//定义task_print状态机
simple_fsm( task_print)
static fsm(task_print) s_fsmTaskPrint;
//初始化task_print状态机
fsm_initialiser(task_print)
    init_body ()
//实现task_print状态机
fsm_implementation( task_print)
    def_states(
        WATING,
        PRINT
    )
    body (
        on_start(
            init_fsm(   print_hello,
                        &s_fsmPrintHello
            );
            transfer_to(WATING);
        )
        state( WATING,
            if (WAIT_EVENT(&s_tPrintTrgo)) {
                transfer_to(PRINT);
            }
        )
        state( PRINT,
            if (fsm_rt_cpl == call_fsm(print_hello,&s_fsmPrintHello)) {
                fsm_cpl();
            }
        )
    )
//定义check_world状态机
simple_fsm( check_world)
static fsm(check_world) s_fsmCheckWorld;
//初始化check_world状态机
fsm_initialiser(check_world)
    init_body ()
//实现check_world状态机
fsm_implementation(check_world)
    def_states(
        CHECK
    )
    body(
        on_start(
            init_fsm(   check_string,
                        &s_fsmCheckString,
                        args("world", sizeof("world") - 1)
            );
            transfer_to(CHECK);
        )
        state( CHECK,
            if (fsm_rt_cpl == call_fsm(check_string, &s_fsmCheckString)) {
                fsm_cpl();
            }
        )
    )
//定义task_check状态机
simple_fsm( task_check)
static fsm(task_check) s_fsmTaskCheck;
//初始化task_check状态机
fsm_initialiser(task_check)
    init_body ()
//实现task_check状态机
fsm_implementation(task_check)
    def_states(
        CHECK
    )
    body(
        on_start(
            init_fsm(   check_world,
                        &s_fsmCheckWorld
            );
            transfer_to(CHECK);
        )
        state( CHECK,
            if (fsm_rt_cpl == call_fsm(check_world,&s_fsmCheckWorld)) {
                SET_EVENT(&s_tPrintTrgo);
                fsm_cpl();
            }
        )
    )


int main(void)
{
    system_init();
    INIT_EVENT(&s_tPrintTrgo,EVENT_RESET,EVENT_AUTO);
    init_fsm(   task_print,
                &s_fsmTaskPrint
     );
    init_fsm(   task_check,
                &s_fsmTaskCheck
    );
    while(1) {
        breath_led();
        call_fsm(   task_check,
                    &s_fsmTaskCheck);
        call_fsm(   task_print,
                    &s_fsmTaskPrint);
    }
}



