/* global_real_argb_violet @ 0x84176D9C — .data data-init (const real_argb_color *).
 * Points at unnamed .rdata constant @ 0x82114008 = {a=1,r=0.81,g=0.13,b=0.56}. */
#include "headers/real_argb_color.h"
static const real_argb_color _global_real_argb_violet = { 1.0f, 0.81f, 0.13f, 0.56f };
const real_argb_color *global_real_argb_violet = &_global_real_argb_violet;
