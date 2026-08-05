/* unit_cause_player_melee_damage @0x836D02B0 — the player's melee attack. It fires a 5x5 fan of collision rays
 * forward from the unit's "head" marker (each ray = forward*0.8 offset by +-0.2 across the two axes
 * perpendicular to forward) to find what is struck. Among object hits it prefers bipeds, then the nearest; it
 * also records the first surface/breakable-surface hit. The damage effect is the equipped weapon's melee
 * override (if any) or the unit's default melee effect. A main damage event is delivered to the struck object
 * (or breakable surface), with damage scale derived from the unit's swing speed (and boosted to 1.5 for a biped
 * lunge). Vehicles struck are shoved (vehicle_accelerate); machines are nudged open. HCEX rule hooks can drop a
 * grenade on an enemy melee and recharge the attacker's shields. Finally the melee sound plays and, if the
 * weapon defines a recoil effect, a second self-damage event applies the recoil.
 *
 * DEVIATION: the decompiler aliases the damage_data buffer over the head-marker object_marker slot and
 * misnames its fields (matrix/node_matrix rows). Both damage_data fills were re-derived from the store
 * instructions; field assignments below use the database damage_data layout. The biped-preference hit test is
 * the decompiler's nested boolean simplified to: replace when the new hit is a biped and either the current
 * best is not a biped or the new hit is nearer. */

#include <stdint.h>
#include <math.h>
#include "headers/data_array.h"
#include "headers/collision_test_flags.h"
#include "headers/collision_surface_flags.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/biped_datum.h"
#include "headers/weapon_datum.h"
#include "headers/unit_definition.h"
#include "headers/weapon_definition.h"
#include "headers/game_globals_grenade.h"
#include "headers/game_globals_player_information.h"
#include "headers/player_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_tag.h"
#include "headers/damage_data.h"
#include "headers/damage_data_flags.h"
#include "headers/object_marker.h"
#include "headers/object_placement_data.h"
#include "headers/collision_result.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/networked_datum_role.h"
#include "headers/object_type.h"
#include "headers/collision_result_type.h"
#include "headers/unit_melee_attack_state.h"
#include "headers/blam_data_globals.h"



extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void vehicle_accelerate(int vehicle_index, const real_vector3d *acceleration);
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void breakable_surface_damage(int16_t breakable_surface_index, const damage_data *damage_data, int seed_surface_index);
extern void machine_try_to_open_with_damage(int machine_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);
extern void unit_melee_sound(int unit_index, int damage_definition_index, int16_t material_type);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void object_disconnect_from_map(int object_index);
extern void unit_drop_item(int unit_index, int item_index);
extern int player_index_from_unit_index(int unit_index);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);

