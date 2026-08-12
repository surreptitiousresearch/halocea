/* projectile_detonate @0x8375A0B0 — detonate a projectile: spawn its detonation (or grouped super-detonation)
 * effect at its position, apply detonation damage against the object it is attached to, spawn the hit
 * material's response effect, and notify the AI of the noise.
 *
 * Stages:
 *  A. Grouped super detonation: if the definition has flag 0x8, this projectile is still attached to a parent
 *     and not yet grouped (projectile flag 0x40), count the parent's attached same-type ungrouped
 *     projectiles. When the parent is a biped (type 0), is AI-controlled or a game engine is running, and
 *     more than 6 are stuck: the excess detonate immediately (timers zeroed) and the rest get randomized
 *     staggered timers + flag 0x40; the detonation switches to the super_detonation_effect, and this
 *     projectile is detached at the parent's origin (forced back inside the BSP).
 *  B. On a first collision, tell this projectile's contrail attachment about the impact.
 *  C. Spawn the detonation effect from two markers: "" (object origin, facing forward) and "gravity"
 *     (object origin, facing global_down3d).
 *  D. If still attached, apply detonation_damage to the parent object.
 *  E. Spawn the hit material's response detonation effect (default response when out of range).
 *  F. Report the detonation noise to the AI.
 *
 * DEVIATIONS (disasm-verified, 0x8375A0B0-0x8375A534):
 *  - Both effect_new_unattached_from_markers calls pass color = nullptr, impulse_field = nullptr,
 *    deterministic = 1. These are stack-slot args (outgoing 8-byte arg slots 10..12 at r1+0x58/0x60/0x68);
 *    the decompiler rendered stale register/stack values (v23/v24, v30-v33) for them.
 *  - The contrail dt multiplies by the named global seconds_per_tick_1, which the decompiler inlined as the
 *    literal 0.033333335.
 *  - time_left is a float widened to double by the FPR-shadow ABI; the DB prototype (float) is used.
 *  - Raw byte offsets are expressed through the DB-verified projectile_datum / projectile_definition /
 *    unit_datum layouts. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_definition.h"
#include "headers/unit_datum.h"
#include "headers/projectile_material_response_definition.h"
#include "headers/damage_data.h"
#include "headers/effect_vector_field.h"
#include "headers/real_rgb_color.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/projectile_definition_flags.h"
#include "headers/projectile_datum_flags.h"
#include "headers/damage_data_flags.h"
#include "headers/ai_spatial_effect_type.h"
#include "headers/blam_data_globals.h"

#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

extern uint8_t game_engine_running(void);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void object_get_orientation(int object_index, real_vector3d *forward, real_vector3d *up);
extern void object_detach(int child_object_index);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);
extern uint8_t object_force_inside_bsp(int object_index, int ignore_object_index, const real_point3d *known_good_point);
extern void object_compute_node_matrices(int object_index);
extern void object_compute_node_matrices_recursive(int object_index);
extern void contrail_owner_collision(int contrail_index, uint8_t object_dying, float dt);
extern int effect_new_unattached_from_markers(int definition_index, int owner_object_index, const real_vector3d *translational_velocity, int16_t marker_count, const char **marker_names, real_point3d *marker_points, real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field, uint8_t deterministic);
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);
extern void ai_handle_spatial_effect(int object_index, const real_point3d *position, int16_t effect_type, int16_t volume, int16_t count);

void projectile_detonate(int projectile_index, uint8_t first_collision, float time_left)
{
    projectile_datum *projectile = ((projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum);
    projectile_definition *definition =
            TAG_GET(projectile_definition, projectile->definition_index);

    const char *marker_names[2];
    marker_names[0] = empty_string;
    marker_names[1] = "gravity";

    int effect_definition_index = definition->projectile.detonation_effect.index;

    /* stage A: grouped super detonation of same-type projectiles stuck to the same parent */
    if ( (definition->projectile.flags & (1u << _projectile_super_combining_explosion_bit)) != 0 && (projectile->projectile.flags & (1u << _projectile_already_super_exploded_bit)) == 0
            && projectile->object.parent_object_index != -1 )
    {
        int16_t sibling_count = 0;
        unit_datum *parent = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile->object.parent_object_index)->datum;
        projectile_datum *child;
        for ( int child_index = parent->object.first_child_object_index; child_index != -1;
                child_index = child->object.next_object_index )
        {
            child = (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_index)->datum;
            if ( child->definition_index == projectile->definition_index
                    && (child->projectile.flags & (1u << _projectile_already_super_exploded_bit)) == 0 )
                ++sibling_count;
        }
        if ( !parent->object.type
                && (parent->unit.player_index == -1 || game_engine_running())
                && sibling_count > 6 )
        {
            projectile_datum *sibling;
            for ( int child_index = parent->object.first_child_object_index; child_index != -1;
                    child_index = sibling->object.next_object_index )
            {
                sibling = ((projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_index)->datum);
                if ( sibling->definition_index == projectile->definition_index )
                {
                    unsigned int sibling_flags = sibling->projectile.flags;
                    if ( (sibling_flags & (1u << _projectile_already_super_exploded_bit)) == 0 )
                    {
                        if ( sibling_count > 6 )
                        {
                            /* excess sibling: detonate immediately */
                            sibling->projectile.detonation_timer = 0.0;
                            sibling->projectile.arming_time = 0.0;
                        }
                        else
                        {
                            sibling->projectile.flags = sibling_flags | (1u << _projectile_already_super_exploded_bit);
                            unsigned int *seed = get_global_random_seed_address();
                            sibling->projectile.detonation_timer =
                                    sibling->projectile.detonation_timer * real_seed_random(seed);
                            unsigned int *arming_seed = get_global_random_seed_address();
                            sibling->projectile.arming_time =
                                    sibling->projectile.arming_time * real_seed_random(arming_seed);
                        }
                        --sibling_count;
                    }
                }
            }

            effect_definition_index = definition->projectile.super_detonation_effect.index;

            real_point3d parent_origin;
            real_point3d known_good_point;
            object_get_origin(projectile->object.parent_object_index, &parent_origin);
            object_detach(projectile_index);
            known_good_point.n[0] = projectile->object.position.n[0];
            known_good_point.n[1] = projectile->object.position.n[1];
            known_good_point.n[2] = projectile->object.position.n[2];
            object_translate(projectile_index, &parent_origin, nullptr);
            object_force_inside_bsp(projectile_index, -1, &known_good_point);
            object_compute_node_matrices_recursive(projectile_index);
        }
    }

    /* stage B: tell the contrail attachment about the first collision */
    if ( first_collision )
    {
        int tracer_slot = projectile->projectile.tracer_attachment_index_index;
        if ( tracer_slot != -1 && projectile->object.attachment_indices[tracer_slot] != -1 )
        {
            object_compute_node_matrices(projectile_index);
            contrail_owner_collision(
                    projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index_index],
                    0,
                    ((float)1.0 - time_left) * seconds_per_tick_1);
        }
    }

    /* stage C: detonation effect from the "" (forward) and "gravity" (down) markers */
    real_point3d marker_points[2];
    real_vector3d marker_forwards[2];
    real_vector3d orientation_up;
    object_get_origin(projectile_index, marker_points);
    object_get_orientation(projectile_index, marker_forwards, &orientation_up);
    marker_points[1] = marker_points[0];
    marker_forwards[1].n[0] = global_down3d->n[0];
    marker_forwards[1].n[1] = global_down3d->n[1];
    marker_forwards[1].n[2] = global_down3d->n[2];
    effect_new_unattached_from_markers(effect_definition_index, projectile->object.owner_object_index,
            nullptr, 2, marker_names, marker_points, marker_forwards, 0.0, 0.0, nullptr, nullptr, 1);

    /* stage D: detonation damage against the object we are attached to */
    if ( projectile->object.parent_object_index != -1 )
    {
        int damage_definition_index = definition->projectile.detonation_damage.index;
        if ( damage_definition_index != -1 )
        {
            damage_data damage;
            damage_data_new(&damage, damage_definition_index);
            damage.flags |= (1u << _damage_from_weapon_bit);
            object_get_orientation(projectile_index, &damage.direction, nullptr);
            object_get_origin(projectile_index, &damage.origin);
            damage.owner_object_index = projectile->object.owner_object_index;
            damage.owner_player_index = projectile->object.owner_player_index;
            damage.owner_team_index = projectile->object.owner_team_index;
            damage.epicenter = damage.origin;
            object_cause_damage(&damage, projectile->object.parent_object_index, -1, -1, -1, nullptr);
        }
    }

    /* stage E: hit material response detonation effect */
    int16_t hit_material_type = projectile->projectile.hit_material_type;
    if ( hit_material_type != -1 )
    {
        projectile_material_response_definition *response;
        if ( hit_material_type < 0
                || hit_material_type >= definition->projectile.material_responses.count )
            response = &default_projectile_material_response;
        else
            response = (projectile_material_response_definition *)
                    definition->projectile.material_responses.address + hit_material_type;
        effect_new_unattached_from_markers(response->detonation_effect.index,
                projectile->object.owner_object_index, nullptr, 2, marker_names, marker_points,
                marker_forwards, 0.0, 0.0, nullptr, nullptr, 1);
    }

    /* stage F: report the detonation noise to the AI */
    ai_handle_spatial_effect(projectile_index, marker_points, _ai_spatial_effect_weapon_detonation, definition->projectile.detonation_noise, 1);
}
