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

int main(void)
{
    system_init();

    while(1) {
        breath_led();

    }
}



