/* scenario_get_atmospheric_fog @0x83704330 — produce the atmospheric fog for a render window, easing a
 * per-local-player interpolator toward the active sky's fog so fog changes are smoothed as the camera moves.
 * The sky definition is resolved (explicit sky_index, or the scenario's first sky reference when -1). The
 * indoor or outdoor fog block of that sky is the easing target; each fog parameter is moved toward the target
 * by at most the distance the camera has travelled this frame (distance*0.05 for density/color/screen
 * intensity), unless the camera jumped far, the state is invalid, or either far plane is zero — in which case
 * the state snaps directly. The eased state is then clamped (far >= near, screen intensity in [0,1]) and written
 * into the render fog block. With no local player a throwaway stack interpolator is used (always snaps).
 *
 * DEVIATION: the decompiler dropped the fog-source pointer assignment for the sky_index == -1 path (it left the
 * pointer used at the interpolation site uninitialised). The disassembly shows it is always the sky's indoor
 * fog block (sky+0x78); the outdoor block (sky+0x58) is used only for an explicit sky_index. Reconstructed
 * accordingly. The hardware fcfid float conversions are restored as plain casts. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/tag_reference.h"
#include "headers/scenario_globals.h"
#include "headers/sky.h"
#include "headers/sky_atmospheric_fog.h"
#include "headers/render_fog.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
extern float __fsqrts(float);

extern sky *scenario_get_sky(int16_t sky_index);

static float ease_toward(float current, float target, float maximum_step)
{
    float delta = target - current;
    if (delta < -maximum_step)
        delta = -maximum_step;
    else if (delta > maximum_step)
        delta = maximum_step;
    return current + delta;
}

void scenario_get_atmospheric_fog(int16_t local_player_index, int16_t sky_index, real_point3d *camera_point, render_fog *render_fog)
{
    int sky_definition_index = -1;
    if (sky_index == -1)
    {
        if (global_scenario->sky_references.count > 0)
            sky_definition_index = ((tag_reference *)global_scenario->sky_references.address)[0].index;
    }
    else if (sky_index >= 0 && sky_index < global_scenario->sky_references.count)
    {
        sky_definition_index = ((tag_reference *)global_scenario->sky_references.address)[sky_index].index;
    }

    sky *sky_definition = nullptr;
    if (sky_definition_index != -1)
        sky_definition = TAG_GET(sky, sky_definition_index);

    scenario_fog_interpolator scratch;
    scenario_fog_interpolator *fog_state =
        (local_player_index == -1) ? &scratch : &scenario_globals->local_players[local_player_index];

    if (sky_definition)
    {
        sky_atmospheric_fog *fog_source;
        float screen_intensity_target;
        if (sky_index == -1)
        {
            fog_source = &sky_definition->indoor_fog; /* indoor fog @0x78 (decompiler dropped this) */
            screen_intensity_target = (scenario_get_sky(0)->indoor_fog_plane.index != -1) ? 1.0f : 0.0f;
        }
        else
        {
            fog_source = &sky_definition->outdoor_fog; /* outdoor fog @0x58 */
            screen_intensity_target = 0.0f;
        }

        float dx = camera_point->n[0] - fog_state->point.n[0];
        float dy = camera_point->n[1] - fog_state->point.n[1];
        float dz = camera_point->n[2] - fog_state->point.n[2];
        float distance_moved = __fsqrts(dx * dx + (dz * dz + dy * dy));

        if (local_player_index == -1 || distance_moved >= 15.0f || !fog_state->valid
            || fog_source->z_far == 0.0f || fog_state->atmospheric_fog_z_far == 0.0f)
        {
            fog_state->atmospheric_fog_z_near = fog_source->z_near;
            fog_state->atmospheric_fog_z_far = fog_source->z_far;
            fog_state->atmospheric_fog_maximum_density = fog_source->maximum_density;
            fog_state->atmospheric_fog_color.red = fog_source->color.red;
            fog_state->atmospheric_fog_color.green = fog_source->color.green;
            fog_state->atmospheric_fog_color.blue = fog_source->color.blue;
            fog_state->screen_external_intensity = screen_intensity_target;
            fog_state->valid = 1;
        }
        else
        {
            float color_step = distance_moved * 0.050000001f;
            fog_state->atmospheric_fog_z_near =
                ease_toward(fog_state->atmospheric_fog_z_near, fog_source->z_near, distance_moved);
            fog_state->atmospheric_fog_z_far =
                ease_toward(fog_state->atmospheric_fog_z_far, fog_source->z_far, distance_moved);
            fog_state->atmospheric_fog_maximum_density =
                ease_toward(fog_state->atmospheric_fog_maximum_density, fog_source->maximum_density, color_step);
            fog_state->atmospheric_fog_color.red =
                ease_toward(fog_state->atmospheric_fog_color.red, fog_source->color.red, color_step);
            fog_state->atmospheric_fog_color.green =
                ease_toward(fog_state->atmospheric_fog_color.green, fog_source->color.green, color_step);
            fog_state->atmospheric_fog_color.blue =
                ease_toward(fog_state->atmospheric_fog_color.blue, fog_source->color.blue, color_step);
            fog_state->screen_external_intensity =
                ease_toward(fog_state->screen_external_intensity, screen_intensity_target, color_step);
        }

        fog_state->point = *camera_point;
    }

    render_fog->atmospheric_color = fog_state->atmospheric_fog_color;
    render_fog->atmospheric_maximum_density = fog_state->atmospheric_fog_maximum_density;
    render_fog->atmospheric_minimum_distance = fog_state->atmospheric_fog_z_near;

    float z_far = fog_state->atmospheric_fog_z_far;
    if (z_far != 0.0f && z_far <= fog_state->atmospheric_fog_z_near + 0.000099999997f)
        z_far = fog_state->atmospheric_fog_z_near + 0.000099999997f;
    render_fog->atmospheric_maximum_distance = z_far;

    float screen_intensity = fog_state->screen_external_intensity;
    if (screen_intensity < 0.0f)
        render_fog->screen_external_intensity = 0.0f;
    else if (screen_intensity > 1.0f)
        render_fog->screen_external_intensity = 1.0f;
    else
        render_fog->screen_external_intensity = fog_state->screen_external_intensity;
}
