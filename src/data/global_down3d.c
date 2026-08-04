/* global_down3d @ 0x84176F6C — data-init pointer (4 bytes) to a {0,0,-1} real_vector3d
 * constant in .rdata (target 0x82114574). Pointee verified big-endian = 0.0,0.0,-1.0. */
#include "../headers/real_vector3d.h"

static const real_vector3d _global_down3d = { { 0.0f, 0.0f, -1.0f } };
const real_vector3d *global_down3d = &_global_down3d;
