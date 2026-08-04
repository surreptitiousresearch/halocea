/* rasterizer_dx9_decrypt_buffer @0x8369D570 — hcex bridge: decrypt a shader/asset buffer in place with the
 * fixed 16-byte key {0x3FFFFFDD, 0x7FC3, 0xE5, 0x3FFFEF} and validate it by checking that the trailing
 * 33 bytes decrypted to all zeroes. Returns 1 when valid; buffers of 33 bytes or fewer are rejected. */

#include <stdint.h>
#include "headers/DecryptBuffer.h"

extern void *memset(void *dst, int value, unsigned int size);

uint8_t rasterizer_dx9_decrypt_buffer(uint8_t *buffer, unsigned int bufferSize)
{
    unsigned char zero_trailer[33];
    memset(zero_trailer, 0, 33);

    if ( bufferSize <= 0x21 )
        return 0;

    unsigned int key[4];
    key[0] = 0x3FFFFFDD;
    key[1] = 0x7FC3;
    key[2] = 0xE5;
    key[3] = 0x3FFFEF;
    DecryptBuffer(bufferSize, buffer, key);

    const unsigned char *expected = zero_trailer;
    const unsigned char *trailer = &buffer[bufferSize - 33];
    int difference = 0;
    do
    {
        difference = *trailer - *expected;
        if ( difference )
            break;
        ++trailer;
        ++expected;
    }
    while ( trailer != &buffer[bufferSize] );

    return difference == 0;
}
