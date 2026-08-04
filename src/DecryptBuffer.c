#include <stdint.h>
#include "headers/DecryptBuffer.h"

/* DecryptBuffer — decrypts bufferSizeInBytes bytes of buffer in place, one 8-byte TEA block at a time.
 * The main loop reimplements the same 32-round TEA decrypt as decode_1 (compiler-inlined rather than
 * called) over each full 8-byte block; when bufferSizeInBytes is not a multiple of 8, the trailing 8
 * bytes ending at buffer[bufferSizeInBytes] are additionally decrypted via a decode_1 call before the
 * main loop runs (a tail special case — matches disassembly at 0x83688960 exactly). Each block is a raw
 * 8-byte pair of 32-bit words (no DB struct) reached via a typed word cursor. */
void DecryptBuffer(unsigned int bufferSizeInBytes, uint8_t *buffer, const unsigned int *key)
{
    unsigned int blocks_remaining;
    unsigned int *block; /* the current 8-byte cipher block as two 32-bit words (raw, no DB struct) */
    unsigned int word0;
    unsigned int word1;
    unsigned int sum;
    unsigned int round_count;
    unsigned int decrypted_word0;

    if ( bufferSizeInBytes < 8 )
        return;

    if ( bufferSizeInBytes % 8 )
        decode_1((unsigned int *)&buffer[bufferSizeInBytes - 8], key);

    blocks_remaining = bufferSizeInBytes / 8;
    if ( blocks_remaining > 0 )
    {
        block = (unsigned int *)buffer;
        do
        {
            word0 = block[0];
            word1 = block[1];
            sum = 0xC6EF3720U; /* 0x9E3779B9 * 32, initial sum for a 32-round decrypt */
            round_count = 32;

            do
            {
                word1 -= ((word0 >> 5) + key[3]) ^ (16 * word0 + key[2]) ^ (sum + word0);
                decrypted_word0 = (16 * word1 + key[0]) ^ ((word1 >> 5) + key[1]) ^ (sum + word1);
                sum -= 0x9E3779B9U;
                word0 -= decrypted_word0;
                --round_count;
            }
            while ( round_count );

            block[0] = word0;
            block[1] = word1;
            --blocks_remaining;
            block += 2; /* advance one 8-byte block */
        }
        while ( blocks_remaining );
    }
}
