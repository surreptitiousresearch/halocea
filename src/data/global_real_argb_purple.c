/* global_real_argb_purple @ 0x84176D8C — .data data-init (const real_argb_color *).
 * Points at unnamed .rdata constant @ 0x82113FC8 = {a=1,r=0.44,g=0.05,b=0.43}. */
#include "headers/real_argb_color.h"
static const real_argb_color _global_real_argb_purple = { 1.0f, 0.44f, 0.05f, 0.43f };
const real_argb_color *global_real_argb_purple = &_global_real_argb_purple;
