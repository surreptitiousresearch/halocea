/* rgb_colors_interpolate @ 0x8377D920 — interpolate between two RGB colors by u in [0,1]. With
 * flags bit0 clear it is a straight per-channel linear lerp. With flags bit0 set the interpolation is
 * done in HSV space so hue rotates correctly: both endpoints are converted to HSV, the hue is
 * interpolated along the shorter (or, per flags bit1, the longer) arc around the colour wheel, then the
 * result is converted back to RGB. Returns rgb_result.
 *
 * Reconstructed against the disassembly: the decompiler mangled the float weights via the long-double
 * ABI. f7=1.0, f6=(1-u), f1=u; the "wrap toggle" is (flags>>1)&1 compared against (|dh|>0.5). */

#include "headers/real_rgb_color.h"
#include "headers/real_hsv_color.h"
#include "headers/rgb_colors_interpolate_flags.h"

extern real_hsv_color *real_rgb_color_to_real_hsv_color(const real_rgb_color *rgb, real_hsv_color *hsv);
extern real_rgb_color *real_hsv_color_to_real_rgb_color(const real_hsv_color *hsv, real_rgb_color *rgb);
extern double __fabs(double x);

/* attested: flags is only bit-tested (clrlwi ...,31 / extrwi 1,30) — width-agnostic, so plain
 * unsigned int; the previous `char` claimed an 8-bit width the binary never pins. */
real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags,
    const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u)
{
    if ( (flags & (1u << _rgb_colors_interpolate_in_hsv_space_bit)) != 0 )
    {
        real_hsv_color hsv_lower, hsv_upper, hsv_result;
        real_rgb_color_to_real_hsv_color(rgb_lower_bound, &hsv_lower);
        real_rgb_color_to_real_hsv_color(rgb_upper_bound, &hsv_upper);

        float lower_hue = hsv_lower.n[0];
        float upper_hue = hsv_upper.n[0];

        /* Rotate the smaller hue forward by a full turn when the requested arc (short vs long, per
         * flags bit1) disagrees with the natural one implied by the half-turn distance. */
        unsigned char crosses_half = (__fabs(lower_hue - upper_hue) > 0.5f);
        if ( crosses_half != (unsigned char)((flags >> _rgb_colors_interpolate_along_farthest_hue_path_bit) & 1) )
        {
            if ( lower_hue >= upper_hue )
                upper_hue = upper_hue + 1.0f;
            else
                lower_hue = lower_hue + 1.0f;
        }

        float interpolated_hue = lower_hue * (1.0f - u) + upper_hue * u;
        if ( interpolated_hue > 1.0f )
            interpolated_hue = interpolated_hue - 1.0f;
        hsv_result.n[0] = interpolated_hue;
        hsv_result.n[1] = hsv_lower.n[1] * (1.0f - u) + hsv_upper.n[1] * u;
        hsv_result.n[2] = hsv_lower.n[2] * (1.0f - u) + hsv_upper.n[2] * u;

        real_hsv_color_to_real_rgb_color(&hsv_result, rgb_result);
        return rgb_result;
    }

    rgb_result->red   = rgb_lower_bound->red   * (1.0f - u) + rgb_upper_bound->red   * u;
    rgb_result->green = rgb_lower_bound->green * (1.0f - u) + rgb_upper_bound->green * u;
    rgb_result->blue  = rgb_lower_bound->blue  * (1.0f - u) + rgb_upper_bound->blue  * u;
    return rgb_result;
}
