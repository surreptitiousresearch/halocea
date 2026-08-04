/* rgb_colors_interpolate_and_scale @0x8377DA60 — interpolate the RGB channels of two ARGB colors (via
 * rgb_colors_interpolate) and blend the result with rgb_scale. When both endpoints' alpha is ~0, the
 * result is directly tinted by rgb_scale (component-wise multiply). Otherwise, the interpolated alpha
 * (lerp of the two endpoint alphas by u) weights a blend between the interpolated RGB and rgb_scale:
 * result = interpolated * alpha + rgb_scale * (1 - alpha). If rgb_scale is NULL, no scaling is applied.
 *
 * DEVIATION: the decompiler renders rgb_scale as an unset local ("v9") that's checked/used without any
 * visible assignment — it's simply the 5th parameter, still live in r7 across the rgb_colors_interpolate
 * call; disasm/DB prototype confirm there is no 6th parameter. */

#include "headers/real_rgb_color.h"
#include "headers/real_argb_color.h"

extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);

real_rgb_color *rgb_colors_interpolate_and_scale(real_rgb_color *rgb_result, unsigned int flags,
        const real_argb_color *argb_lower_bound, const real_argb_color *argb_upper_bound,
        const real_rgb_color *rgb_scale, float u)
{
    rgb_colors_interpolate(rgb_result, flags, &argb_lower_bound->rgb, &argb_upper_bound->rgb, u);

    if ( rgb_scale )
    {
        if ( argb_lower_bound->n[0] <= 0.0001f && argb_upper_bound->n[0] <= 0.0001f )
        {
            float old_g = rgb_result->n[1];
            float old_b = rgb_result->n[2];
            rgb_result->n[0] = rgb_result->n[0] * rgb_scale->n[0];
            rgb_result->n[1] = rgb_scale->n[1] * old_g;
            rgb_result->n[2] = rgb_scale->n[2] * old_b;
            return rgb_result;
        }

        float alpha = (1.0f - u) * argb_lower_bound->n[0] + argb_upper_bound->n[0] * u;
        float complement = 1.0f - alpha;

        float old_g = rgb_result->n[1];
        float old_b = rgb_result->n[2];
        rgb_result->n[0] = rgb_result->n[0] * alpha + rgb_scale->n[0] * complement;
        rgb_result->n[1] = rgb_scale->n[1] * complement + old_g * alpha;
        rgb_result->n[2] = rgb_scale->n[2] * complement + old_b * alpha;
    }
    return rgb_result;
}
