/* action_obey_command_perform @0x837DE148 — evaluate one command of a scenario AI "command list" for an
 * actor obeying scripted orders, returning whether the command has completed (so the caller can advance to
 * the next command). Dispatches on the command opcode (word 0 of the 32-byte command record); most opcodes
 * are instantaneous (return 1) or gate on the simple-control pause timer, while move / directmovement /
 * shoot / grenade / animation opcodes test the unit's live state.
 *
 * Clean decompile (0x837DE148). The actor record, the scenario command list (DB ai_command_definition), and unit object datums
 * are read through named DB struct members; residual offsets are named in comments. The two branchless
 * `(x - A) - ((x - (A+1)) + (x == A))` expressions (opcodes 0xD/0x10) are exactly `state != A` and the
 * `(-x & ~x) >> 31` (opcode 0x13) is `awareness > 0`; written directly. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/object_datum.h"
#include "headers/obey_simple_control_flags.h"
#include "headers/obey_metadata_flags.h"
#include "headers/ai_command_definition.h"
#include "headers/ai_command_list_definition.h"
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/actor_fire_target_type.h"
#include "headers/unit_animation_state.h"
#include "headers/actor_combat_status.h"
#include "headers/ai_atom_type.h"
#include "headers/blam_data_globals.h"
#include "headers/unit_speech_priority.h"


extern uint8_t actor_path_at_destination(uint16_t actor_index);
extern float actor_destination_tolerance(uint16_t actor_index);
extern float normalize3d(real_vector3d *v);
extern float normalize2d(real_vector2d *v);
extern uint8_t actor_move_halt(uint16_t actor_index);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern uint8_t unit_is_busy(int object_index);
extern uint8_t unit_flying_through_air(int unit_index);
extern uint8_t recorded_animation_controlling_unit(int unit_index);
extern uint8_t actor_combat_plan_grenade_trajectory(int actor_index, int16_t trajectory_type, const real_point3d *grenade_target, int grenade_target_prop_index, int grenade_ignore_object_index);

uint8_t action_obey_command_perform(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control)
{
    int current_command_index = simple_control->current_command_index;
    int done = 1;
    ai_command_list_definition *command_list = (ai_command_list_definition *)global_scenario->ai_command_lists.address + command_list_index;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( current_command_index >= command_list->commands.count )   /* past the last command */
        return done;

    ai_command_definition *command = (ai_command_definition *)command_list->commands.address + current_command_index;
    int actor_unit_index = actor->meta.unit_index;   /* actor meta.unit_index (+0x18) */

    switch ( *(int16_t *)command )
    {
        case _ai_atom_pause:
        case _ai_atom_look:
        case _ai_atom_move_immediate:
        case _ai_atom_look_random:
        case _ai_atom_look_player:
        case _ai_atom_look_object:
            return simple_control->pause_timer == 0;

        case _ai_atom_go_to:
        case _ai_atom_go_to_and_face:
            if ( unit_index != actor_unit_index || !complex_control )
                return 1;
            done = actor_path_at_destination(actor_index);
            if ( !(char)done )
            {
                if ( complex_control->destination_keep_moving && complex_control->destination_valid )
                {
                    float tolerance = actor_destination_tolerance(actor_index);
                    float dx = complex_control->destination_point.n[0] - actor->input.position.body_position.n[0];
                    float dy = complex_control->destination_point.n[1] - actor->input.position.body_position.n[1];
                    float dz = complex_control->destination_point.n[2] - actor->input.position.body_position.n[2];
                    float distance_squared = dx * dx + (dz * dz + dy * dy);
                    object_datum *unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor_unit_index)->datum;
                    if ( distance_squared < tolerance * tolerance
                      || (distance_squared < (tolerance + 0.5f) * (tolerance + 0.5f)
                          && unit_object->object.translational_velocity.n[0] * (complex_control->destination_point.n[0] - actor->input.position.body_position.n[0])
                              + (unit_object->object.translational_velocity.n[2] * (complex_control->destination_point.n[2] - actor->input.position.body_position.n[2])
                                  + unit_object->object.translational_velocity.n[1] * (complex_control->destination_point.n[1] - actor->input.position.body_position.n[1]))
                              < 0.0f) )   /* velocity already carrying it past the point */
                        done = 1;
                }
            }
            if ( command->atom_type == _ai_atom_go_to_and_face || !command->atom_modifier )
            {
                if ( actor->control.moving )
                    simple_control->pause_timer = 10;
                uint8_t arrived = 1;
                if ( !(char)done || simple_control->pause_timer )
                    arrived = 0;
                done = arrived;
            }
            if ( !(char)done || !complex_control->destination_facing )
                goto move_done;

            {
                real_vector3d facing;
                facing.n[0] = complex_control->destination_facing_point.n[0] - actor->input.position.body_position.n[0];
                float facing_dot;
                if ( actor->state.flying )   /* full 3D facing check */
                {
                    facing.n[2] = complex_control->destination_facing_point.n[2] - actor->input.position.body_position.n[2];
                    facing.n[1] = complex_control->destination_facing_point.n[1] - actor->input.position.body_position.n[1];
                    if ( normalize3d(&facing) <= 0.0f )
                        goto move_done;
                    facing_dot = actor->input.facing_vector.n[0] * facing.n[0]
                            + (actor->input.facing_vector.n[2] * facing.n[2] + actor->input.facing_vector.n[1] * facing.n[1]);
                }
                else                /* horizontal-only facing check */
                {
                    facing.n[1] = complex_control->destination_facing_point.n[1] - actor->input.position.body_position.n[1];
                    if ( normalize2d((real_vector2d *)&facing) <= 0.0f )
                        goto move_done;
                    facing_dot = actor->input.facing_vector.n[1] * facing.n[1] + actor->input.facing_vector.n[0] * facing.n[0];
                }
                if ( facing_dot < 0.98400003f )
                    done = 0;
            }

        move_done:
            if ( !(char)done )
                return done;
            actor_move_halt(actor_index);
            return done;

        case _ai_atom_move_direction:
        {
            real_point3d origin;
            if ( unit_index == actor_unit_index )
            {
                origin.n[0] = actor->input.position.body_position.n[0];
                origin.n[1] = actor->input.position.body_position.n[1];
                origin.n[2] = actor->input.position.body_position.n[2];
            }
            else
            {
                object_get_origin(unit_index, &origin);
            }
            return (origin.n[0] - simple_control->___u5.directmovement.start_position.n[0])
                            * simple_control->___u5.directmovement.vector.n[0]
                    + (simple_control->___u5.directmovement.vector.n[2]
                            * (origin.n[2] - simple_control->___u5.directmovement.start_position.n[2])
                        + simple_control->___u5.directmovement.vector.n[1]
                            * (origin.n[1] - simple_control->___u5.directmovement.start_position.n[1]))
                    > (double)command->parameter1;
        }

        case _ai_atom_animation_mode:
        case _ai_atom_crouch:
        case _ai_atom_vehicle:
        case _ai_atom_script:
        case _ai_atom_targeting:
        case _ai_atom_initiative:
        case _ai_atom_loop:
        case _ai_atom_die:
        case _ai_atom_set_radius:
        case _ai_atom_teleport:
            return 1;

        case _ai_atom_shoot:
            if ( unit_index != actor_unit_index || !complex_control )
                return 1;
            {
                /* DEVIATION: decompiler pun shifted the reads down one field (float at &current_fire_target_type);
                 * disasm 0x837DE55C..7C loads control+0x1A4/0x1A8/0x1AC = manual_point.x/y/z (DB-verified). */
                float sx = actor->control.___u58.current_fire_target_manual_point.x - complex_control->shoot_target.n[0];
                float sy = actor->control.___u58.current_fire_target_manual_point.y - complex_control->shoot_target.n[1];
                float sz = actor->control.___u58.current_fire_target_manual_point.z - complex_control->shoot_target.n[2];
                if ( actor->control.current_fire_target_type != actor_fire_target_manual_point || sx * sx + (sz * sz + sy * sy) >= 0.25f )
                {
                    /* not shooting the target yet: hold for the variant's first-burst delay (sec->ticks), floored at 60 */
                    actor_variant_definition *variant = TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
                    int16_t timer = (int)(variant->ranged_combat.first_burst_delay_upper_bound * 30.0f);  /* variant+132 */
                    if ( timer <= 60 )
                        timer = 60;
                    simple_control->pause_timer = timer;
                }
            }
            return simple_control->pause_timer == 0;

        case _ai_atom_grenade:
            if ( unit_index != actor_unit_index || !complex_control )
                return 1;
            if ( complex_control->grenade_throw_started )
            {
                /* recovered: unit datum byte +653 -> unit.grenade_throw_state (unit_datum.unit is at
                 * +500, member at +153/0x99). Not a flag word: it is the 4-state throw ladder, and
                 * 0x1E is a 30-tick (1 s) hold, not a mask. Any state but _none = throw underway. */
                uint8_t throw_state = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum)
                                          ->unit.grenade_throw_state;
                simple_control->pause_timer = (throw_state != _grenade_throw_state_none) ? 30 : 0;
                return simple_control->pause_timer == 0;
            }
            else
            {
                if ( !unit_is_busy(unit_index) )
                {
                    real_point3d grenade_target = complex_control->grenade_target;
                    if ( actor_combat_plan_grenade_trajectory(actor_index,
                            complex_control->grenade_throw_trajectory_type, &grenade_target, -1, -1) )
                        complex_control->grenade_throw_depress_trigger = 1;
                }
                return simple_control->pause_timer == 0;
            }

        case _ai_atom_running_jump:
        case _ai_atom_targeted_jump:
        {
            if ( (simple_control->simple_control_flags & (1u << _obey_simple_control_jump_bit)) == 0 )
                return 1;
            uint8_t airborne;
            if ( unit_index == actor_unit_index )
                airborne = actor->input.in_midair;
            else
                airborne = unit_flying_through_air(unit_index);
            if ( (simple_control->simple_control_flags & (1u << _obey_simple_control_jump_begun_bit)) != 0 && airborne )
                simple_control->pause_timer = 0;
            return simple_control->pause_timer == 0;
        }

        case _ai_atom_animate:
        {
            unsigned char *unit = ((unsigned char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
            return ((unit_datum *)unit)->unit.animation.state != _unit_state_user_animation;   /* animation state != 28 (branchless in the binary) */
        }

        case _ai_atom_recording:
            return recorded_animation_controlling_unit(unit_index) == 0;

        case _ai_atom_action:
            if ( !complex_control )
                return 1;
            if ( !complex_control->play_action )
                return 1;
            return 0;

        case _ai_atom_vocalize:
        {
            unsigned char *unit = ((unsigned char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
            return (uint16_t)((unit_datum *)unit)->unit.speech.current.priority != _unit_speech_scripted;   /* branchless in the binary */
        }

        case _ai_atom_wait:
        {
            unsigned int parameter = (uint16_t)command->atom_modifier;
            int result;
            if ( parameter )
            {
                if ( parameter == 1 )
                {
                    result = actor->state.combat_status >= _actor_combat_status_visible;
                }
                else
                {
                    if ( parameter >= 3 )
                        return 1;
                    /* parameter == 2: one-shot latch on told_to_advance -> waiting_for_advance_notification */
                    uint8_t metadata_flags = simple_control->metadata_flags;
                    if ( (metadata_flags & (1u << _obey_metadata_told_to_advance_bit)) != 0 )
                    {
                        simple_control->metadata_flags = metadata_flags
                                & ~((1u << _obey_metadata_told_to_advance_bit) | (1u << _obey_metadata_waiting_for_advance_notification_bit));
                        return 1;
                    }
                    simple_control->metadata_flags = metadata_flags | (1u << _obey_metadata_waiting_for_advance_notification_bit);
                    result = 0;
                }
            }
            else
            {
                result = actor->state.combat_status > _actor_combat_status_none;
            }
            return result;
        }

        default:
            return done;
    }
}
