#include "sys.h"
#include "critical.h"

bool enter_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical) {
        if (false == ptCritical ->bLocked) {
            ptCritical ->bLocked = true;
            return true;
        }
    }
    return false;
}

void leave_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical) {
        ptCritical ->bLocked = false;
    }
}

void init_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical) {
        ptCritical ->bLocked = false;
    }
}
