/* one_over_full_circle @ 0x821285E0 — .rdata, 4 bytes, const float. Initializer reconstructed
 * from the binary (big-endian):
 *   +0x0000 0x3E22F983 -> 0.15915494f = 1/(2*pi), already named INV_TWO_PI in
 *                         src/headers/math_constants.h (bit-exact round trip)
 * Consumed by src/build_sprite_rotational.c to turn a rotation in radians into a turn fraction.
 * The declared type in that TU (`const float`) matches. */

#include "../headers/math_constants.h"

const float one_over_full_circle = INV_TWO_PI;
