#pragma once
/* data_iterator — cursor over a data_array (data.c) */
#include <stdint.h>
#include "data_array.h"

typedef struct data_iterator
{
    data_array  *data;            /* 0x0 */
    int16_t      absolute_index;  /* 0x4 */
    unsigned char _pad0[2]; /* db-verified padding */
    int          index;           /* 0x8 — current datum index */
    unsigned int signature;       /* 0xC */
} data_iterator;                  /* 16 bytes */

#ifdef __cplusplus
extern "C" {
#endif
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
#ifdef __cplusplus
}
#endif
