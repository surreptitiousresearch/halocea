/* global_real_argb_pink @ 0x84176D80 — .data data-init (const real_argb_color *).
 * Points at unnamed .rdata constant @ 0x82113F98 = {a=1,r=1,g=0.41,b=0.7}. */
#include "headers/real_argb_color.h"
static const real_argb_color _global_real_argb_pink = { 1.0f, 1.0f, 0.41f, 0.7f };
const real_argb_color *global_real_argb_pink = &_global_real_argb_pink;
