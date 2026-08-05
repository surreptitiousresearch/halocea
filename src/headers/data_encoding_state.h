#pragma once
#include <stdint.h>
/* data_encoding_state — running state for serializing a packet into a fixed-size buffer (16 bytes). */

typedef struct data_encoding_state
{
    void           *buffer;        /* 0x00 */
    int             offset;        /* 0x04 — bytes written so far */
    int             buffer_size;   /* 0x08 */
    uint8_t overflow_flag; /* 0x0C — set once a write would exceed buffer_size */
} data_encoding_state;
