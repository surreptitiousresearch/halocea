/* quaternion_decompress_6byte @0x837959F0 — expand a 6-byte (48-bit) compressed quaternion into four floats.
 * The 48 bits hold four 12-bit signed components (i,j,k,w) that straddle the three 16-bit words. Each component is
 * expanded to a 16-bit value by placing the 12 bits in bits 4..15 and replicating the high nibble into bits 0..3
 * (fixed-point bit replication for extra precision), then scaled by 1/32767.
 *
 * Reconstructed from disassembly — the decompiler's local allocation failed (overlapped __int128 scratch). The
 * component bit-extraction below is transcribed directly from the rlwimi/extrwi sequence. */

#include "headers/compressed_quaternion_6byte.h"
#include "headers/real_quaternion.h"

/* 0x38000100: 1/32767 — 0x38000100 decodes to 3.0518509447574615e-05, the float nearest 1/32767
 * (1/32768 would be 0x38000000), and 0x38000100 * 32767 = 0.999999999, exactly inverting
 * the x 32767.0f encode.
 * Same constant as the rotation component scale used throughout the animation decoder;
 * loaded at `lfs f0, __real_38000100@l(r10)` @0x83795A0C. */
#define QUATERNION_COMPONENT_SCALE 0.000030518509f

void quaternion_decompress_6byte(const compressed_quaternion_6byte *compressed, real_quaternion *decompressed)
{
    unsigned int iiij = compressed->iiij;   /* word 0 */
    unsigned int jjkk = compressed->jjkk;   /* word 1 */
    unsigned int kwww = compressed->kwww;   /* word 2 */

    short i = (short)((iiij & 0xFFF0) | (iiij >> 12));
    short j = (short)(((iiij & 0xF) | ((jjkk >> 4) & 0xFF0)) | (iiij << 12));
    short k = (short)(((((kwww >> 4) & 0xF0F) | (jjkk & 0xF0)) >> 4 & 0xFFF) | (jjkk << 8));
    short w = (short)(((kwww >> 8) & 0xF) | (kwww << 4));

    decompressed->v.n[0] = (float)i * QUATERNION_COMPONENT_SCALE;
    decompressed->v.n[1] = (float)j * QUATERNION_COMPONENT_SCALE;
    decompressed->v.n[2] = (float)k * QUATERNION_COMPONENT_SCALE;
    decompressed->w = (float)w * QUATERNION_COMPONENT_SCALE;
}
