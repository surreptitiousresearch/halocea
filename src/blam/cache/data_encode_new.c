/* data_encode_new @0x838304E8 — initialize an encoding state over a fixed-size output buffer. */

#include "headers/data_encoding_state.h"

void data_encode_new(data_encoding_state *state, void *buffer, int buffer_size)
{
    state->buffer = 0;
    state->offset = 0;
    state->buffer_size = 0;
    state->overflow_flag = 0; /* recovered: *(int *)&state->overflow_flag -> overflow_flag (4-byte write cleared the 3 pad bytes) */
    state->buffer = buffer;
    state->buffer_size = buffer_size;
}
