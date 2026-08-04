#include "../headers/real_argb_color.h"
/* points to anonymous .rdata constant; bytes {1,1,0,0} (n[4]=a,r,g,b) */
static const real_argb_color _global_real_argb_red = { { 1.0f, 1.0f, 0.0f, 0.0f } };
const real_argb_color *global_real_argb_red = &_global_real_argb_red;
