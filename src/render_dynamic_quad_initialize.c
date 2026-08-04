/* render_dynamic_quad_initialize @0x8382C97C — resets the default dynamic-quad animation state: all 4
 * animation colors default to white, all 4 animation values default to 0.
 *
 * DEVIATION: the decompiler renders the 4th color's copy as split LODWORD/float-register punning
 * artifacts (element [3] copied field-by-field instead of as one struct assignment, unlike [0]-[2]); this
 * is a plain 4-element loop-equivalent struct copy. */

#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"


int render_dynamic_quad_initialize(void)
{
    global_default_animation_colors[0] = *global_real_rgb_white;
    global_default_animation_colors[1] = *global_real_rgb_white;
    global_default_animation_colors[2] = *global_real_rgb_white;
    global_default_animation_colors[3] = *global_real_rgb_white;

    global_default_animation_values[0] = 0.0f;
    global_default_animation_values[1] = 0.0f;
    global_default_animation_values[2] = 0.0f;
    global_default_animation_values[3] = 0.0f;

    return *(int *)&global_real_rgb_white->n[1];
}
