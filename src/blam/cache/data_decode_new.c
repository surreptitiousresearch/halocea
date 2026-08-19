/* data_decode_new @0x838308D8 — initializes a data_encoding_state cursor for reading `buffer_size` bytes from
 * `buffer`. */

#include "headers/data_encoding_state.h"

void data_decode_new(data_encoding_state *state, void *buffer, int buffer_size)
{
    state->buffer = 0;
    state->offset = 0;
    state->buffer_size = 0;
    state->overflow_flag = 0;

    state->buffer = buffer;
    state->buffer_size = buffer_size;
}
