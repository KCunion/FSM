#include "sys.h"

bool enter_cricital_sector(critical_sector_t *ptCritical)
{
    if ((ptCritical != NULL) && false == ptCritical ->bLocked) {
        ptCritical ->bLocked = true;
        return true;
    }
    return false;
}

void leave_cricital_sector(critical_sector_t *ptCritical)
{
    if (ptCritical != NULL) {
        ptCritical ->bLocked = false;
    }
}

void init_cricital_sector(critical_sector_t *ptCritical)
{
    if (ptCritical != NULL) {
        ptCritical ->bLocked = false;
    }
}
