/* global_forward3d @ 0x84176F58 — data-init pointer (4 bytes) to a {1,0,0} real_vector3d
 * constant in .rdata (original target 0x82114524). Pointee bytes verified big-endian = 1.0,0.0,0.0. */
#include "../headers/real_vector3d.h"

static const real_vector3d _global_forward3d = { { 1.0f, 0.0f, 0.0f } };
const real_vector3d *global_forward3d = &_global_forward3d;
