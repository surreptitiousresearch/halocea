/* physics_compute_biped_collision @0x837BF180 — per-tick biped/obstacle collision resolver. Tests the
 * biped's physics-pill base point against `instance`'s collision model; if already inside it, or if a
 * feature-based sphere test around the pill also reports a hit, pushes the biped away (accelerate) and
 * attempts to reposition it clear of the obstacle (collision_fix_pill). If the obstacle keeps colliding
 * (fix failed), or the biped hasn't recently exited the very same object and the relative motion between
 * biped and obstacle is non-negligible, applies "vehicle_collision_damage" to the biped (crediting the
 * obstacle's owner, or its driver if it has one) and "vehicle_killed_unit_damage_effect" to the obstacle
 * itself (scaled by the biped's own tag-defined blip_type, reused here as an index into the `scales`
 * global — not a bug, the same small enum field is reused as a lookup index).
 *
 * DEVIATION: the DB's own decompiler output for this function is unusable — 62 fabricated parameters
 * (a3..a62/a3..a63 depending on pull) versus the DB's real 2-arg prototype `(collision_model_instance
 * *instance, int biped_index)`. The huge stack frame (0xAD70 = ~44KB, mostly the `collision_feature_list`
 * local, which is itself ~44KB per its own DB layout) is what confused the decompiler into treating
 * stack slots as incoming arguments. Fully reconstructed from disasm_range(0x837BF180, 0x837BF180+1264).
 * Confirmed via cross-reference against types_members: the field read at unit-object+0x324 is exactly
 * _unit_datum.driver_object_index (relative 304, absolute 500+304=0x324); the field read at
 * global_game_globals->falling_damage.address+0x68/+0x58 are exactly game_globals_falling_damage's
 * vehicle_collision_damage.index (offset 0x5C+0xC) and vehicle_killed_unit_damage_effect.index
 * (offset 0x4C+0xC); and the field read at the biped's own tag+0x298 is exactly _unit_definition's
 * blip_type (relative 284, absolute 380+284=0x298) — all newly added to their respective headers by this
 * pass (unit_datum.h, biped_definition.h, game_globals_falling_damage.h). */

#include <stdint.h>
#include "headers/collision_model_instance.h"
#include "headers/collision_feature_list.h"
#include "headers/collision_plane.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/biped_definition.h"
#include "headers/damage_data.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_falling_damage.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"


#include "headers/location.h"
extern void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width);
extern uint8_t collision_model_test_point(const collision_model_instance *instance, const real_point3d *point);
extern void collision_features_new(collision_feature_list *features);
extern uint8_t collision_model_get_features_in_sphere(const collision_model_instance *instance, const real_point3d *center, float radius, float height, float width, collision_feature_list *features);
extern uint8_t collision_features_test_point(const collision_feature_list *features, const real_point3d *point, collision_plane *collision);
extern float normalize3d(real_vector3d *v);
extern float magnitude3d(const real_vector3d *v);
extern void biped_accelerate(int biped_index, real_vector3d *acceleration);
extern uint8_t collision_fix_pill(unsigned int flags, const real_point3d *old_position, float distance, float height, float width, int ignore_object_index, real_point3d *new_position);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);
extern int game_time_get(void);
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);


