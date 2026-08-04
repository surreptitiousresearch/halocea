/* global_real_argb_darkgreen @ 0x84176D94 — .data data-init (const real_argb_color *).
 * Points at unnamed .rdata constant @ 0x82113FE8 = {a=1,r=0,g=0.39,b=0}. */
#include "headers/real_argb_color.h"
static const real_argb_color _global_real_argb_darkgreen = { 1.0f, 0.0f, 0.39f, 0.0f };
const real_argb_color *global_real_argb_darkgreen = &_global_real_argb_darkgreen;
