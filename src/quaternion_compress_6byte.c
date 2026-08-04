/* quaternion_compress_6byte @0x83795B78 — pack a quaternion into 6 bytes (the inverse of quaternion_decompress_6byte).
 * Each component is scaled by 32767, truncated to a 16-bit value, and its top 12 bits are packed across the three
 * output words. n[3]=w. */

#include "headers/real_quaternion.h"
#include "headers/compressed_quaternion_6byte.h"

void quaternion_compress_6byte(const real_quaternion *decompressed, compressed_quaternion_6byte *compressed)
{
    unsigned short i = (unsigned short)(int)(decompressed->v.n[0] * 32767.0f);
    unsigned short j = (unsigned short)(int)(decompressed->v.n[1] * 32767.0f);
    unsigned short k = (unsigned short)(int)(decompressed->v.n[2] * 32767.0f);
    unsigned short w = (unsigned short)(int)(decompressed->w * 32767.0f);

    compressed->iiij = (unsigned short)((i & 0xFFF0) | ((j >> 12) & 0xF));
    /* DEVIATION: the decompiler rendered the k term as BYTE2 of a 32-bit int; the disasm (lhz/srwi) shows it is
     * the high byte of the 16-bit k value, i.e. (unsigned short)k >> 8. */
    compressed->jjkk = (unsigned short)(((j << 4) & 0xFF00) | (k >> 8));
    compressed->kwww = (unsigned short)(((k << 8) & 0xF000) | (w >> 4));
}
