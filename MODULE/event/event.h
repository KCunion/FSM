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
    
#define SET_EVENT(__EVENT)      set_event(__EVENT)       //发送事件
#define WAIT_EVENT(__EVENT)     wait_event(__EVENT)     //等待事件
#define RESET_EVENT(__EVENT)    reset_event(__EVENT)   //复位事件
#define INIT_EVENT(__EVENT, __INIT_VALUE,__MANUAL)  init_event(__EVENT, __INIT_VALUE,__MANUAL)   //初始化事件


extern void set_event(event_t *ptEvent);
extern bool wait_event(event_t *ptEvent);
extern void reset_event(event_t *ptEvent);
extern void init_event(event_t *ptEvent,bool bValue,bool bManual);
#endif
