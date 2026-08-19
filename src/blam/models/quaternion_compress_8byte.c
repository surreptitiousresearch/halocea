/* quaternion_compress_8byte @0x83795B08 — store a quaternion as four signed 16-bit components, each scaled by
 * 32767. n[3]=w. */

#include "headers/real_quaternion.h"
#include "headers/compressed_quaternion_8byte.h"

void quaternion_compress_8byte(const real_quaternion *decompressed, compressed_quaternion_8byte *compressed)
{
    compressed->i = (short)(int)(decompressed->v.n[0] * 32767.0f);
    compressed->j = (short)(int)(decompressed->v.n[1] * 32767.0f);
    compressed->k = (short)(int)(decompressed->v.n[2] * 32767.0f);
    compressed->w = (short)(int)(decompressed->w * 32767.0f);
}
