#include <stdint.h>

#pragma once
/* object_iterator — cursor for walking the object header table by type mask. Layout from the database. */

typedef struct object_iterator
{
    unsigned int    type_flags;     /* 0x00 */
    unsigned __int8 flags;          /* 0x04 */
    char            _pad05;         /* 0x05 */
    __int16         absolute_index; /* 0x06 */
    int             index;          /* 0x08 */
    unsigned int    signature;      /* 0x0C */
} object_iterator;

struct object_datum;
/* Blam object-table iteration (boundary; unmangled C exports). */
extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator); /* DB prototype returns void*; callers cast to the concrete *_datum */
