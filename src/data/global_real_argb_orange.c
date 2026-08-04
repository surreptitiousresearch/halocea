/* global_real_argb_orange @ 0x84176D88 — .data data-init (const real_argb_color *).
 * Points at unnamed .rdata constant @ 0x82113FB8 = {a=1,r=1,g=0.5,b=0}. */
#include "headers/real_argb_color.h"
static const real_argb_color _global_real_argb_orange = { 1.0f, 1.0f, 0.5f, 0.0f };
const real_argb_color *global_real_argb_orange = &_global_real_argb_orange;
