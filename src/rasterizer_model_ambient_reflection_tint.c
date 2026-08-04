/* rasterizer_model_ambient_reflection_tint @0x8378D244 — set the global model ambient-reflection
 * tint (brightness in alpha slot, then RGB), when the tint block is present.
 *
 * DEVIATION: the four color components arrive as doubles via the soft-float ABI; the stored fields
 * are floats (real_argb_color::n[]). */

#include "headers/real_argb_color.h"

extern real_argb_color *global_rasterizer_model_ambient_reflection_tint;

void rasterizer_model_ambient_reflection_tint(float brightness, float red, float green, float blue)
{
    real_argb_color *tint = global_rasterizer_model_ambient_reflection_tint;
    if ( tint )
    {
        tint->n[0] = brightness;
        tint->n[1] = red;
        tint->n[2] = green;
        tint->n[3] = blue;
    }
}
