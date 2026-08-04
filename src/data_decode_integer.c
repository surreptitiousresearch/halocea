/* data_decode_integer @0x83830B58 — reads a value from the decode stream sized to fit `maximum_value`: a byte
 * if it fits in 8 bits, a short if it fits in 16 bits, else a full 32-bit int. Returns 0 if the stream is
 * exhausted/overflowed. Mirrors the encode-side tiering in `data_encode_integer`. */

#include <stdint.h>
#include "headers/data_encoding_state.h"

extern char *data_decode_memory(data_encoding_state *state, int16_t count, int code);
extern int16_t data_decode_short(data_encoding_state *state);

int16_t data_decode_integer(data_encoding_state *state, int maximum_value)
{
    if (maximum_value > 255)
    {
        if (maximum_value > 0xFFFF)
        {
            int *value = (int *)data_decode_memory(state, 1, -4);
            if (value)
                return *value;
            return 0;
        }

        return data_decode_short(state);
    }

    unsigned __int8 *value = (unsigned __int8 *)data_decode_memory(state, 1, 1);
    if (value)
        return *value;
    return 0;
}
