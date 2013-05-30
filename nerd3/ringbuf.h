/* ringbuf.h */
#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include "mystd.h"

#ifdef RINGBUF_SIZE
  #define SIZE RINGBUF_SIZE
#else
  #error "You need to define RINGBUF_SIZE"
#endif

/* types and values */
typedef struct {
    u1 buf[SIZE];
    u1 pr, pw;
    u1 unread_count;
} RINGBUF;


/* service functions */
static inline void ringbuf_init(RINGBUF *r)
{
    for(u1 i=0; i<SIZE; i++)
        r->buf[i] = 0xFF;

    r->pr = r->pw = 0;
    r->unread_count = 0;
}

bool ringbuf_write(RINGBUF *r, u1 ch)
{
    u1 nextw = (r->pw+1) % SIZE;
    
    if(nextw == r->pr) return NG;

    r->pw = nextw;
    r->buf[nextw] = ch;
    r->unread_count++;
    return OK;
}

bool ringbuf_read(RINGBUF *r, u1 *ch)
{
    u1 nextw = (r->pw+1) % SIZE;
    u1 nextr = (r->pr+1) % SIZE;

    if(nextw == nextr) return NG;

    r->pr = nextr;
    r->unread_count--;
    *ch = r->buf[nextr];
    return OK;
}

#undef SIZE

#endif  /* __RINGBUF_H__ */

