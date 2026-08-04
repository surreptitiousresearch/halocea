#ifndef _OVERLAPPED_H
#define _OVERLAPPED_H

#include <stdint.h>

typedef struct _OVERLAPPED
{
    uint32_t Internal;
    uint32_t InternalHigh;
    uint32_t Offset;
    uint32_t OffsetHigh;
    void    *hEvent;
} _OVERLAPPED;

#endif /* _OVERLAPPED_H */
