/* global_real_rgb_grey @ 0x84176DA4 — .data data-init (const real_rgb_color *).
 * Points at unnamed .rdata constant @ 0x82113F1C = {r=0.5,g=0.5,b=0.5}. */
#include "headers/real_rgb_color.h"
static const real_rgb_color _global_real_rgb_grey = { 0.5f, 0.5f, 0.5f };
const real_rgb_color *global_real_rgb_grey = &_global_real_rgb_grey;