void unit_cause_player_melee_damage(int unit_index)
{
    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *unit_definition_tag = TAG_GET(unit_definition, unit_data->definition_index);

    int best_object = -1;
    int16_t material_type = -1;
    int16_t breakable_surface_index = -1;
    int recoil_effect = -1;
    int surface_index = -1;
    int16_t best_object_type = 0;
    float best_t = 0.0f;

    float *forward = unit_data->unit.aiming_vector.n;

    object_marker head_marker;
    object_get_marker_by_name(unit_index, "head", &head_marker, 1);
    real_point3d position;
    position.n[0] = head_marker.matrix.n[3][0];
    position.n[1] = head_marker.matrix.n[3][1];
    position.n[2] = head_marker.matrix.n[3][2];

    /* two axes perpendicular to forward: perp (perpendicular3d) and third = forward x perp */
    real_vector3d perp;
    perpendicular3d((const real_vector3d *)forward, &perp);
    float perp_mag = sqrtf((perp.n[0] * perp.n[0]) + ((perp.n[1] * perp.n[1]) + (perp.n[2] * perp.n[2])));
    if ( fabsf(perp_mag) >= 0.000099999997f )
    {
        float inverse = 1.0f / perp_mag;
        perp.n[0] *= inverse;
        perp.n[1] *= inverse;
        perp.n[2] *= inverse;
    }
    float cross_x = (forward[2] * perp.n[0]) - (forward[0] * perp.n[2]);   /* (forward x perp).y in source order */
    float cross_y = (forward[0] * perp.n[1]) - (forward[1] * perp.n[0]);   /* .z */
    float cross_z = (forward[1] * perp.n[2]) - (forward[2] * perp.n[1]);   /* .x */

    for ( int row = -2; row <= 2; row++ )
    {
        for ( int col = -2; col <= 2; col++ )
        {
            real_vector3d ray;
            ray.n[0] = (((row * perp.n[0]) + (col * cross_z)) * 0.1f) + (forward[0] * 0.80000001f);
            ray.n[1] = (((col * cross_x) + (row * perp.n[1])) * 0.1f) + (forward[1] * 0.80000001f);
            ray.n[2] = (((col * cross_y) + (row * perp.n[2])) * 0.1f) + (forward[2] * 0.80000001f);

            collision_result collision;
            if ( !collision_test_vector(_collision_test_for_projectiles_flags /* 0x1000E9 */, &position, &ray, unit_index, &collision) )
                continue;

            if ( collision.type == collision_result_structure )   /* surface */
            {
                if ( best_object == -1 )
                {
                    material_type = collision.material_type;
                    if ( (collision.flags & (1u << _collision_surface_breakable_bit)) != 0 )   /* struck a breakable surface */
                    {
                        breakable_surface_index = collision.breakable_surface_index;
                        surface_index = collision.surface_index;
                    }
                }
            }
            else if ( collision.type == collision_result_object )   /* object */
            {
                int object_index = collision.object_index;
                unit_datum *hit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                    object_index)->datum;
                if ( hit_data->object.type != object_type_weapon && hit_data->object.parent_object_index != -1 )
                {
                    object_index = hit_data->object.parent_object_index;
                    hit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                        object_index)->datum;
                }

                int16_t hit_type = hit_data->object.type;
                if ( best_object == -1
                  || (hit_type == object_type_biped
                      && (best_object_type != object_type_biped || collision.t < best_t)) )
                {
                    best_object_type = hit_type;
                    best_object = object_index;
                    material_type = collision.material_type;
                    best_t = collision.t;
                }
            }
        }
    }

    /* choose the damage effect: equipped weapon's melee override, else the unit's default melee effect */
    unit_datum *unit_data2 = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int16_t weapon_slot = unit_data2->unit.current_weapon_index;
    int damage_definition_index;
    if ( weapon_slot == -1
      || unit_data2->unit.weapon_object_indices[weapon_slot] == -1 )
    {
        damage_definition_index = unit_definition_tag->unit.melee_damage.index;
    }
    else
    {
        int weapon_object_index = unit_data2->unit.weapon_object_indices[weapon_slot];
        weapon_datum *weapon_data = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
            weapon_object_index)->datum;
        weapon_definition *weapon_definition_tag = TAG_GET(weapon_definition, weapon_data->definition_index);
        damage_definition_index = weapon_definition_tag->weapon.melee_attack_damage.index;
        recoil_effect = weapon_definition_tag->weapon.melee_attack_response.index;
        if ( damage_definition_index == -1 )
            damage_definition_index = unit_definition_tag->unit.melee_damage.index;
    }

    /* shove a struck vehicle */
    if ( best_object != -1 )
    {
        unit_datum *struck = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
            best_object)->datum;
        if ( struck->object.type == object_type_vehicle && struck->object.datum_role != _networked_datum_puppet )
        {
            unit_definition *struck_definition_tag = TAG_GET(unit_definition, struck->definition_index);
            float shove = struck_definition_tag->object.acceleration_scale * 0.035f;
            real_vector3d acceleration;
            acceleration.n[0] = forward[0] * shove;
            acceleration.n[1] = unit_data->unit.aiming_vector.n[1] * shove;
            acceleration.n[2] = unit_data->unit.aiming_vector.n[2] * shove;
            vehicle_accelerate(best_object, &acceleration);
        }
    }

    if ( damage_definition_index != -1 )
    {
        damage_data damage;
        damage_data_new(&damage, damage_definition_index);
        damage.flags |= (1u << _damage_area_of_effect_bit);
        damage.owner_player_index = unit_data->unit.player_index;
        damage.owner_object_index = unit_index;
        damage.owner_team_index = unit_data->object.owner_team_index;
        damage.location = unit_data->object.location;
        damage.origin.n[0] = position.n[0];
        damage.origin.n[1] = position.n[1];
        damage.origin.n[2] = position.n[2];
        damage.epicenter.n[0] = unit_data->object.bounding_sphere_center.n[0];
        damage.epicenter.n[1] = unit_data->object.bounding_sphere_center.n[1];
        damage.epicenter.n[2] = unit_data->object.bounding_sphere_center.n[2];
        damage.direction.n[0] = forward[0];
        damage.direction.n[1] = unit_data->unit.aiming_vector.n[1];
        damage.direction.n[2] = unit_data->unit.aiming_vector.n[2];
        damage.material_type = material_type;

        if ( best_object == -1 )
        {
            if ( breakable_surface_index != -1 )
                breakable_surface_damage(breakable_surface_index, &damage, surface_index);
        }
        else
        {
            if ( DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, best_object)->datum->object.type == object_type_machine )
                machine_try_to_open_with_damage(best_object);

            /* damage scale from swing speed, normalized by the player run speed */
            game_globals_player_information *player_info =
                (game_globals_player_information *)global_game_globals->player_information.address;
            if ( player_info->run_forward_speed > 0.0f )
            {
                float swing = (((unit_data->object.forward.n[0] * unit_data->object.translational_velocity.n[0])
                                + ((unit_data->object.forward.n[2] * unit_data->object.translational_velocity.n[2])
                                   + (unit_data->object.forward.n[1] * unit_data->object.translational_velocity.n[1])))
                               * 30.0f) / player_info->run_forward_speed;
                damage.scale = swing;
                if ( swing >= 0.0f )
                {
                    if ( swing > 1.0f )
                        damage.scale = 1.0f;
                }
                else
                {
                    damage.scale = 0.0f;
                }
            }

            if ( unit_data->object.type == object_type_biped
              && ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                      unit_index)->datum)->biped.airborne_ticks > 15 )
                damage.scale = 1.5f;

            unit_datum *struck_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                best_object)->datum;
            if ( struck_data->object.type == object_type_biped )   /* struck a biped */
            {
                if ( hcex_unit_drop_grenade_on_melee
                  && game_team_is_enemy(unit_data->object.owner_team_index, struck_data->object.owner_team_index) )
                {
                    object_placement_data placement;
                    /* grenades.address dword[29] = grenades[1].item.index (the plasma-grenade item) */
                    object_placement_data_new(&placement,
                                              ((game_globals_grenade *)global_game_globals->grenades.address)[1].item.index,
                                              best_object);
                    int grenade_index = object_new(&placement);
                    if ( grenade_index != -1 )
                    {
                        object_disconnect_from_map(grenade_index);
                        unit_drop_item(best_object, grenade_index);
                    }
                    char grenade_count = struck_data->unit.grenade_counts[1];
                    if ( grenade_count > 0 )
                        struck_data->unit.grenade_counts[1] = grenade_count - 1;
                }

                if ( hcex_recharge_on_melee )
                {
                    int16_t player_index = (player_index_from_unit_index(unit_index) == -1)
                        ? -1
                        : DATA_ARRAY_ELEMENT(player_data, player_datum,
                              player_index_from_unit_index(unit_index))->local_player_index;
                    if ( player_index != -1 )
                        hcex_recharge_ticks[player_index] += 30;
                }

                object_cause_damage(&damage, best_object, -1, -1, -1, 0);
            }
        }
    }

    /* sound always plays (the source uses a non-short-circuiting OR over the call) */
    unit_melee_sound(unit_index, damage_definition_index, material_type);
    if ( material_type == -1 || recoil_effect == -1 )
    {
        unit_data->unit.melee_attack_state = _unit_melee_attack_none;
        return;
    }

    /* weapon recoil: a backward self-damage event */
    damage_data recoil;
    damage_data_new(&recoil, recoil_effect);
    recoil.flags |= (1u << _damage_from_weapon_bit);
    recoil.origin.n[0] = unit_data->object.bounding_sphere_center.n[0];
    recoil.origin.n[1] = unit_data->object.bounding_sphere_center.n[1];
    recoil.origin.n[2] = unit_data->object.bounding_sphere_center.n[2];
    recoil.epicenter.n[0] = unit_data->object.bounding_sphere_center.n[0];
    recoil.epicenter.n[1] = unit_data->object.bounding_sphere_center.n[1];
    recoil.epicenter.n[2] = unit_data->object.bounding_sphere_center.n[2];
    recoil.direction.n[0] = -forward[0];
    recoil.direction.n[1] = -unit_data->unit.aiming_vector.n[1];
    recoil.direction.n[2] = -unit_data->unit.aiming_vector.n[2];
    object_cause_damage(&recoil, unit_index, -1, -1, -1, 0);
    unit_data->unit.melee_attack_state = _unit_melee_attack_none;
}
