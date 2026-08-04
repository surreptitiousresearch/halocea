/* global_right3d @ 0x84176F68 — .data data-init (const real_vector3d *).
 * Points at .rdata constant @ 0x82114568 = {0,-1,0}. */
#include "../headers/real_vector3d.h"
static const real_vector3d _global_right3d = { { 0.0f, -1.0f, 0.0f } };
const real_vector3d *global_right3d = &_global_right3d;
