/* global_real_argb_lightblue @ 0x84176D84 — .data data-init (const real_argb_color *).
 * Points at unnamed .rdata constant @ 0x82113FA8 = {a=1,r=0.39,g=0.58,b=0.93}. */
#include "headers/real_argb_color.h"
static const real_argb_color _global_real_argb_lightblue = { 1.0f, 0.39f, 0.58f, 0.93f };
const real_argb_color *global_real_argb_lightblue = &_global_real_argb_lightblue;
