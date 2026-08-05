/* data_decode_array @0x83830C08 — counterpart of data_encode_array.c: read an element count in the width
 * given by element_count_size (byte-swap convention: negative = byte-swapped 8/4/2, 1 = plain byte), clamp
 * it to [0, maximum_element_count] (rejecting negative or over-large counts, or a stream already in
 * overflow), then decode that many elements via their byte-swap definition.
 *
 * SHIPPED QUIRK, disasm-verified: element_count_size values outside {-8,-4,-2,1} read the count from an
 * uninitialized stack slot (a jump-table default case with no real handler) — reproduced verbatim; this
 * path is presumably unreachable with valid tag data. */

#include <stdint.h>
#include "headers/data_encoding_state.h"
#include "headers/byte_swap_definition.h"

extern uint64_t data_decode_int64(data_encoding_state *state);
extern int data_decode_long(data_encoding_state *state);
extern int16_t data_decode_short(data_encoding_state *state);
extern uint8_t data_decode_byte(data_encoding_state *state);
extern char * data_decode_structures(data_encoding_state *state, int16_t structure_count, byte_swap_definition *bs_definition);

void * data_decode_array(data_encoding_state *state, int element_count_size, int *element_count_reference, int maximum_element_count, byte_swap_definition *bs_definition)
{
    int count;
    int uninitialized_count;   /* shipped: read raw for unhandled element_count_size values */

    switch ( element_count_size )
    {
    case -8:
        count = (int)(data_decode_int64(state) >> 32);
        break;
    case -4:
        count = data_decode_long(state);
        break;
    case -2:
        count = data_decode_short(state);
        break;
    case 1:
        count = data_decode_byte(state);
        break;
    default:
        count = uninitialized_count;
        break;
    }

    if ( state->overflow_flag || count < 0 || count > maximum_element_count )
        return 0;

    *element_count_reference = count;
    return data_decode_structures(state, count, bs_definition);
}
