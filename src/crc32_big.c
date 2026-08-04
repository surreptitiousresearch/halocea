/* crc32_big @0x82D61788 — big-endian/reflected slicing-by-4 CRC-32 update (zlib-style "crc32_big"
 * variant): byte-at-a-time until 4-byte alignment, then 32-byte (8-word) unrolled slicing-by-4
 * chunks, then a 4-byte tail loop, then a final byte-at-a-time tail. `crc_table_0` is an 8x256
 * word table (crc_table_0[4..7] are the slicing-by-4 sub-tables this variant indexes).
 */
#include <stdint.h>
#include "headers/crc_globals.h"

extern unsigned int crc_table_0[8][256];

unsigned int crc32_big(unsigned int crc, const uint8_t *buf, unsigned int len)
{
    unsigned int c = ~(((crc & 0xFF00) << 8) | (crc << 24) | ((crc >> 8) & 0xFF00) | (crc >> 24));

    while (len && ((unsigned int)(size_t)buf & 3) != 0) {
        c = crc_table_0[4][(unsigned char)(c >> 24) ^ *buf] ^ (c << 8);
        --len;
        ++buf;
    }

    while (len >= 32) {
        const unsigned int *words = (const unsigned int *)buf;
        /* DEVIATION: the raw decompile inlines 8 successive
         * "r = T[6][(x>>14)&0x3FC] ^ T[5][(x>>6)&0x3FC] ^ T[7][(x>>22)&0x3FC] ^ T[4][(4x)&0x3FC]"
         * folding steps (classic slicing-by-4: the first step's input is word0^acc, each
         * subsequent step's input is the previous step's result XORed with the next word, and the
         * 8th/final step's result — with no further XOR — becomes the new accumulator) with heavy
         * register reuse the decompiler renders as ~15 numbered temporaries; collapsed here to the
         * equivalent explicit fold loop over the 8 words in a 32-byte block. The 0x3FC-masked
         * byte-offset indexing in the raw decompile (`(char*)table + ((x>>k)&0x3FC)`, read back as
         * a DWORD) is a byte-addressed form of `table[(x>>k)&0xFF]`; simplified to direct array
         * indexing here. */
        #define CRC32_BIG_FOLD(x) \
            (crc_table_0[6][((x) >> 14) & 0xFF] ^ crc_table_0[5][((x) >> 6) & 0xFF] ^ \
             crc_table_0[7][((x) >> 22) & 0xFF] ^ crc_table_0[4][(x) & 0xFF])

        unsigned int acc = CRC32_BIG_FOLD(words[0] ^ c) ^ words[1]; // r1
        for (int i = 2; i < 8; ++i)
            acc = CRC32_BIG_FOLD(acc) ^ words[i];                  // r2..r7
        c = CRC32_BIG_FOLD(acc);                                    // final fold, no further XOR

        #undef CRC32_BIG_FOLD
        buf += 32;
        len -= 32;
    }

    while (len >= 4) {
        unsigned int x = *(const unsigned int *)buf ^ c;
        c = crc_table_0[6][(x >> 14) & 0xFF] ^
            crc_table_0[5][(x >> 6) & 0xFF] ^
            crc_table_0[7][(x >> 22) & 0xFF] ^
            crc_table_0[4][x & 0xFF];
        buf += 4;
        len -= 4;
    }

    while (len) {
        c = crc_table_0[4][(unsigned char)(c >> 24) ^ *buf] ^ (c << 8);
        --len;
        ++buf;
    }

    unsigned int ic = ~c;
    return ((ic & 0xFF00) << 8) | (ic << 24) | ((ic >> 8) & 0xFF00) | (ic >> 24);
}
