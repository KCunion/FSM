#ifndef __EVENT_H
#define __EVENT_H
#include <stdbool.h>

typedef struct {
    bool bAutoReset;    //  reset type
    bool bIsSet;        //  initial state 
} event_t;

#define EVENT_SET     true
#define EVENT_RESET   false
#define EVENT_AUTO    false
#define EVENT_MANUAL  true
    
#define SET_EVENT(__EVENT)      set_event(__EVENT)       //�����¼�
#define WAIT_EVENT(__EVENT)     wait_event(__EVENT)     //�ȴ��¼�
#define RESET_EVENT(__EVENT)    reset_event(__EVENT)   //��λ�¼�
#define INIT_EVENT(__EVENT, __INIT_VALUE,__MANUAL)  init_event(__EVENT, __INIT_VALUE,__MANUAL)   //��ʼ���¼�


extern void set_event(event_t *ptEvent);
extern bool wait_event(event_t *ptEvent);
extern void reset_event(event_t *ptEvent);
extern void init_event(event_t *ptEvent,bool bValue,bool bManual);
#endif
