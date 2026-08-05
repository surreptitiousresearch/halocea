/* private_identity_quaternion @ 0x8211458C — .rdata, 16 bytes = exactly sizeof(real_quaternion).
 * The identity rotation; src/data/global_identity_quaternion.c holds the .data pointer to it.
 * Initializer reconstructed from the binary (big-endian): 0x00000000, 0x00000000, 0x00000000,
 * 0x3F800000 -> v = (0,0,0), w = 1. Brace nesting targets the DB's {real_vector3d v; float w;}
 * arm of real_quaternion's anonymous union (the n[4] flat view aliases the same 16 bytes). */

#include "../headers/real_quaternion.h"

const real_quaternion private_identity_quaternion =
{
    {                                     /* anonymous union -> {v, w} arm */
        { { { 0.0f, 0.0f, 0.0f } },       /* v (real_vector3d -> n[3]) */
          1.0f }                          /* w */
    }
};
