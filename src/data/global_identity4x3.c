/* global_identity4x3 @ 0x84176F7C — data-init pointer (4 bytes) to the identity
 * real_matrix4x3 constant in .rdata (target 0x82114520, 52 bytes). Pointee verified
 * big-endian: scale 1.0, rows forward(1,0,0) left(0,1,0) up(0,0,1) position(0,0,0). */
#include "../headers/real_matrix4x3.h"

static const real_matrix4x3 _global_identity4x3 =
{
    1.0f,                              /* scale */
    { { {                              /* anon union -> ___u1 -> n[4][3] */
        { 1.0f, 0.0f, 0.0f },          /* forward */
        { 0.0f, 1.0f, 0.0f },          /* left */
        { 0.0f, 0.0f, 1.0f },          /* up */
        { 0.0f, 0.0f, 0.0f },          /* position */
    } } }
};
const real_matrix4x3 *global_identity4x3 = &_global_identity4x3;
