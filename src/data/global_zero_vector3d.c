/* global_zero_vector3d @ 0x84176F54 — data-init pointer (4 bytes) to a {0,0,0} real_vector3d
 * constant in .rdata (original target 0x82114548). Reconstructed as a pointer to a static const
 * zero vector; the pointee bytes were verified big-endian = 0.0,0.0,0.0. */
#include "../headers/real_vector3d.h"

static const real_vector3d _global_zero_vector3d = { { 0.0f, 0.0f, 0.0f } };
const real_vector3d *global_zero_vector3d = &_global_zero_vector3d;
