/* weapon_get_field_of_view @0x836D9320 — field of view for a weapon at a zoom level: the default FOV divided by
 * the zoom magnification, but only when that yields a sane angle (in (0.0314, 3.110) radians) and the weapon is
 * actually zoomed (magnification != 1.0); otherwise the default FOV is returned unchanged.
 *
 * DEVIATION: the decompiler split the single zoom_level parameter (r5) into an unused "zoom_level" plus a
 * phantom "a4"; the disassembly shows r5 is the zoom level passed to weapon_get_zoom_magnification. */

#include <stdint.h>

extern float weapon_get_zoom_magnification(int weapon_index, int16_t zoom_level);

float weapon_get_field_of_view(int weapon_index, float default_field_of_view, int16_t zoom_level)
{
    float zoom_magnification = weapon_get_zoom_magnification(weapon_index, zoom_level);
    float field_of_view = default_field_of_view / zoom_magnification;
    if ( zoom_magnification == 1.0f || field_of_view <= 0.031415928f || field_of_view >= 3.1101768f )
        field_of_view = default_field_of_view;
    return field_of_view;
}
