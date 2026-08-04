/* data_encode_structures @0x838306D0 — append structure_count structures (each bs_definition->size bytes) from
 * source_structures to the encoding buffer as a flat memcpy (byte-swapping itself happens elsewhere; this just
 * moves the raw bytes). Sets the overflow flag and fails if the write would exceed the buffer. Mirrors
 * data_encode_memory.c's overflow-check structure. */

#include <stdint.h>
#include <string.h>
#include "headers/data_encoding_state.h"
#include "headers/byte_swap_definition.h"

uint8_t data_encode_structures(data_encoding_state *state, void *source_structures, int16_t structure_count, byte_swap_definition *bs_definition)
{
    __int16 byte_count = (__int16)(bs_definition->size * structure_count);

    if ( byte_count > 0 )
    {
        int offset = state->offset;
        if ( offset + byte_count > state->buffer_size || state->overflow_flag )
        {
            state->overflow_flag = 1;
        }
        else
        {
            memcpy((char *)state->buffer + offset, source_structures, byte_count);
            state->offset += byte_count;
        }
    }

    return state->overflow_flag == 0;
}
