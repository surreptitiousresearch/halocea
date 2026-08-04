/* global_real_rgb_blue @ 0x84176DB4 — .data data-init (const real_rgb_color *).
 * Points at unnamed .rdata constant @ 0x82113F5C = {r=0,g=0,b=1}. */
#include "headers/real_rgb_color.h"
static const real_rgb_color _global_real_rgb_blue = { 0.0f, 0.0f, 1.0f };
const real_rgb_color *global_real_rgb_blue = &_global_real_rgb_blue;
