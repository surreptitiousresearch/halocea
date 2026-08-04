#pragma once
/* simple_circular_queue — fixed-capacity ring buffer of fixed-size user elements. */

#include "simple_circular_queue_entry.h"

typedef struct simple_circular_queue
{
    int                          max_size;               /* 0x00 */
    int                          user_data_element_size; /* 0x04 */
    simple_circular_queue_entry *entries;                /* 0x08 */
    int                          write_position;         /* 0x0C */
    int                          read_position;          /* 0x10 */
} simple_circular_queue;                                 /* 20 bytes */
