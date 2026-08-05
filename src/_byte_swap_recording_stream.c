/* _byte_swap_recording_stream @0x837F7570 — byte-swap an AI/unit recorded-animation playback stream in place
 * (for cross-endian save/replay). It first swaps the unit-control header (version-dependent) and a 6-short
 * fixed block, then walks the variable-length event stream: each event begins with a control byte whose low two
 * bits select an inline operand to swap (2 = skip one byte, 3 = swap one short and skip past it) and whose upper
 * six bits are the opcode. Walking stops at the terminator opcode (1). The stream is self-terminating, so `size`
 * is not consulted.
 *
 * DEVIATION: the decompiler's redundant cursor writes (dead `v7 = v3` stores) are dropped; the opcode dispatch,
 * a degenerate switch whose only case falls through to the loop, is reduced to the terminator test. */

extern void recorded_animation_byteswap_unit_control(char **data, uint8_t unit_control_version);
extern void _byte_swap_memory(void *address, int count, int element_size);

#include <stdint.h>
#include "headers/byte_swap_definition.h"

void _byte_swap_recording_stream(char *data, int size, uint8_t unit_control_version)
{
    char *cursor = data;
    recorded_animation_byteswap_unit_control(&cursor, unit_control_version);
    _byte_swap_memory(cursor, 6, _2byte);

    char *event = cursor + 12;  /* byte-swap cursor: skip the 6-short block just swapped (irreducible stream offset) */
    while ( 1 )
    {
        char *control = event++;
        int operand_type = *control & 3;
        if ( operand_type == 2 )
        {
            ++event;
        }
        else if ( operand_type == 3 )
        {
            _byte_swap_memory(event, 1, _2byte);
            event = control + 3;
        }

        if ( (uint8_t)*control >> 2 == 1 )   /* terminator opcode */
            break;
    }
}
