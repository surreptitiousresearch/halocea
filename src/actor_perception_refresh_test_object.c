/* actor_perception_refresh_test_object @0x837D8DA8 — walk an object sub-tree (following each object's sibling
 * chain and recursing into its first-child chain) and, for every object not yet visited this perception pass
 * (guarded by the global_object_marker epoch), feed it into the actor's perception. What happens depends on the
 * object type:
 *   - type 1 (vehicle): if it has no driver (object+804 == -1), assess it as a potential vehicle danger.
 *   - type 5 (grenade / thrown danger): if its definition has a positive danger radius and it is either
 *     un-owned or has the "armed" flag (object+556 bit 5), record it as the actor's tracked grenade danger
 *     (position, velocity, radius, timer) when it is closer than an existing danger of equal/greater priority,
 *     tagging it friendly/enemy/own by its thrower's team.
 *   - type 0 (unit / prop): resolve the unit (dereferencing a swarm root when present), and if it is a distinct
 *     unit assess suicide danger and offer it to actor_perception_desire_prop; a desired prop is added to the
 *     enemy or friend prop environment as a new unacknowledged prop.
 *
 * One of ~40 actor_perception_* siblings. 2026-07-13: fully typed (unit_datum/projectile_datum).
 *
 * DEVIATIONS (disasm-verified 0x837D8DDC-0x837D9048):
 *  - the actor_index param is a plain int actor index (the decompiler typed it actor_position_data*).
 *  - the two calls the decompiler mis-argued are corrected: unit_from_swarm receives (&sense_position,
 *    swarm_root, -1, 1, mark_units) with sense_position punned into the int arg1 (0x837D8EA8: r3=&var_F0);
 *    assess_suicide_danger receives (enemy, currently_perceived=0) not the decompiler's (v18&4, v18)
 *    (0x837D8FE4: r7=is_enemy, r8=0).
 *  - dead_ticks is reconstructed from disasm (0x837D8F6C-0x837D8F88): game_time_get()'s result, which the
 *    decompiler dropped, is (current game time - object's acknowledge time at object+1052).
 *  - required_ticks and the optional_reference scratch byte are both 0 (r20 is a constant-zero register,
 *    0x837D8DF4 li r20,0).
 *  - mark_units is genuinely uninitialized in the shipped code (Hex-Rays models it uninitialized too);
 *    reproduced as an uninitialized local. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/projectile_datum_flags.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_definition.h"
#include "headers/unit_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_position_data.h"
#include "headers/actor_prop_environment.h"
#include "headers/real_point3d.h"
#include <string.h>
#include "headers/actor_danger_zone_type.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"


extern float __fsqrts(float x);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern int game_time_get(void);
extern int actor_perception_assess_vehicle_danger(int actor_index, int vehicle_index, actor_position_data *sense_position, uint8_t currently_perceived);
extern void actor_perception_find_sense_position(uint16_t actor_index, const real_point3d *position, int prop_index, actor_position_data *sense_position);
extern int actor_perception_unit_from_swarm(actor_position_data *sense_position, int actor_index, int preferred_unit_index, uint8_t mark_units);
extern int actor_perception_assess_suicide_danger(uint16_t actor_index, int unit_index, float suicide_radius, float current_distance, uint8_t enemy, uint8_t currently_perceived);
extern uint8_t actor_perception_desire_prop(uint16_t actor_index, int16_t desired_target_state, int unit_index, int unit_actor_index, uint8_t in_use, uint8_t player, uint8_t enemy, uint8_t dead, int16_t dead_ticks, float suicide_radius, float distance_squared, int16_t required_ticks, uint8_t *optional_reference);
extern int prop_new_unacknowledged(int actor_index, int unit_index, uint8_t enemy);
extern void prop_position_refresh(int actor_index, int prop_index, actor_position_data *sense_position, uint8_t forced_position_update, uint8_t full_status_update);

void actor_perception_refresh_test_object(int actor_index, int object_index, actor_prop_environment *enemies,
        actor_prop_environment *friends)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int current_object = object_index;

    while ( current_object != -1 )
    {
        unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, current_object)->datum);

        if ( object->object.magic_number != global_object_marker )
        {
            object->object.magic_number = global_object_marker;
            __int16 object_type = object->object.type;
            if ( object_type )
            {
                if ( object_type == object_type_vehicle )
                {
                    if ( object->unit.driver_object_index == -1 )
                        actor_perception_assess_vehicle_danger(actor_index, current_object, nullptr, 0);
                }
                else if ( object_type == object_type_projectile )
                {
                    projectile_datum *grenade = (projectile_datum *)object;
                    projectile_definition *definition = TAG_GET(projectile_definition, grenade->definition_index);
                    if ( definition->projectile.danger_radius > 0.0f )
                    {
                        char is_dangerous = 0;
                        if ( object->object.parent_object_index == -1 || (grenade->projectile.flags & (1u << _projectile_counting_down_bit)) != 0 )
                            is_dangerous = 1;
                        if ( is_dangerous )
                        {
                            real_point3d grenade_origin;
                            object_get_origin(current_object, &grenade_origin);
                            actor_position_data sense_position;
                            actor_perception_find_sense_position(actor_index, &grenade_origin, -1, &sense_position);
                            float distance = __fsqrts((((grenade_origin.n[0]
                                                    - sense_position.body_position.n[0])
                                            * (grenade_origin.n[0] - sense_position.body_position.n[0]))
                                    + (((grenade_origin.n[2] - sense_position.body_position.n[2])
                                                    * (grenade_origin.n[2] - sense_position.body_position.n[2]))
                                            + ((grenade_origin.n[1] - sense_position.body_position.n[1])
                                                    * (grenade_origin.n[1] - sense_position.body_position.n[1])))));
                            if ( distance < (definition->projectile.danger_radius + 10.0f) )
                            {
                                __int16 danger_state = actor->danger_zone.danger_type;
                                if ( danger_state < actor_danger_zone_projectile
                                        || danger_state == actor_danger_zone_projectile && actor->danger_zone.object_index != current_object
                                                && distance < actor->danger_zone.current_distance_from_actor )
                                {
                                    memset(&actor->danger_zone, 0, 0x6C);
                                    actor->danger_zone.object_index = current_object;
                                    actor->danger_zone.danger_type = actor_danger_zone_projectile;
                                    int thrower_prop = -1;
                                    actor->danger_zone.danger_radius = definition->projectile.danger_radius;
                                    actor->danger_zone.initial_position.n[0] = grenade_origin.n[0];
                                    actor->danger_zone.initial_position.n[1] = grenade_origin.n[1];
                                    actor->danger_zone.initial_position.n[2] = grenade_origin.n[2];
                                    actor->danger_zone.initial_velocity.n[0] = object->object.translational_velocity.n[0];
                                    actor->danger_zone.initial_velocity.n[1] = object->object.translational_velocity.n[1];
                                    actor->danger_zone.initial_velocity.n[2] = object->object.translational_velocity.n[2];
                                    actor->danger_zone.acknowledgement_timer = 30;
                                    actor->danger_zone.currently_perceived = 0;
                                    actor->danger_zone.hostility = 0;
                                    int thrower = object->object.owner_object_index;
                                    if ( thrower != -1 )
                                    {
                                        unit_datum *thrower_object =
                                                object_try_and_get_and_verify_type(thrower, object_mask_all);
                                        if ( thrower_object )
                                        {
                                            if ( ((1 << thrower_object->object.type) & object_mask_unit) != 0 )
                                            {
                                                int current_target = actor->meta.unit_index;
                                                thrower_prop = object->object.owner_object_index;
                                                if ( current_target == -1 || thrower_prop != current_target )
                                                {
                                                    if ( !game_team_is_enemy(actor->meta.team_index,
                                                                object->object.owner_team_index) )
                                                        actor->danger_zone.hostility = 1;
                                                }
                                                else
                                                {
                                                    actor->danger_zone.hostility = 2;
                                                }
                                            }
                                        }
                                    }
                                    actor->danger_zone.owner_unit_index = thrower_prop;
                                }
                            }
                        }
                    }
                }
                goto next_sibling;
            }

            /* object_type == object_type_biped: unit / prop perception */
            unit_datum *unit = object;
            real_point3d unit_origin;
            object_get_origin(current_object, &unit_origin);
            actor_position_data sense_position;
            actor_perception_find_sense_position(actor_index, &unit_origin, -1, &sense_position);

            int swarm_root;
            if ( object->unit.swarm_actor_index == -1 )
            {
                swarm_root = object->unit.actor_index;
            }
            else
            {
                swarm_root = object->unit.swarm_actor_index;
                /* attested 4-param: phantom 5th arg (was an uninitialized `mark_units`) dropped */
                current_object = actor_perception_unit_from_swarm(&sense_position, swarm_root, -1, 1);
                if ( current_object == -1 )
                    goto next_sibling;
                unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, current_object)->datum);
                object_get_origin(current_object, &unit_origin);
            }

            if ( current_object != -1 && swarm_root != actor_index )
            {
                int player = unit->unit.player_index != -1;
                unit_definition *tag = TAG_GET(unit_definition, unit->definition_index);
                unsigned __int8 enemy = game_team_is_enemy(actor->meta.team_index, unit->object.owner_team_index);

                unsigned __int8 dead = 0;
                if ( (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 && unit->unit.feign_death_timer == 0 )
                    dead = 1;

                float suicide_radius = tag->unit.ai_danger_radius;
                float distance_squared = (((unit_origin.n[1] - sense_position.body_position.n[1])
                                * (unit_origin.n[1] - sense_position.body_position.n[1]))
                        + (((unit_origin.n[2] - sense_position.body_position.n[2])
                                        * (unit_origin.n[2] - sense_position.body_position.n[2]))
                                + ((unit_origin.n[0] - sense_position.body_position.n[0])
                                        * (unit_origin.n[0] - sense_position.body_position.n[0]))));

                if ( suicide_radius > 0.0f && (dead || unit->unit.animation.state == _unit_state_melee_attack) )
                    actor_perception_assess_suicide_danger(actor_index, current_object, suicide_radius,
                            __fsqrts(distance_squared), enemy, 0);

                __int16 dead_ticks;
                if ( !dead )
                {
                    dead_ticks = 0;
                }
                else
                {
                    int acknowledge_time = unit->unit.time_of_death;
                    if ( acknowledge_time == -1 )
                        dead_ticks = 0x7FFF;
                    else
                        dead_ticks = (__int16)(game_time_get() - acknowledge_time);
                }

                unsigned __int8 optional_reference = 0;
                if ( actor_perception_desire_prop(actor_index, 0, current_object, swarm_root, 0, player, enemy,
                        dead, dead_ticks, suicide_radius, distance_squared, 0, &optional_reference) )
                {
                    actor_prop_environment *environment = enemy ? enemies : friends;
                    int prop_index = prop_new_unacknowledged(actor_index, current_object, enemy);
                    if ( prop_index != -1 )
                    {
                        prop_position_refresh(actor_index, prop_index, &sense_position, 0, 0);
                        if ( !dead )
                            ++environment->existing_count;
                    }
                }
            }
        }

next_sibling:
        {
            int first_child = object->object.first_child_object_index;
            if ( first_child != -1 )
                actor_perception_refresh_test_object(actor_index, first_child, enemies, friends);
            current_object = object->object.next_object_index;
        }
    }
}
