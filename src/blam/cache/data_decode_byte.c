/* data_decode_byte @0x83830A18 — reads one raw byte from the decode stream (via data_decode_memory), or 0 if
 * the stream is exhausted/overflowed. */

#include <stdint.h>
#include "headers/data_encoding_state.h"

extern char *data_decode_memory(data_encoding_state *state, int16_t count, int code);

uint8_t data_decode_byte(data_encoding_state *state)
{
    uint8_t *byte = (uint8_t *)data_decode_memory(state, 1, 1);

    if (byte)
        return *byte;

    return 0;
}
