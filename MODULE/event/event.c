#include <stdio.h>
#include ".\event.h"

void set_event(event_t *ptEvent)
{
    if (NULL != ptEvent) {
        ptEvent->bIsSet = true;
    }
}

bool wait_event(event_t *ptEvent)
{
    if (NULL != ptEvent) {
        if (false != ptEvent->bIsSet) {
            if (false != ptEvent->bAutoReset) {
                ptEvent->bIsSet = false;
            }
            return true;
        }
    }
    return false;
}

void reset_event(event_t *ptEvent)
{
    if (NULL != ptEvent) {
        ptEvent->bIsSet = false;
    }
}

void init_event(event_t *ptEvent,bool bValue,bool bManual)
{
    if (NULL != ptEvent) {
        ptEvent->bAutoReset = !bManual;
        ptEvent->bIsSet = bValue;
    }
}
