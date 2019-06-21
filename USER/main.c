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

static print_str_t s_tPrintHello;
static check_str_t s_tCheckWorld;
static event_t s_tPrintTrgo;

//����print_hello״̬��
simple_fsm( print_hello,
)
static fsm(print_hello) s_fsmPrintHello;
//��ʼ��print_hello״̬��
fsm_initialiser(print_hello)
    init_body ()

//ʵ��print_hello״̬��
fsm_implementation(print_hello)
    def_states(
        PRINT
    )
    body(
        on_start(
            if (!PRINT_STR_INIT(&s_tPrintHello,"hello\r\n")) {
                reset_fsm();
            }
            transfer_to(PRINT);
        )
        state ( PRINT,
            if (fsm_rt_cpl == PRINT_STRING(&s_tPrintHello)) {
                fsm_cpl();
            }
        )
    )
//����task_print״̬��
simple_fsm( task_print,
)
static fsm(task_print) s_fsmTaskPrint;
//��ʼ��task_print״̬��
fsm_initialiser(task_print)
    init_body ()
//ʵ��task_print״̬��
fsm_implementation( task_print)
    def_states(
        WATING,
        PRINT
    )
    body(
        on_start(
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
//����check_world״̬��
simple_fsm( check_world,
)
static fsm(check_world) s_fsmCheckWorld;
//��ʼ��check_world״̬��
fsm_initialiser(check_world)
    init_body ()
//ʵ��check_world״̬��
fsm_implementation(check_world)
    def_states(
        CHECK
    )
    body(
        on_start(
            if (!CHECK_STR_INIT(&s_tCheckWorld,"world")) {
                    reset_fsm();
            }
            transfer_to(CHECK);
        )
        state( CHECK,
            if (fsm_rt_cpl == CHECK_STRING(&s_tCheckWorld)) {
                fsm_cpl();
            }
        )
    )
//����task_check״̬��
simple_fsm( task_check,
)
static fsm(task_check) s_fsmTaskCheck;
//��ʼ��task_check״̬��
fsm_initialiser(task_check)
    init_body ()
//ʵ��task_check״̬��
fsm_implementation(task_check)
    def_states(
        CHECK
    )
    body(
        on_start(
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
    while(1) {
        breath_led();
        call_fsm(   task_check,
                    &s_fsmTaskCheck);
        call_fsm(   task_print,
                    &s_fsmTaskPrint);
    }
}



