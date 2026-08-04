/* brighten_real_rgb_color @0x836F8268 — scale an RGB color up by `fraction` while preserving hue. The scale is
 * normally (1 + fraction), but it is capped so the brightest channel never exceeds 1.0, and floored so the
 * brightest channel reaches at least `fraction`. A fully black color is left unchanged. */

#include "headers/real_rgb_color.h"

void brighten_real_rgb_color(real_rgb_color *color, float fraction)
{
    float max_channel = (color->n[1] <= color->n[2]) ? color->n[2] : color->n[1];
    if ( color->n[0] > max_channel )
        max_channel = color->n[0];

    float scale = fraction + 1.0f;
    if ( max_channel == 0.0f )
    {
        scale = 1.0f;
    }
    else
    {
        float scaled_max = (fraction + 1.0f) * max_channel;
        if ( scaled_max <= 1.0f )
        {
            if ( scaled_max < fraction )
                scale = fraction / max_channel;
        }
        else
        {
            scale = 1.0f / max_channel;
        }
    }

    color->n[0] = color->n[0] * scale;
    color->n[1] = color->n[1] * scale;
    color->n[2] = color->n[2] * scale;
}
