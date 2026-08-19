/* data_decode_structures @0x838308F8 */
#include <stdint.h>
#include "headers/data_encoding_state.h"
#include "headers/byte_swap_definition.h"

char * data_decode_structures(data_encoding_state *state, int16_t structure_count, byte_swap_definition *bs_definition)
{
    int total_size = bs_definition->size * structure_count;

    if ( state->offset + total_size > state->buffer_size || state->overflow_flag )
    {
        state->overflow_flag = 1;
        return 0;
    }

    char *result = (char *)state->buffer + state->offset;
    if ( total_size )
        state->offset += total_size;
    return result;
}
