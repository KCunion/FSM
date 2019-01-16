#include "sys.h"
#include "mailbox.h"

void init_mail(mailbox_t *ptMail)
{
    if (NULL != ptMail) {
        INIT_EVENT(&ptMail->tSealed,RESET,AUTO);
        ptMail->pTarget = NULL;
    }
}

void *open_mail(mailbox_t *ptMail)
{
    if (NULL != ptMail) {
        if (WAIT_EVENT(&ptMail ->tSealed)) {
            return ptMail->pTarget;
        }
    }
    return NULL;
}

void post_mail(mailbox_t *ptMail,void *pTarget)
{
    if (NULL != ptMail && NULL != pTarget) {
        SET_EVENT(&ptMail ->tSealed);
        ptMail->pTarget = pTarget;
    }
}
