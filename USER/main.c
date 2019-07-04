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

#include "..\MODULE\fsm\simple_fsm.h"
#include "..\MODULE\fsm\compiler.h"
#include "..\MODULE\fsm\ooc.h"

#define PRINT_STR_POOL_ITEM_SIZE    sizeof(fsm(print_string))
 
typedef struct print_str_pool_item_t     print_str_pool_item_t;
struct print_str_pool_item_t {
    bool bIsFree;
    uint8_t chBuffer[PRINT_STR_POOL_ITEM_SIZE];
};
#define PRINT_STR_POOL_ITEM_COUNT    8
 
static print_str_pool_item_t s_tExamplePool[PRINT_STR_POOL_ITEM_COUNT];

print_str_pool_item_t *    print_str_pool_allocate (void);
void print_str_pool_free( print_str_pool_item_t *ptItem );

declare_fsm(print_apple)
declare_fsm(print_orange)
declare_fsm(print_world)

def_fsm(print_apple,
    def_params(
        fsm(print_string) fsmPrintApple;
    ) 
)
def_fsm(print_orange,
    def_params(
        fsm(print_string) fsmPrintOrange;
    ) 
)
def_fsm(print_world,
    def_params(
        fsm(print_string) fsmPrintWorld;
    ) 
)
fsm_initialiser(print_apple)
    init_body()
fsm_initialiser(print_orange)
    init_body()
fsm_initialiser(print_world)
    init_body()
fsm_implementation(print_apple)
{
    def_states(
        PRINT
    )
    body_begin();
    
    on_start(
        init_fsm(print_string,
                &this.fsmPrintApple,args("apple\r\n",sizeof("apple\r\n") -1)
        );
        transfer_to(PRINT)
    )
    state(PRINT) {
        if(fsm_rt_cpl == call_fsm(print_string,&this.fsmPrintApple)) {
            fsm_cpl();
        }
    }
    body_end();
}

fsm_implementation(print_orange)
{
    def_states(
        PRINT
    )
    body_begin();
    
    on_start(
        init_fsm(print_string,
                &this.fsmPrintOrange,args("orange\r\n",sizeof("orange\r\n") -1)
        );
        transfer_to(PRINT)
    )
    state(PRINT) {
        if(fsm_rt_cpl == call_fsm(print_string,&this.fsmPrintOrange)) {
            fsm_cpl();
        }
    }
    body_end();
}

fsm_implementation(print_world)
{
    def_states(
        PRINT
    )
    body_begin();
    
    on_start(
        init_fsm(print_string,
                &this.fsmPrintWorld,args("world\r\n",sizeof("world\r\n") -1)
        );
        transfer_to(PRINT)
    )
    state(PRINT) {
        if(fsm_rt_cpl == call_fsm(print_string,&this.fsmPrintWorld)) {
            fsm_cpl();
        }
    }
    body_end();
}
static fsm(print_apple) s_fsmPrintApple;
int main(void)
{
    system_init();
    init_fsm(print_apple,&s_fsmPrintApple);
    while(1) {
        breath_led();
        call_fsm(print_apple,&s_fsmPrintApple);
    }
}



