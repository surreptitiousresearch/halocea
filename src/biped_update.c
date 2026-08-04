/* biped_update @ 0x837B4028 — per-tick update for a biped unit. Blends client/server prediction, handles
 * the seated case (vehicle speech, auto-exit, rider ejection) by deferring to unit_update_animation with a
 * vehicle-derived desired state, and otherwise runs the on-foot biped logic: snap facing, normalize aim,
 * classify the movement state (idle/airborne/landing classes at +675), decay tiny velocities, age the
 * airborne/landing tick counters (+1281/+1282), then dispatch turning/moving and one of the
 * dead/airborne/landing/footstep updates. It then services the melee timer (starting a melee swing from the
 * current weapon when armed), footsteps, falling danger, and out-of-world deletion. Finally it advances the
 * unit animation and, if that requests a jump, calls biped_jump; the death-camera counter at +188 is bumped
 * while dead and active. Returns 1.
 *
 * DEVIATION: halo_hud_get_timer_enabled is called with no argument set at the call site (stale r3); it is
 * invoked argument-less here, matching the disassembly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/unit_datum.h"
#include "headers/unit_animation_update_data.h"
#include "headers/unit_animation_state.h"
#include "headers/networked_datum_role.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/biped_datum_flags.h"
#include "headers/material_effect_event.h"
#include "headers/unit_animation_action.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/first_person_weapon_animation.h"
#include "headers/blam_data_globals.h"


extern void biped_blend_client_and_server(int biped_index);
extern void biped_vehicle_speech(int biped_index);
extern uint8_t unit_try_and_exit_seat(int object_index, uint8_t should_allow_clients);
extern __int16 halo_hud_get_timer_enabled(void);
extern void hcex_fire_plr_event(const char *eventName, int plr_idx);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern void biped_snap_facing(biped_datum *biped, const biped_definition *biped_definition);
extern float normalize3d(real_vector3d *v);
extern void biped_update_turning(int biped_index, unit_animation_update_data *animation);
extern void biped_update_moving(int biped_index, unit_animation_update_data *animation_update);
extern void biped_update_dead(int biped_index, unit_animation_update_data *animation);
extern void biped_update_airborne(int biped_index, unit_animation_update_data *animation);
extern void biped_update_landing(int biped_index, unit_animation_update_data *animation);
extern void biped_make_footstep(int biped_index, int16_t event_index, int16_t contact_point_index);
extern void unit_cause_player_melee_damage(int unit_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern uint8_t weapon_prevents_melee_attack(int weapon_index);
extern void unit_animation_start_action(int unit_index, int16_t action);
extern void weapon_stop_reload(int weapon_index);
extern void first_person_weapon_message_from_unit(int unit_index, int16_t message_type);
extern uint16_t weapon_get_first_person_animation_time(int weapon_index, int16_t mode, int16_t animation_type, int16_t shotgun_reload_type);
extern void biped_try_to_make_footsteps(int biped_index);
extern void biped_falling_danger(int biped_index);
extern uint8_t game_engine_running(void);
extern void object_delete(int object_index);
extern int16_t unit_update_animation(int unit_index, unit_animation_update_data *data);
extern int biped_jump(int biped_index);

uint8_t biped_update(int biped_index)
{
    /* object-header datum pool entry is 12 bytes (salted); the biped_datum pointer lives at byte +8 */
    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    const biped_definition *definition =
        TAG_GET(const biped_definition, biped->definition_index);

    /* network-predicted, locally-simulated, unparented biped: blend the client and server states */
    if ( biped->object.datum_role == _networked_datum_puppet && biped->object.is_server_position_valid == 1
      && biped->object.parent_object_index == -1 )
    {
        biped_blend_client_and_server(biped_index);
    }

    unit_animation_update_data animation;
    animation.state_desired = 0;
    animation.crouching = 0;

    if ( biped->object.parent_object_index != -1 )
    {
        /* seated: behaviour depends on the parent object type */
        unit_datum *parent = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped->object.parent_object_index)->datum;
        __int16 parent_type = parent->object.type;
        if ( parent_type == 1 )
        {
            /* riding a vehicle */
            biped_vehicle_speech(biped_index);
            if ( (biped->unit.control_flags & (1u << _unit_control_action_bit)) != 0 )
                unit_try_and_exit_seat(biped_index, 0);
            if ( rider_ejection && parent->object.up.n[2] < 0.0f && (parent->object.flags & (1u << _object_on_ground_bit)) != 0 )
            {
                /* DEVIATION: halo_hud_get_timer_enabled takes no argument at the call site (stale r3). */
                if ( biped->object.owner_player_index != -1 && (unsigned __int16)halo_hud_get_timer_enabled() )
                    hcex_fire_plr_event("player_ejected_ride", -1);
                unit_exit_seat_end(biped_index, 0, 0, 1u);
            }
        }
        else if ( !parent_type )
        {
            /* seated on another biped */
            animation.state_desired = (parent->object.damage_flags & (1u << _object_dead_bit)) | 0x20;
        }
    }
    else
    {
        biped_snap_facing(biped, definition);

        if ( (biped->object.damage_flags & (1u << _object_dead_bit)) != 0
            || ((definition->biped.flags & (1u << _biped_flying_bit)) == 0 && (definition->biped.flags & (1u << _biped_climbs_anything_bit)) == 0) )
        {
            biped->unit.desired_facing_vector.n[2] = 0.0f;
            if ( normalize3d(&biped->unit.desired_facing_vector) == 0.0f )
                biped->unit.desired_facing_vector = *global_forward3d;
        }

        /* classify unit.animation.state into biped.state (0 idle, 1 moving, 2 airborne) */
        unsigned __int8 state = (unsigned __int8)biped->unit.animation.state;
        char animation_class;
        if ( state > _unit_state_move_right || state == _unit_state_gesture )
            animation_class = 2;
        else if ( state == _unit_state_idle || state == _unit_state_turn_left || state == _unit_state_turn_right )
            animation_class = 0;
        else
            animation_class = 1;
        biped->biped.state = animation_class;

        /* squelch sub-threshold aim velocity */
        if ( ((biped->unit.throttle.n[2] * biped->unit.throttle.n[2])
                + ((biped->unit.throttle.n[0] * biped->unit.throttle.n[0])
                    + (biped->unit.throttle.n[1] * biped->unit.throttle.n[1]))) < 0.010000001f )
            biped->unit.throttle = *global_zero_vector3d;

        /* age airborne / slipping tick counters from the biped physics flags */
        int physics_state = biped->biped.flags;
        if ( (physics_state & (1u << _biped_airborne_bit)) != 0 )
        {
            if ( biped->biped.airborne_ticks != 127 )
                ++biped->biped.airborne_ticks;
        }
        else
        {
            biped->biped.airborne_ticks = 0;
        }
        if ( (physics_state & (1u << _biped_slipping_bit)) != 0 )
        {
            if ( biped->biped.slipping_ticks != 127 )
                ++biped->biped.slipping_ticks;
        }
        else
        {
            biped->biped.slipping_ticks = 0;
        }

        animation.state_desired = 0;
        animation.crouching = biped->unit.control_flags & (1u << _unit_control_crouch_modifier_bit);

        if ( (biped->object.damage_flags & (1u << _object_dead_bit)) == 0 )
            biped_update_turning(biped_index, &animation);
        biped_update_moving(biped_index, &animation);

        if ( (biped->object.damage_flags & (1u << _object_dead_bit)) != 0 )
        {
            biped_update_dead(biped_index, &animation);
        }
        else
        {
            int state_flags = biped->biped.flags;
            if ( (state_flags & (1u << _biped_airborne_bit)) != 0 )
            {
                biped_update_airborne(biped_index, &animation);
            }
            else if ( (unsigned __int16)biped->biped.landing == 0xFFFF )
            {
                if ( (state_flags & (1u << _biped_slipping_bit)) != 0 )
                {
                    /* recovered: `self` reload via object_header_data resolves to the same biped datum pointer */
                    if ( biped->biped.slipping_ticks > 3
                        && ((biped->object.translational_velocity.n[2] * biped->object.translational_velocity.n[2])
                                + ((biped->object.translational_velocity.n[0] * biped->object.translational_velocity.n[0])
                                    + (biped->object.translational_velocity.n[1] * biped->object.translational_velocity.n[1]))) > 0.0011111111f
                        && !g_running_simulated_update )
                    {
                        biped_make_footstep(biped_index, _material_effect_biped_sliding, 0);
                        biped_make_footstep(biped_index, _material_effect_biped_sliding, 1);
                    }
                }
            }
            else
            {
                biped_update_landing(biped_index, &animation);
            }
        }

        if ( !g_running_simulated_update )
        {
            /* player melee swing timer, with damage window at player_melee_attack_tick */
            if ( biped->biped.player_melee_ticks )
            {
                if ( biped->biped.player_melee_ticks == biped->biped.player_melee_attack_tick )
                    unit_cause_player_melee_damage(biped_index);
                --biped->biped.player_melee_ticks;
            }
            else if ( biped->unit.player_index != -1 && (biped->unit.control_flags & (1u << _unit_control_use_equipment_bit)) != 0 )
            {
                int weapon = unit_inventory_get_weapon(biped_index, biped->unit.current_weapon_index);
                if ( !weapon_prevents_melee_attack(weapon) && (unsigned __int8)biped->unit.current_zoom_level == 255 )
                {
                    unit_animation_start_action(biped_index, _unit_animation_action_melee);
                    weapon_stop_reload(weapon);
                    first_person_weapon_message_from_unit(biped_index, _first_person_weapon_message_melee);
                    char total_time = (char)weapon_get_first_person_animation_time(weapon, 0, _first_person_weapon_animation_melee, -1);
                    biped->biped.player_melee_ticks = total_time;
                    char damage_offset = (char)weapon_get_first_person_animation_time(weapon, 1, _first_person_weapon_animation_melee, -1);
                    biped->biped.player_melee_attack_tick = total_time - damage_offset;
                    biped->biped.player_melee_ticks = total_time - (total_time >> 2);
                    biped->biped.player_melee_attack_tick = total_time - damage_offset - (total_time >> 2);
                }
            }
        }

        if ( !g_running_simulated_update )
            biped_try_to_make_footsteps(biped_index);
        if ( !g_running_simulated_update )
            biped_falling_danger(biped_index);
        if ( !g_running_simulated_update )
        {
            /* recovered: `self` reload resolves to `biped`; self+156 reads object.location.cluster_index
             * (location+4); 0xFFFF means "not in world") */
            if ( !game_engine_running()
                && ((biped->object.flags & (1u << _object_outside_of_map_bit)) != 0
                    || (unsigned __int16)biped->object.location.cluster_index == 0xFFFF)
                && biped->object.position.n[2] < -2000.0f )
            {
                object_delete(biped_index);
            }
        }
    }

    if ( unit_update_animation(biped_index, &animation) == 1 )
        biped_jump(biped_index);

    if ( (biped->object.damage_flags & (1u << _object_dead_bit)) != 0 && (biped->object.flags & (1u << _object_at_rest_bit)) != 0 )
    {
        ++biped->object.idle_ticks;
        return 1;
    }
    biped->object.idle_ticks = 0;
    return 1;
}
