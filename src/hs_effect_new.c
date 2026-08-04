/* hs_effect_new @0x837F8540 — spawn an unattached effect at a scenario cutscene flag's location/orientation.
 * The flag's 2D euler facing (cutscene flag +0x30) is converted to a forward vector and the effect is placed
 * at the flag position (cutscene flag +0x24).
 *
 * DEVIATION: the trailing color / impulse-vector-field / flag arguments are uninitialized in the decompiler
 * (the binary leaves them as scratch registers); passed as null/0 here. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_cutscene_flag.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern int effect_new_unattached_from_markers(int definition_index, int owner_object_index, const real_vector3d *translational_velocity, int16_t marker_count, const char **marker_names, real_point3d *marker_points, real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field, uint8_t deterministic);

void hs_effect_new(int definition_index, int16_t flag_index)
{
    scenario_cutscene_flag *flag =
        &((scenario_cutscene_flag *)global_scenario->cutscene_flags.address)[flag_index];
    real_vector3d forward;
    vector3d_from_euler_angles2d(&forward, &flag->facing);

    effect_new_unattached_from_markers(definition_index, -1, global_zero_vector3d, 1, nullptr,
                                       &flag->position, &forward, 1.0f, 1.0f,
                                       nullptr, nullptr, 0);
}
