/* global_real_rgb_white @ 0x84176DA0 — data-init pointer (4 bytes) to a {1,1,1}
 * real_rgb_color constant in .rdata (target 0x82113F0C). Pointee verified big-endian
 * = r 1.0, g 1.0, b 1.0. */
#include "../headers/real_rgb_color.h"

static const real_rgb_color _global_real_rgb_white = { { 1.0f, 1.0f, 1.0f } };
const real_rgb_color *global_real_rgb_white = &_global_real_rgb_white;
