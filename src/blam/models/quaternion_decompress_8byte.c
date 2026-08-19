/* quaternion_decompress_8byte @0x83795960 — expand a quaternion stored as four signed 16-bit components into
 * floats, scaling each by 1/32768. n[3]=w. */

#include "headers/real_quaternion.h"
#include "headers/compressed_quaternion_8byte.h"

#define QUATERNION_COMPONENT_SCALE 0.000030518509f

void quaternion_decompress_8byte(const compressed_quaternion_8byte *compressed, real_quaternion *decompressed)
{
    decompressed->v.n[0] = (float)compressed->i * QUATERNION_COMPONENT_SCALE;
    decompressed->v.n[1] = (float)compressed->j * QUATERNION_COMPONENT_SCALE;
    decompressed->v.n[2] = (float)compressed->k * QUATERNION_COMPONENT_SCALE;
    decompressed->w = (float)compressed->w * QUATERNION_COMPONENT_SCALE;
}
