#ifndef __CRITICAL_H
#define __CRITICAL_H
#include "stdbool.h"

typedef struct {
    bool bLocked;
}critical_sector_t;
typedef critical_sector_t mutex_t;

#define ENTER_CRITICAL_SECTOR(__CRITICAL)   enter_cricital_sector(__CRITICAL)   //进入临界区
#define LEAVE_CRITICAL_SECTOR(__CRITICAL)   leave_cricital_sector(__CRITICAL)   //离开临界区
#define INIT_CRITICAL_SECTOR(__CRITICAL)    init_cricital_sector(__CRITICAL)    //初始化临界量

extern bool enter_cricital_sector(critical_sector_t *ptCritical);
extern void leave_cricital_sector(critical_sector_t *ptCritical);
extern void init_cricital_sector(critical_sector_t *ptCritical);
#endif
