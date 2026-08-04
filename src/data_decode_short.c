/* data_decode_short @0x83830A7C — reads one little-endian 16-bit value from the decode stream (via
 * data_decode_memory), or 0 if the stream is exhausted/overflowed. */

#include <stdint.h>
#include "headers/data_encoding_state.h"

extern char *data_decode_memory(data_encoding_state *state, int16_t count, int code);

int16_t data_decode_short(data_encoding_state *state)
{
    __int16 *value = (__int16 *)data_decode_memory(state, 1, -2);

    if (value)
        return *value;

    return 0;
}
