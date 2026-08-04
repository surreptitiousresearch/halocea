/* global_real_argb_blue @ 0x84176D70 — .data data-init (const real_argb_color *).
 * Points at unnamed .rdata constant @ 0x82113F58 = {a=1,r=0,g=0,b=1}. */
#include "headers/real_argb_color.h"
static const real_argb_color _global_real_argb_blue = { 1.0f, 0.0f, 0.0f, 1.0f };
const real_argb_color *global_real_argb_blue = &_global_real_argb_blue;
