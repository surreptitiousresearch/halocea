/* global_real_rgb_green @ 0x84176DB0 — .data data-init (const real_rgb_color *).
 * Points at unnamed .rdata constant @ 0x82113F4C = {r=0,g=1,b=0}. */
#include "headers/real_rgb_color.h"
static const real_rgb_color _global_real_rgb_green = { 0.0f, 1.0f, 0.0f };
const real_rgb_color *global_real_rgb_green = &_global_real_rgb_green;
