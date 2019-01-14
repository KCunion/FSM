#ifndef __CRITICAL_H
#define __CRITICAL_H
#include "stdbool.h"
#include "event.h"
typedef struct {
    event_t tSealed;
    void *pTarget;
}mailbox_t;

#define POST_MAIL(__MAIL,__TARGET)  post_mail(__MAIL,__TARGET)
#define OPEN_MAIL(__MAIL)           open_mail(__MAIL)
#define INIT_MAIL(__MAIL)           init_mail(__MAIL)

#endif