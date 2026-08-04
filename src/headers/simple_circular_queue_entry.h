#pragma once
/* simple_circular_queue_entry — one ring-buffer slot; holds a 4-byte pointer
   into the caller-supplied user-data array. */

typedef struct simple_circular_queue_entry
{
    void *user_data; /* 0x00 */
} simple_circular_queue_entry; /* 4 bytes */
