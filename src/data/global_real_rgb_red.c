/* global_real_rgb_red @ 0x84176DAC — .data data-init (const real_rgb_color *).
 * Points at unnamed .rdata constant @ 0x82113F3C = {r=1,g=0,b=0}. */
#include "headers/real_rgb_color.h"
static const real_rgb_color _global_real_rgb_red = { 1.0f, 0.0f, 0.0f };
const real_rgb_color *global_real_rgb_red = &_global_real_rgb_red;
