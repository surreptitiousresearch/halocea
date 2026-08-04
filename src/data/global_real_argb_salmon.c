/* global_real_argb_salmon @ 0x84176D98 — .data data-init (const real_argb_color *).
 * Points at unnamed .rdata constant @ 0x82113FF8 = {a=1,r=1,g=0.63,b=0.48}. */
#include "headers/real_argb_color.h"
static const real_argb_color _global_real_argb_salmon = { 1.0f, 1.0f, 0.63f, 0.48f };
const real_argb_color *global_real_argb_salmon = &_global_real_argb_salmon;
