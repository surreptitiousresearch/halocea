/* data_encode_array @0x83830750 — serialize a counted array into a data-encoding stream: first the element
 * count in the width named by element_count_size (the byte_swap_code convention — _8byte/_4byte/_2byte are
 * byte-swapped 8/4/2-byte fields, _1byte is a plain byte), then the elements themselves via their byte-swap
 * definition. Returns whether the stream did not overflow.
 *
 * DEVIATION (disasm 0x838307B8-0x838307F8): all four arms stage the count into the SAME stack slot (var_30)
 * and pass its address unchanged (`addi r4, r1, 0x80+var_30` in every case) — one coalesced typed local per
 * arm, not offsets into a shared byte block. An earlier reconstruction staged -2 at +2 and 1 at +3 and handed
 * data_encode_memory uninitialized leading bytes; and staged -8 as a zeroed high word where the binary
 * sign-extends (`extsw r11, r30 ; std r11` @0x838307E8). */

#include <stdint.h>
#include "headers/data_encoding_state.h"
#include "headers/byte_swap_definition.h"

extern int data_encode_memory(data_encoding_state *state, const void *buffer, int16_t count, int code);
extern uint8_t data_encode_structures(data_encoding_state *state, void *source_structures, int16_t structure_count, byte_swap_definition *bs_definition);

uint8_t data_encode_array(data_encoding_state *state, int element_count_size, void *source_array, int element_count, byte_swap_definition *bs_definition)
{
    int16_t count = element_count;

    switch ( element_count_size )
    {
    case _8byte:
    {
        int64_t staged_count = element_count;             /* extsw r11, r30 ; std r11 @0x838307E8 */
        data_encode_memory(state, &staged_count, 1, _8byte);
        break;
    }
    case _4byte:
    {
        int32_t staged_count = element_count;             /* stw r30 @0x838307D8 */
        data_encode_memory(state, &staged_count, 1, _4byte);
        break;
    }
    case _2byte:
    {
        int16_t staged_count = (int16_t)element_count;    /* sth r30 @0x838307CC */
        data_encode_memory(state, &staged_count, 1, _2byte);
        break;
    }
    case _1byte:
    {
        uint8_t staged_count = (uint8_t)element_count;    /* stb r30 @0x838307BC */
        data_encode_memory(state, &staged_count, 1, _1byte);
        break;
    }
    default:
        break;
    }

    data_encode_structures(state, source_array, count, bs_definition);
    return state->overflow_flag == 0;
}
