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
declare_simple_fsm(task_print)
declare_simple_fsm(task_check)
declare_simple_fsm(print_hello)
declare_simple_fsm(check_world)

//����print_hello״̬��
def_simple_fsm( print_hello,
    def_params(
        fsm(print_string) fsmPrintString;
    )
)
//����task_print״̬��
def_simple_fsm( task_print,
    def_params(
        fsm(print_hello) fsmPrintHello;
    )
)
//����check_world״̬��
def_simple_fsm( check_world,
    def_params(
        fsm(check_string) fsmCheckString;
    )
)
//����task_check״̬��
def_simple_fsm( task_check,
    def_params(
        fsm(check_world) fsmCheckWorld;
    )
)

//����״̬����ȫ�ֱ���
static fsm(task_check) s_fsmTaskCheck;
static fsm(task_print) s_fsmTaskPrint;
static event_t s_tPrintTrgo;

//��ʼ��print_hello״̬��
fsm_initialiser(print_hello)
    init_body ()

//ʵ��print_hello״̬��
fsm_implementation(print_hello)
{
    def_states(
        PRINT
    )
    body_begin(); 
    on_start(
        init_fsm(   print_string,
                    &(this.fsmPrintString),
                    args("hello\r\n", sizeof("hello\r\n") - 1)
        );
        transfer_to(PRINT);
    );
    state (PRINT) {
        if (fsm_rt_cpl == call_fsm(print_string, &(this.fsmPrintString))) {
            fsm_cpl();
        }
    }
    body_end();
}

//��ʼ��task_print״̬��
fsm_initialiser(task_print)
    init_body ()
//ʵ��task_print״̬��
fsm_implementation(task_print)
{
    def_states(
        WATING,
        PRINT
    )
    body_begin(); 
    on_start(
        init_fsm(   print_hello,
                    &(this.fsmPrintHello)
        );
        transfer_to(WATING);
    )
    state (WATING) {
        if (WAIT_EVENT(&s_tPrintTrgo)) {
            transfer_to(PRINT);
        }
    }
    state (PRINT) {
        if (fsm_rt_cpl == call_fsm(print_hello,&(this.fsmPrintHello))) {
            fsm_cpl();
        }
    }
    body_end();
}
//��ʼ��check_world״̬��
fsm_initialiser(check_world)
    init_body ()
//ʵ��check_world״̬��
fsm_implementation(check_world)
{
    def_states(
        CHECK
    )
    body_begin();
    on_start(
        init_fsm(   check_string,
                    &(this.fsmCheckString),
                    args("world", sizeof("world") - 1)
        );
        transfer_to(CHECK);
    )
    state(CHECK) {
        if (fsm_rt_cpl == call_fsm(check_string, &(this.fsmCheckString))) {
            fsm_cpl();
        }
    }
    body_end();
}
//��ʼ��task_check״̬��
fsm_initialiser(task_check)
    init_body ()
//ʵ��task_check״̬��
fsm_implementation(task_check)
{
    def_states(
        CHECK
    )
    body_begin();
        on_start(
            init_fsm(   check_world,
                        &(this.fsmCheckWorld)
            );
            transfer_to(CHECK);
        )
        state(CHECK) {
            if (fsm_rt_cpl == call_fsm(check_world,&(this.fsmCheckWorld))) {
                SET_EVENT(&s_tPrintTrgo);
                fsm_cpl();
            }
        }
    body_end();
}



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



