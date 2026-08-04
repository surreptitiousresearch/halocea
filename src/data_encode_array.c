/* data_encode_array @0x83830750 — serialize a counted array into a data-encoding stream: first the element
 * count in the requested width (element_count_size uses the byte-swap convention: negative = byte-swapped
 * 8/4/2, 1 = plain byte; the count is staged in the correctly-aligned tail bytes of a local 8-byte block,
 * exactly as compiled), then the elements themselves via their byte-swap definition. Returns whether the
 * stream did not overflow. */

#include <stdint.h>
#include "headers/data_encoding_state.h"
#include "headers/byte_swap_definition.h"

extern int data_encode_memory(data_encoding_state *state, const void *buffer, int16_t count, int code);
extern uint8_t data_encode_structures(data_encoding_state *state, void *source_structures, int16_t structure_count, byte_swap_definition *bs_definition);

uint8_t data_encode_array(data_encoding_state *state, int element_count_size, void *source_array, int element_count, byte_swap_definition *bs_definition)
{
    __int16 count = element_count;
    unsigned char staged[8];

    switch ( element_count_size )
    {
    case -8:
        *(int *)&staged[0] = 0;
        *(int *)&staged[4] = element_count;
        data_encode_memory(state, staged, 1, -8);
        break;
    case -4:
        *(int *)&staged[0] = element_count;
        data_encode_memory(state, staged, 1, -4);
        break;
    case -2:
        *(__int16 *)&staged[2] = count;
        data_encode_memory(state, staged, 1, -2);
        break;
    case 1:
        staged[3] = (unsigned char)count;
        data_encode_memory(state, staged, 1, 1);
        break;
    default:
        break;
    }

    data_encode_structures(state, source_array, count, bs_definition);
    return state->overflow_flag == 0;
}
