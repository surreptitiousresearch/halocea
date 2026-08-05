/* data_encode_integer @0x83830640 — encode an integer into the buffer using the smallest element that holds
 * maximum_value (1 byte for <=255, 2 bytes for <=65535, otherwise 4 bytes), big-endian. Returns no-overflow.
 *
 * Deviation: the decompiler's HIBYTE/HIWORD stores place the value in the high-order bytes of a temp int; on
 * the big-endian target those are the leading bytes, so the temp is filled via leading-byte writes. */

#include <stdint.h>
#include "headers/data_encoding_state.h"

extern int data_encode_memory(data_encoding_state *state, const void *buffer, int16_t count, int code);

int data_encode_integer(data_encoding_state *state, int value, int maximum_value)
{
    int encoded = 0;
    int code;
    if ( maximum_value > 255 )
    {
        if ( maximum_value > 0xFFFF )
        {
            encoded = value;          /* full 32-bit */
            code = -4;
        }
        else
        {
            *(int16_t *)&encoded = value; /* HIWORD on big-endian = leading 2 bytes */
            code = -2;
        }
    }
    else
    {
        *(char *)&encoded = value;        /* HIBYTE on big-endian = leading byte */
        code = 1;
    }
    data_encode_memory(state, &encoded, 1, code);
    return state->overflow_flag == 0;
}
