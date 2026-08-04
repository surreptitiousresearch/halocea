/* global_real_argb_black @ 0x84176D64 — .data data-init (const real_argb_color *).
 * Points at unnamed .rdata constant @ 0x82113F28 = {a=1,r=0,g=0,b=0}. */
#include "headers/real_argb_color.h"
static const real_argb_color _global_real_argb_black = { 1.0f, 0.0f, 0.0f, 0.0f };
const real_argb_color *global_real_argb_black = &_global_real_argb_black;
