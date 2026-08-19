/* data_encode_integer @0x83830628 — encode an integer into the buffer using the smallest element that holds
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
    uint8_t *encoded_bytes = (uint8_t *)&encoded;   /* data_encode_memory memcpy's the LEADING byte_count bytes */
    int code;
    if ( maximum_value > 255 )
    {
        if ( maximum_value > 0xFFFF )
        {
            /* DEVIATION: `encoded = value` emitted the word in host order onto the wire (big-endian
             * on PPC, little-endian on x64) — same wire-order class as the halfword path below.
             * Spelled as explicit big-endian leading-byte writes, byte-identical on PPC. 2026-08-18 */
            encoded_bytes[0] = (uint8_t)((unsigned int)value >> 24);
            encoded_bytes[1] = (uint8_t)((unsigned int)value >> 16);
            encoded_bytes[2] = (uint8_t)((unsigned int)value >> 8);
            encoded_bytes[3] = (uint8_t)value;
            code = -4;
        }
        else
        {
            /* DEVIATION: the `*(int16_t *)&encoded = value` pun was NOT endian-neutral — it emits the
             * halfword in host order into the two leading bytes that data_encode_memory copies, so an
             * x64 build would put the encoding on the wire little-endian. Spelled as explicit
             * big-endian leading-byte writes, byte-identical to the image on PPC. 2026-08-18 */
            encoded_bytes[0] = (uint8_t)((unsigned int)value >> 8);
            encoded_bytes[1] = (uint8_t)value;
            code = -2;
        }
    }
    else
    {
        /* single leading byte: the low 8 bits of `value` land at &encoded[0] on either endianness,
         * so this one is already endian-neutral — kept as an explicit leading-byte write. */
        encoded_bytes[0] = (uint8_t)value;
        code = 1;
    }
    data_encode_memory(state, &encoded, 1, code);
    return state->overflow_flag == 0;
}
