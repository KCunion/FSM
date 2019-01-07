#include "sys.h"
#include "event.h"

void set_event(event_t *ptEvent)
{
    if (ptEvent != NULL) {
        ptEvent->bIsSet = true;
    }
}

bool wait_event(event_t *tEvent)
{
    if ((tEvent != NULL) && false != tEvent->bIsSet) {
        if (false != tEvent->bAutoReset) {
            tEvent->bIsSet = false;
        }
        return true;
    }
    return false;
}

void reset_event(event_t *ptEvent)
{
    if (ptEvent != NULL) {
        ptEvent->bIsSet = false;
    }
}

void init_event(event_t *ptEvent,bool bValue,bool bManual)
{
    if (ptEvent != NULL) {
        ptEvent->bAutoReset = !bManual;
        ptEvent->bIsSet = bValue;
    }
}
