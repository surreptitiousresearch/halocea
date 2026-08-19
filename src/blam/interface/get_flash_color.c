/* get_flash_color @0x8379E5F8 — resolve a hud_color_definition's current color, animating a flash
 * pulse (cosine ease in/out between base and flash color) when the element has flashed for less than
 * `number_of_flashes` cycles since `reference_value`. Verified via disasm_range(0x8379E5F8,
 * 0x8379E5F8+728) — decompile is clean here (no "local variable allocation failed" warning) except
 * that the vectors_interpolate/scalars_interpolate destination locals are the same reused `result`
 * struct across both interpolate calls (confirmed by stack offset: the scalar call's dest sits exactly
 * at result.n[3]).
 *
 * DEVIATION: the packed-ARGB-return expression the decompiler repeats at every exit is the literal
 * inlined body of real_argb_color_to_pixel32 (confirmed identical via decompile(0x836A3AE0) — this
 * function has no `bl` to it, the compiler inlined the 116B helper); calls that helper instead of
 * repeating the expression 5 times. */

#include "headers/hud_color_definition.h"
#include "headers/hud_flash_reverse_colors_flags.h"
#include "headers/real_argb_color.h"
#include "headers/real_vector3d.h"
#include "headers/game_time_constants.h"

extern int game_time_get(void);
extern double fmod(double x, double y);
extern double cos(double x);
extern float __fsqrts(float x);
extern real_argb_color *pixel32_to_real_argb_color(unsigned int pixel, real_argb_color *color);
extern unsigned int real_argb_color_to_pixel32(const real_argb_color *color);
extern void vectors_interpolate(const real_vector3d *a, const real_vector3d *b, float t, real_vector3d *result);
extern void scalars_interpolate(float a, float b, float t, float *result);

unsigned int get_flash_color(const hud_color_definition *hud_color_def, int reference_value)
{
    if (hud_color_def->flash_period == 0.0f || hud_color_def->flash_length == 0.0f)
    {
        real_argb_color color;
        pixel32_to_real_argb_color(hud_color_def->color, &color);
        return real_argb_color_to_pixel32(&color);
    }

    float elapsed_seconds = (float)(game_time_get() - reference_value) * SECONDS_PER_TICK;
    float flash_phase = (float)fmod(elapsed_seconds, hud_color_def->flash_period);

    real_argb_color base_color, flash_color;
    pixel32_to_real_argb_color(hud_color_def->color, &base_color);
    pixel32_to_real_argb_color(hud_color_def->flash_color, &flash_color);

    if (flash_phase >= (float)hud_color_def->number_of_flashes
                        * (hud_color_def->flash_delay + hud_color_def->flash_length))
        return real_argb_color_to_pixel32(&base_color);

    float flash_cycle_phase = (float)fmod(flash_phase, hud_color_def->flash_delay + hud_color_def->flash_length);

    if (!reference_value)
    {
        if (hud_color_def->flash_flags & (1u << _hud_flash_reverse_colors_bit))
            return real_argb_color_to_pixel32(&base_color);
        return real_argb_color_to_pixel32(&flash_color);
    }

    if (flash_cycle_phase >= hud_color_def->flash_length)
    {
        if (!(hud_color_def->flash_flags & (1u << _hud_flash_reverse_colors_bit)))
            return real_argb_color_to_pixel32(&base_color);
        return real_argb_color_to_pixel32(&flash_color);
    }

    float cos_phase = (flash_cycle_phase / hud_color_def->flash_length) * 6.283f;
    double ease = -((cos(cos_phase) + 1.0) * 0.5 - 1.0);
    if (ease < 0.0)
        ease = 0.0;
    else if (ease > 1.0)
        ease = 1.0;
    float blend = __fsqrts((float)ease);

    real_argb_color result;
    if (hud_color_def->flash_flags & (1u << _hud_flash_reverse_colors_bit))
    {
        vectors_interpolate((const real_vector3d *)&flash_color, (const real_vector3d *)&base_color, blend,
                (real_vector3d *)&result);
        scalars_interpolate(flash_color.n[3], base_color.n[3], blend, &result.n[3]);
    }
    else
    {
        vectors_interpolate((const real_vector3d *)&base_color, (const real_vector3d *)&flash_color, blend,
                (real_vector3d *)&result);
        scalars_interpolate(base_color.n[3], flash_color.n[3], blend, &result.n[3]);
    }

    return real_argb_color_to_pixel32(&result);
}
