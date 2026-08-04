/* quaternion_decompress_6byte_renormalized @0x83795AD0 — decompress a 6-byte quaternion and renormalize it. */

#include "headers/compressed_quaternion_6byte.h"
#include "headers/real_quaternion.h"

extern void quaternion_decompress_6byte(const compressed_quaternion_6byte *compressed, real_quaternion *decompressed);
extern void quaternion_normalize(real_quaternion *q);

void quaternion_decompress_6byte_renormalized(const compressed_quaternion_6byte *compressed, real_quaternion *decompressed)
{
    quaternion_decompress_6byte(compressed, decompressed);
    quaternion_normalize(decompressed);
}
