/* material_effect_new_from_point @0x836EAAD0 — spawns a material effect (e.g. a footstep/impact decal)
 * by casting a short ray straight down (0.3 world units along global_down3d) from just above the given
 * point. If it hits a surface, the effect is created there using that surface's material type, or the
 * "water" material (28) when the impact location is underwater. No-op if effect_index is out of range. */

#include <stdint.h>
#include "headers/collision_result.h"
#include "headers/collision_test_flags.h"
#include "headers/material_type.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);
extern void material_effect_new(int effects_definition_index, int16_t effect_index, int16_t material_index, const real_point3d *origin, const real_vector3d *normal, const location *location, float scale, uint8_t is_player);

void material_effect_new_from_point(int effects_definition_index, int16_t effect_index, const real_point3d *origin, float scale, uint8_t is_player)
{
    /* the effects definition's first dword is the effect count */
    if (effect_index >= *TAG_GET(int, effects_definition_index))
        return;

    real_point3d ray_origin;
    ray_origin.n[0] = origin->n[0];
    ray_origin.n[1] = origin->n[1];
    ray_origin.n[2] = origin->n[2] + 0.15000001f;

    real_vector3d ray_delta;
    ray_delta.n[0] = global_down3d->n[0] * 0.30000001f;
    ray_delta.n[1] = global_down3d->n[1] * 0.30000001f;
    ray_delta.n[2] = global_down3d->n[2] * 0.30000001f;

    collision_result collision;
    if (collision_test_vector(_collision_test_for_bipeds_passthrough_living_flags /* 0xC2A0 */, &ray_origin, &ray_delta, -1, &collision))
    {
        int16_t material_type = _material_water;
        if (!scenario_location_underwater(&collision.location, &collision.point, nullptr))
            material_type = collision.material_type;

        material_effect_new(effects_definition_index, effect_index, material_type, &collision.point,
                            &collision.plane.n, &collision.location, scale, is_player);
    }
}
