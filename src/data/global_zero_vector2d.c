/* global_zero_vector2d @ 0x84176F24 — .data data-init (const real_vector2d *).
 * Points at shared .rdata zero-constant @ 0x8211453C = {0,0}. */
#include "../headers/real_vector2d.h"
static const real_vector2d _global_zero_vector2d = { { 0.0f, 0.0f } };
const real_vector2d *global_zero_vector2d = &_global_zero_vector2d;