uint8_t physics_compute_biped_collision(collision_model_instance *instance, int biped_index)
{
    uint8_t collision_occurred = 0;

    real_point3d pill_base;
    float pill_height, pill_width;
    biped_get_physics_pill(biped_index, &pill_base, &pill_height, &pill_width);

    if (collision_model_test_point(instance, &pill_base))
    {
        collision_occurred = 1;
    }
    else
    {
        collision_feature_list features;
        collision_features_new(&features);

        real_point3d center;
        center.x = pill_base.x;
        center.y = pill_base.y;
        center.z = pill_base.z + pill_height * 0.5f;
        float radius = pill_height * 0.5f + pill_width;

        /* FAITHFUL: shrinks the query width by a small epsilon, floored at that same epsilon. */
        float shrunk_width = pill_width - 0.015625f;
        if (shrunk_width <= 0.015625f)
            shrunk_width = 0.015625f;

        collision_model_get_features_in_sphere(instance, &center, radius, pill_height, shrunk_width, &features);

        collision_plane collision;
        if (collision_features_test_point(&features, &pill_base, &collision))
            collision_occurred = 1;
    }

    if (!collision_occurred)
        return collision_occurred;

    unit_datum *obstacle_object =
        ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, instance->object_index)->datum);
    unit_datum *biped_object =
        ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);

    /* push direction: from the obstacle's bounding-sphere center toward the biped's, biased upward and
     * renormalized (a second normalize3d pass with +0.8 added to z) */
    real_vector3d push_vector;
    push_vector.n[0] = biped_object->object.bounding_sphere_center.n[0]
                      - obstacle_object->object.bounding_sphere_center.n[0];
    push_vector.n[1] = biped_object->object.bounding_sphere_center.n[1]
                      - obstacle_object->object.bounding_sphere_center.n[1];
    push_vector.n[2] = biped_object->object.bounding_sphere_center.n[2]
                      - obstacle_object->object.bounding_sphere_center.n[2];
    /* DEVIATION: obstacle_speed is |obstacle_object->translational_velocity| (fsqrts f30 of 0x68/0x6C/0x70
     * @0x837BF2C0-F8, computed BEFORE the normalize3d calls); both normalize3d returns are discarded. */
    real_vector3d obstacle_velocity = obstacle_object->object.translational_velocity;
    float obstacle_speed = magnitude3d(&obstacle_velocity);
    normalize3d(&push_vector);
    push_vector.n[2] = push_vector.n[2] + 0.8f;
    normalize3d(&push_vector);

    float speed_scale = obstacle_speed;
    if (speed_scale < 0.1f)
        speed_scale = 0.1f;

    real_vector3d acceleration;
    acceleration.n[0] = (obstacle_velocity.n[0] + push_vector.n[0] * speed_scale) * 0.5f;
    acceleration.n[1] = (obstacle_velocity.n[1] + push_vector.n[1] * speed_scale) * 0.5f;
    acceleration.n[2] = (obstacle_velocity.n[2] + push_vector.n[2] * speed_scale) * 0.5f;
    biped_accelerate(biped_index, &acceleration);

    real_point3d proposed_position;
    proposed_position.x = pill_base.x + acceleration.n[0] * 2.0f;
    proposed_position.y = pill_base.y + acceleration.n[1] * 2.0f;
    proposed_position.z = pill_base.z + acceleration.n[2] * 2.0f;

    uint8_t fix_succeeded;
    real_point3d new_position;
    if (collision_fix_pill(_collision_test_for_bipeds_living_flags, &proposed_position, pill_width * 2.0f, pill_height, pill_width, biped_index,
                            &new_position))
    {
        new_position.z -= pill_width;
        object_translate(biped_index, &new_position, 0);
        fix_succeeded = 1;
    }
    else
    {
        fix_succeeded = 0;
    }

    if (fix_succeeded)
    {
        /* both gates apply in sequence (not mutually exclusive): skip the crush-damage application if
         * the biped just exited this exact object (<1.5s / 90 ticks ago), AND separately skip it if
         * there's negligible relative motion between biped and obstacle */
        if (biped_object->unit.last_vehicle_index == instance->object_index
                && game_time_get() <= biped_object->unit.game_time_at_last_vehicle_exit + 90)
            return collision_occurred;

        if (obstacle_speed <= 0.06666667f)
        {
            real_vector3d relative_velocity;
            relative_velocity.n[0] = biped_object->object.translational_velocity.n[0]
                                    - obstacle_object->object.translational_velocity.n[0];
            relative_velocity.n[1] = biped_object->object.translational_velocity.n[1]
                                    - obstacle_object->object.translational_velocity.n[1];
            relative_velocity.n[2] = biped_object->object.translational_velocity.n[2]
                                    - obstacle_object->object.translational_velocity.n[2];
            float relative_speed_squared = relative_velocity.n[0] * relative_velocity.n[0]
                                          + relative_velocity.n[1] * relative_velocity.n[1]
                                          + relative_velocity.n[2] * relative_velocity.n[2];
            /* DEVIATION: 0.0011111112f was one ULP off; the image holds 0.0011111111f (1/900). */
            if (relative_speed_squared <= 0.0011111111f)
                return collision_occurred;
        }
    }

    game_globals_falling_damage *falling_damage_globals =
        (game_globals_falling_damage *)global_game_globals->falling_damage.address;

    if (falling_damage_globals->vehicle_collision_damage.index != -1)
    {
        /* the object credited/damaged is the obstacle's driver, if it has one; otherwise the obstacle
         * object itself */
        int damage_target_object_index = instance->object_index;
        object_datum *damage_target_object = (object_datum *)obstacle_object;
        if (obstacle_object->unit.driver_object_index != -1)
        {
            damage_target_object_index = obstacle_object->unit.driver_object_index;
            damage_target_object =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, damage_target_object_index)->datum;
        }

        damage_data damage;
        damage_data_new(&damage, falling_damage_globals->vehicle_collision_damage.index);
        damage.flags |= 1; /* runtime damage_data.flags bit0 — no DB enum for the runtime flag set */
        damage.scale = 1.0f;
        damage.owner_player_index = damage_target_object->object.owner_player_index;
        damage.owner_object_index = damage_target_object->object.owner_object_index != -1
                                   ? damage_target_object->object.owner_object_index
                                   : damage_target_object_index;
        damage.owner_team_index = damage_target_object->object.owner_team_index;
        damage.origin = biped_object->object.bounding_sphere_center;
        damage.epicenter = obstacle_object->object.bounding_sphere_center;
        /* DEVIATION: this is the final `acceleration` vector (not `push_vector`) — disasm confirms the
         * stack slot backing push_vector is overwritten with the acceleration computation before this
         * read, and is normalized fresh here for the damage direction. */
        damage.direction = acceleration;
        normalize3d(&damage.direction);

        object_cause_damage(&damage, biped_index, -1, -1, -1, 0);
    }

    if (falling_damage_globals->vehicle_killed_unit_damage_effect.index != -1)
    {
        const biped_definition *biped_def =
            TAG_GET(const biped_definition, biped_object->definition_index);

        damage_data damage;
        damage_data_new(&damage, falling_damage_globals->vehicle_killed_unit_damage_effect.index);
        damage.direction.n[0] = -acceleration.n[0];
        damage.direction.n[1] = -acceleration.n[1];
        damage.direction.n[2] = -acceleration.n[2];
        damage.origin = biped_object->object.bounding_sphere_center;
        damage.scale = scales[biped_def->unit.blip_type];

        object_cause_damage(&damage, instance->object_index, -1, -1, -1, 0);
    }

    return collision_occurred;
}
