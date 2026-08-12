/* data_decode_memory @0x83830958 — reserves `count` scaled elements from the decode stream and returns a
 * pointer to them, or NULL (setting `state->overflow_flag`) if that would exceed `state->buffer_size`.
 * `code` selects the per-element size: -8/-4/-2 = 8/4/2 bytes each, 1 = 1 byte each. Any other `code` value
 * reads an uninitialized stack slot for the byte count — this is a real quirk of the original compiled
 * function (an incompletely-covered switch with no default case), reproduced faithfully rather than patched,
 * since every real caller in this codebase only ever passes one of the four handled codes. */

#include <stdint.h>
#include "headers/data_encoding_state.h"
#include "headers/byte_swap_definition.h"

char *data_decode_memory(data_encoding_state *state, int16_t count, int code)
{
    int byte_count;
    int uninitialized_fallback;

    switch (code)
    {
        case _8byte: byte_count = 8 * count; break;
        case _4byte: byte_count = 4 * count; break;
        case _2byte: byte_count = 2 * count; break;
        case _1byte: byte_count = count; break;
        default: byte_count = uninitialized_fallback; break;
    }

    int offset = state->offset;
    int new_offset = offset + byte_count;

    if (new_offset > state->buffer_size || state->overflow_flag)
    {
        state->overflow_flag = 1;
        return 0;
    }

    char *buffer = (char *)state->buffer;
    state->offset = new_offset;
    return &buffer[offset];
}
