#ifndef __MAILBOX_H
#define __MAILBOX_H
#include "stdbool.h"
#include "..\event\event.h"

typedef struct {
    event_t tSealed;
    void *pTarget;
}mailbox_t;

#define POST_MAIL(__MAIL,__TARGET)  post_mail(__MAIL,__TARGET)
#define OPEN_MAIL(__MAIL)           open_mail(__MAIL)
#define INIT_MAIL(__MAIL)           init_mail(__MAIL)

extern void init_mail(mailbox_t *ptMail);
extern void *open_mail(mailbox_t *ptMail);
extern void post_mail(mailbox_t *ptMail,void *pTarget);
#endif
