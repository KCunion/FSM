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
 
typedef struct print_str_pool_item_t print_str_pool_item_t;
struct print_str_pool_item_t {
    bool bIsFree;
    uint8_t chBuffer[PRINT_STR_POOL_ITEM_SIZE];
};
#define PRINT_STR_POOL_ITEM_COUNT    2
//ÄÚ´æ³Ø
static print_str_pool_item_t s_tExamplePool[PRINT_STR_POOL_ITEM_COUNT];

print_str_pool_item_t *    print_str_pool_allocate (void);
void print_str_pool_free( print_str_pool_item_t *ptItem );

declare_fsm(print_apple)
declare_fsm(print_orange)
declare_fsm(print_world)

def_fsm(print_apple,
    def_params(
//        fsm(print_string) fsmPrintApple;
        print_str_pool_item_t * poolPrintApple;
    ) 
)
def_fsm(print_orange,
    def_params(
//        fsm(print_string) fsmPrintOrange;
        print_str_pool_item_t * poolPrintOrange;
    ) 
)
def_fsm(print_world,
    def_params(
//        fsm(print_string) fsmPrintWorld;
        print_str_pool_item_t * poolPrintWorld;
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
        this.poolPrintApple = print_str_pool_allocate();
        if(NULL != this.poolPrintApple) {
            init_fsm(print_string,
                    (fsm(print_string) *) &this.poolPrintApple->chBuffer,
                     args("apple",sizeof("apple") -1)
            );
            transfer_to(PRINT);
        }
        fsm_on_going();
    )
    state(PRINT) {
        if(fsm_rt_cpl == 
           call_fsm(print_string,
                    (fsm(print_string) *) &this.poolPrintApple->chBuffer)) {
            print_str_pool_free(this.poolPrintApple);
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
        this.poolPrintOrange = print_str_pool_allocate();
        if(NULL != this.poolPrintOrange) {
            init_fsm(print_string,
                    (fsm(print_string) *) &this.poolPrintOrange->chBuffer,
                     args("orange",sizeof("orange") -1)
            );
            transfer_to(PRINT);
        }
        fsm_on_going();
    )
    state(PRINT) {
        if(fsm_rt_cpl == 
           call_fsm(print_string,
                   (fsm(print_string) *) &this.poolPrintOrange->chBuffer)) {
            print_str_pool_free(this.poolPrintOrange);
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
        this.poolPrintWorld = print_str_pool_allocate();
        if(NULL != this.poolPrintWorld) {
            init_fsm(print_string,
                    (fsm(print_string) *) &this.poolPrintWorld,
                    args("world",sizeof("world") -1)
            );
            transfer_to(PRINT);
        }
        fsm_on_going();
    )
    state(PRINT) {
        if(fsm_rt_cpl == 
           call_fsm(print_string,
           (fsm(print_string) *) &this.poolPrintWorld->chBuffer)) {
            print_str_pool_free(this.poolPrintWorld);
            fsm_cpl();
        }
    }
    body_end();
}
static fsm(print_apple) s_fsmPrintApple;
static fsm(print_orange) s_fsmPrintOrange;
static fsm(print_world) s_fsmPrintWorld;
int main(void)
{
    uint8_t i;
    system_init();
    for (i = 0;i < PRINT_STR_POOL_ITEM_COUNT;i ++) {
        print_str_pool_free(&s_tExamplePool[i]);
    }
    init_fsm(print_apple,&s_fsmPrintApple);
    init_fsm(print_orange,&s_fsmPrintOrange);
    init_fsm(print_world,&s_fsmPrintWorld);
    while(1) {
        breath_led();
        call_fsm(print_apple,&s_fsmPrintApple);
        call_fsm(print_orange,&s_fsmPrintOrange);
        call_fsm(print_world,&s_fsmPrintWorld);
    }
}

print_str_pool_item_t *    print_str_pool_allocate (void)
{
    uint8_t i;
    for (i = 0; i < PRINT_STR_POOL_ITEM_COUNT; i ++) {
        if (false  != s_tExamplePool[i].bIsFree) {
            s_tExamplePool[i].bIsFree = false;
            return &s_tExamplePool[i];
        }
    }
    return NULL;
}
void print_str_pool_free( print_str_pool_item_t *ptItem )
{
    if (NULL != ptItem) {
        ptItem->bIsFree = true;
    }
}

