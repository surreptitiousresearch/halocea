/* data_encode_memory @0x83830508 — append count elements to the encoding buffer, sizing each element from the
 * code (1 = bytes, -2/-4/-8 = 2/4/8-byte elements). A null source writes zeros. Sets the overflow flag and
 * fails if the write would exceed the buffer. Returns whether no overflow has occurred.
 *
 * Deviation: the decompiler's "default" case reads an uninitialized stack size; only the defined codes
 * (1, -2, -4, -8) are produced by callers, so the default is left as an unreachable size of 0. */

#include <stdint.h>
#include "headers/data_encoding_state.h"
extern void *memcpy(void *dst, const void *src, unsigned int size);
extern void *memset(void *dst, int value, unsigned int n);

int data_encode_memory(data_encoding_state *state, const void *buffer, int16_t count, int code)
{
    unsigned int byte_count;
    switch ( code )
    {
        case -8: byte_count = 8 * count; break;
        case -4: byte_count = 4 * count; break;
        case -2: byte_count = 2 * count; break;
        case 1:  byte_count = count;     break;
        default: byte_count = 0;         break;
    }

    int offset = state->offset;
    if ( (signed int)(offset + byte_count) > state->buffer_size || state->overflow_flag )
    {
        state->overflow_flag = 1;
    }
    else
    {
        char *destination = (char *)state->buffer + offset;
        if ( buffer )
            memcpy(destination, buffer, byte_count);
        else
            memset(destination, 0, byte_count);
        state->offset += byte_count;
    }
    return state->overflow_flag == 0;
}
