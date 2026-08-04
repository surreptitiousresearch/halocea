/* global_real_argb_white @ 0x84176D5C — data-init pointer (4 bytes) to a {1,1,1,1}
 * real_argb_color constant in .rdata (target 0x82113F08). Pointee verified big-endian
 * = alpha 1.0, r 1.0, g 1.0, b 1.0. */
#include "../headers/real_argb_color.h"

static const real_argb_color _global_real_argb_white = { { 1.0f, 1.0f, 1.0f, 1.0f } };
const real_argb_color *global_real_argb_white = &_global_real_argb_white;
