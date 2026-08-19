/* chicago_fill_const @ 0x836A14E0
   Expands one texture-stage config into 25 boolean shader constants: a one-hot
   selector for the color function (slots 0..11), a one-hot selector for the
   alpha function (slots 12..23), and an alpha-replicate flag (slot 24).

   Deviation: Hex-Rays aliases the by-value stage struct onto the bool_const
   register (SHIWORD/HIWORD/_cntlzw noise). Decoded here to the equivalent
   field comparisons (color_func/alpha_func one-hot over 1..12, a_replicate). */

#include "headers/chicago_shaders.h"

void chicago_fill_const(int *bool_const, shader_stage_cfg stage)
{
    for ( int i = 0; i < 12; ++i )
    {
        bool_const[i]      = (i + 1) == stage.color_func;
        bool_const[12 + i] = (i + 1) == stage.alpha_func;
    }
    bool_const[24] = stage.a_replicate != 0;
}
