/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* action_obey_command_begin @0x837DCF60 — begin executing one command (atom) of a scenario AI
 * "command list" for an actor obeying scripted orders. Dispatches on the command opcode
 * (ai_command_definition.atom_type) and sets up the actor's simple/complex control blocks
 * (move-to, direct-movement, look, shoot, grenade, vehicle-enter, animation, speech, teleport, ...).
 * Returns whether the command started successfully (the perform variant then advances the list).
 *
 * Reconstructed from a clean decompile (0x837DCF60). The actor record (actor_data, stride 1828), the
 * object datums reached through object_header_data, and the actor/variant definition tags are typed
 * structs (actor_datum, object_datum, actor_definition, actor_variant_definition); the one exception is
 * the teleport atom's (0x1B) unit tag read, which is left as a raw offset because the same byte means a
 * biped's flags word for one unit kind and an unrelated vehicle field for the other — see the comment at
 * that call site. The scenario command list / command / point blocks are read through the DB-verified
 * ai_command_list_definition / ai_command_definition / ai_command_point_definition types.
 * The single stack scratch slot the compiler shared across the prop/data/object iterators and the look
 * direction_specification is split back into separate typed locals (equivalent lifetimes). Several
 * `(_cntlzw(x - 1) & 0x20) != 0` idioms are exactly `x == 1` (and `(_cntlzw(x) & 0x20) != 0` is `x == 0`)
 * and are written that way; the opcode-0xD flag goto-lattice is expanded into its equivalent switch. */

#include <stdint.h>
#include <string.h>
#include "headers/ai_animation_reference_definition.h"
#include "headers/biped_definition.h"
#include "headers/biped_definition_flags.h"
#include "headers/unit_animation_impulse.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/prop_iterator.h"
#include "headers/prop_datum.h"
#include "headers/object_iterator.h"
#include "headers/vehicle_possibility.h"
#include "headers/object_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_simple_control_flags.h"
#include "headers/obey_metadata_flags.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/direction_specification.h"
#include "headers/unit_speech_item.h"
#include "headers/actor_position_data.h"
#include "headers/ai_command_list_definition.h"
#include "headers/ai_command_definition.h"
#include "headers/ai_command_point_definition.h"
#include "headers/ai_script_reference_definition.h"
#include "headers/ai_recording_reference_definition.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/unit_definition.h"
#include "headers/_object_definition.h"
#include "headers/object_damage_flags.h"
#include "headers/ai_atom_type.h"
#include "headers/object_type.h"
#include "headers/prop_perception_state.h"
#include "headers/math_constants.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/ai_communication_type.h"
#include "headers/unit_speech_priority.h"
#include "headers/biped_datum.h"
#include "headers/biped_datum_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/direction_specification_type.h"
#include "headers/ai_atom_look_modifier.h"
#include "headers/ai_atom_animate_modifier.h"
#include "headers/ai_atom_action_modifier.h"


/* DEVIATION: the seat-name substring argument was reconstructed as
 * `extern const char ai_vehicle_any_seat_name;` at 0x8200155A. That address is the image's shared
 * empty-string constant (a lone NUL byte), not a named seat-name object - the original passed a
 * "" literal meaning "any seat". Restored as the literal; no storage is owed. */
#include "headers/ai_information_packet.h"
#include "headers/actor_position_data.h"
#include "headers/scenario.h"

extern void   qsort(void *base, unsigned int num, unsigned int width,
                    int (*comp)(const void *, const void *));
extern int vehicle_possibility_qsort(const void *p1, const void *p2);

extern uint8_t actor_move_to_point(int actor_index, real_point3d *destination, int surface_index, int ignore_target_object_index);
extern void actor_move_keep_moving_past_destination(uint16_t actor_index);
extern uint8_t actor_move_halt(uint16_t actor_index);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern float normalize3d(real_vector3d *v);
extern float normalize2d(real_vector2d *v);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern int object_index_from_name_index(int16_t name_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);
/* direction_get_empty is __cdecl variadic in the DB; called here with (result, actor_def_index,
 * command_type, variant_definition_data). */
extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(uint16_t actor_index, int16_t type, int16_t priority, direction_specification *direction);
extern int16_t unit_add_grenade_type_to_inventory(int unit_index, int16_t grenade_type, int16_t grenade_count);
extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);
extern uint8_t actor_action_try_to_enter_vehicle(int actor_index, int vehicle_index, const char *seat_substring_name, int16_t seat_desire_type, int16_t precomputed_seat_count, int16_t *precomputed_seat_indices);
extern int hs_wake_by_name(const char *script_name);
extern uint8_t unit_start_user_animation(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate);
extern int16_t scenario_get_animation_by_name(scenario *scenario, const char *animation_name);
extern uint8_t recorded_animation_play(int unit_index, int16_t animation_index);
extern int16_t unit_test_speech(int unit_index, int16_t priority, uint8_t allow_recursive_lookup, uint8_t allow_queue, int *unit_last_speech_time, int16_t *vocalization_type_reference, int *sound_definition_index_reference);
extern void ai_communication_packet_new(ai_information_packet *information);
extern void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item);
extern void action_obey_directmovement_update_facing(uint16_t actor_index, int unit_index, obey_individual_simple_control *simple_control);
extern void unit_get_facing_vector(int unit_index, real_vector3d *facing_vector);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern void object_reset(int object_index);
extern void object_compute_node_matrices_recursive(int object_index);
extern void actor_input_sample_position(int actor_index, int unit_index, actor_position_data *position);

uint8_t action_obey_command_begin(
        int actor_index,
        int unit_index,
        int16_t command_list_index,
        obey_individual_simple_control *simple_control,
        obey_individual_complex_control *complex_control)
{
    int current_command_index = simple_control->current_command_index;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int actor_definition_index = actor->meta.definition_index;
    ai_command_list_definition_t *command_list =
        &((ai_command_list_definition_t *)global_scenario->ai_command_lists.address)[command_list_index];
    int command_count = command_list->commands.count;
    int command_result = 0;

    /* actor's two tag references: the actor definition (+22) and its variant definition (+23). */
    actor_definition *actor_definition_data = TAG_GET(actor_definition, actor_definition_index);
    actor_variant_definition *variant_definition_data = TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);

    if ( current_command_index >= command_count )
        return command_result;

    ai_command_definition *command =
        &((ai_command_definition *)command_list->commands.address)[current_command_index];
    ai_command_point_definition *points = (ai_command_point_definition *)command_list->points.address;
    int16_t atom_type = command->atom_type;

    switch ( (uint16_t)command->atom_type )
    {
        case _ai_atom_pause:   /* pause */
            simple_control->pause_timer = (int)(command->parameter1 * (float)30.0);
            command_result = 1;
            break;

        case _ai_atom_go_to:   /* move to point (optionally facing a second point) */
        case _ai_atom_go_to_and_face:
        {
            if ( !complex_control )
                return command_result;
            int destination_point_index = command->point1_index;
            if ( destination_point_index < 0 || destination_point_index >= command_list->points.count )
                return command_result;
            ai_command_point_definition *destination_point = &points[destination_point_index];
            simple_control->pause_timer = 0;
            complex_control->destination_valid = 1;
            complex_control->destination_keep_moving = (command->atom_modifier == 1);
            complex_control->destination_point.n[0] = destination_point->position.n[0];
            complex_control->destination_point.n[1] = destination_point->position.n[1];
            complex_control->destination_point.n[2] = destination_point->position.n[2];
            complex_control->destination_surface_index = destination_point->surface_index;
            command_result = actor_move_to_point(actor_index, &complex_control->destination_point,
                                                 destination_point->surface_index, -1);
            if ( !(uint8_t)command_result )
                return command_result;
            if ( complex_control->destination_keep_moving )
                actor_move_keep_moving_past_destination(actor_index);
            if ( (uint16_t)command->atom_type != _ai_atom_go_to_and_face )
                return command_result;
            int facing_point_index = command->point2_index;
            if ( facing_point_index < 0 || facing_point_index >= command_list->points.count )
                return command_result;
            complex_control->destination_facing = 1;
            ai_command_point_definition *facing_point = &points[facing_point_index];
            complex_control->destination_facing_point.n[0] = facing_point->position.n[0];
            complex_control->destination_facing_point.n[1] = facing_point->position.n[1];
            complex_control->destination_facing_point.n[2] = facing_point->position.n[2];
            return command_result;
        }

        case _ai_atom_move_direction:   /* direct movement (walk a vector) */
        {
            if ( unit_index == actor->meta.unit_index )
            {
                simple_control->___u5.directmovement.start_position.n[0] = actor->input.position.body_position.n[0];
                simple_control->___u5.directmovement.start_position.n[1] = actor->input.position.body_position.n[1];
                simple_control->___u5.directmovement.start_position.n[2] = actor->input.position.body_position.n[2];
            }
            else
            {
                object_get_origin(unit_index, &simple_control->___u5.directmovement.start_position);
            }
            int direction_point_index = command->point1_index;
            if ( direction_point_index < 0 || direction_point_index >= command_list->points.count )
            {
                float angle_degrees = command->parameter2;
                if ( angle_degrees >= 0.0 && angle_degrees < 360.0 )
                {
                    vector3d_from_angle(&simple_control->___u5.directmovement.vector,
                                        (command->parameter2 * DEG_TO_RAD));
                    command_result = 1;
                }
            }
            else
            {
                ai_command_point_definition *direction_point = &points[direction_point_index];
                simple_control->___u5.directmovement.vector.n[0] =
                    direction_point->position.n[0] - simple_control->___u5.directmovement.start_position.n[0];
                simple_control->___u5.directmovement.vector.n[1] =
                    direction_point->position.n[1] - simple_control->___u5.directmovement.start_position.n[1];
                simple_control->___u5.directmovement.vector.n[2] =
                    direction_point->position.n[2] - simple_control->___u5.directmovement.start_position.n[2];
                command_result = normalize3d(&simple_control->___u5.directmovement.vector) > 0.0;
            }
            if ( !(uint8_t)command_result )
                return command_result;
            int16_t facing = command->atom_modifier;
            if ( (unsigned int)facing >= 4 )
                facing = -1;
            simple_control->___u5.directmovement.facing = facing;
            if ( unit_index == actor->meta.unit_index )
                actor_move_halt(actor_index);
            /* clear directmovement + update_continuously, then latch directmovement */
            simple_control->simple_control_flags =
                (simple_control->simple_control_flags
                 & ~((1u << _obey_simple_directmovement_bit)
                   | (1u << _obey_simple_directmovement_update_continuously_bit)))
                | (1u << _obey_simple_directmovement_bit);
            return command_result;
        }

        case _ai_atom_look:    /* look at point / prop / player / named object */
        case _ai_atom_look_random:
        case _ai_atom_look_player:
        case _ai_atom_look_object:
        {
            if ( !complex_control )
                return command_result;
            float look_duration = command->parameter1;
            int16_t look_point_index = -1;
            int look_prop_index = -1;
            int look_unit_index = -1;
            switch ( atom_type )
            {
                case _ai_atom_look:
                {
                    int16_t point_index = command->point1_index;
                    if ( point_index >= 0 && point_index < command_list->points.count )
                        look_point_index = command->point1_index;
                    break;
                }
                case _ai_atom_look_random:   /* look at a random one of a point range */
                {
                    int16_t range_low = command->point1_index;
                    if ( range_low >= 0 )
                    {
                        int point_count = command_list->points.count;
                        if ( range_low < point_count )
                        {
                            int16_t range_high = command->point2_index;
                            if ( range_high >= 0 && range_high < point_count )
                            {
                                int16_t range_high_inclusive = range_high + 1;
                                unsigned int *seed = get_global_random_seed_address();
                                look_point_index = seed_random_range(seed, range_low, range_high_inclusive);
                                if ( command->parameter1 == 0.0 && command->parameter2 == 0.0 )
                                {
                                    /* default look duration range comes from the actor definition tag. */
                                    float duration_upper = actor_definition_data->looking.idle_look_noncombat.idle_look_time_upper_bound;
                                    float duration_lower = actor_definition_data->looking.idle_look_noncombat.idle_look_time_lower_bound;
                                    unsigned int *duration_seed = get_global_random_seed_address();
                                    look_duration = real_seed_random_range(duration_seed, duration_lower, duration_upper);
                                }
                                else
                                {
                                    float duration_upper = command->parameter2;
                                    unsigned int *duration_seed = get_global_random_seed_address();
                                    look_duration = real_seed_random_range(duration_seed, command->parameter1, duration_upper);
                                }
                            }
                        }
                    }
                    break;
                }
                case _ai_atom_look_player:   /* look at nearest interesting prop, else nearest player */
                {
                    float best_prop_distance = 3.4028235e38;
                    float best_player_distance_sq = 3.4028235e38;
                    prop_iterator prop_cursor;
                    prop_iterator_new(&prop_cursor, actor_index);
                    prop_datum *prop = prop_iterator_next(&prop_cursor);
                    if ( prop )
                    {
                        do
                        {
                            int prop_state = prop->state;
                            if ( prop_state >= _prop_state_becoming_unacknowledged && prop_state <= _prop_state_acknowledged && prop->player && prop->distance < best_prop_distance )
                            {
                                look_prop_index = prop_cursor.index;
                                best_prop_distance = prop->distance;
                            }
                            prop = prop_iterator_next(&prop_cursor);
                        }
                        while ( prop );
                    }
                    if ( look_prop_index == -1 )
                    {
                        data_iterator player_cursor;
                        data_iterator_new(&player_cursor, player_data);
                        for ( player_datum *player = data_iterator_next(&player_cursor); player; player = data_iterator_next(&player_cursor) )
                        {
                            int player_unit_index = player->unit_index;
                            if ( player_unit_index != -1 )
                            {
                                real_point3d player_head;
                                unit_get_head_position(player_unit_index, &player_head);
                                float delta_x = actor->input.position.head_position.n[0] - player_head.n[0];   /* actor eye position */
                                float delta_z = actor->input.position.head_position.n[2] - player_head.n[2];
                                float delta_y = actor->input.position.head_position.n[1] - player_head.n[1];
                                float distance_sq = delta_x * delta_x + (delta_z * delta_z + delta_y * delta_y);
                                if ( distance_sq < best_player_distance_sq )
                                {
                                    look_unit_index = player->unit_index;
                                    best_player_distance_sq = distance_sq;
                                }
                            }
                        }
                    }
                    break;
                }
                case _ai_atom_look_object:   /* look at a named scenario object */
                {
                    int16_t object_name_index = command->object_name_index;
                    if ( object_name_index >= 0 && object_name_index < global_scenario->object_names.count )
                    {
                        int object_index = object_index_from_name_index(object_name_index);
                        if ( object_try_and_get_and_verify_type(object_index, object_mask_unit) )
                        {
                            look_prop_index = prop_get_active_by_unit_index(actor_index, object_index);
                            look_unit_index = object_index;
                        }
                    }
                    break;
                }
            }
            if ( look_duration <= 0.0
              || (look_prop_index == -1 && look_unit_index == -1
                  && (look_point_index < 0 || look_point_index >= command_list->points.count)) )
                return command_result;

            direction_specification look_direction;
            /* attested 1-param: the extra args were live-register decompiler artifacts */
            direction_get_empty(&look_direction);
            int16_t aim_priority = _secondary_look_priority_default;
            switch ( (uint16_t)command->atom_modifier )
            {
                case _ai_atom_look_modifier_idle_turn_around: aim_priority = _secondary_look_priority_turn_and_aim; break;
                case _ai_atom_look_modifier_idle_look:        aim_priority = _secondary_look_priority_idle_look; break;
                case _ai_atom_look_modifier_force_aim_weapon: aim_priority = _secondary_look_priority_override; break;
                case _ai_atom_look_modifier_force_facing:     aim_priority = _secondary_look_priority_override_exact_facing; break;
            }
            if ( look_prop_index == -1 )
            {
                if ( look_unit_index == -1 )
                {
                    ai_command_point_definition *look_point = &points[look_point_index];
                    look_direction.type = _direction_specification_point;
                    look_direction.___u1.point.n[0] = look_point->position.n[0];
                    look_direction.___u1.point.n[1] = look_point->position.n[1];
                    look_direction.___u1.point.n[2] = look_point->position.n[2];
                }
                else
                {
                    look_direction.type = _direction_specification_point;
                    unit_get_head_position(look_unit_index, &look_direction.___u1.point);
                }
            }
            else
            {
                look_direction.___u1.prop_index = look_prop_index;
                look_direction.type = _direction_specification_prop;
            }
            actor_look_secondary(actor_index, _secondary_look_scripted, aim_priority, &look_direction);
            simple_control->pause_timer = (int)(look_duration * (float)30.0);
            command_result = 1;
            break;
        }

        case _ai_atom_animation_mode:   /* override movement type */
        {
            if ( !complex_control )
                return command_result;
            int16_t movement_type = command->atom_modifier;
            if ( (unsigned int)movement_type >= 4 )
                return command_result;
            complex_control->override_movement_type = movement_type;
            command_result = 1;
            break;
        }

        case _ai_atom_crouch:   /* override crouch */
            if ( !complex_control )
                return command_result;
            complex_control->override_crouch = (command->atom_modifier == 1);
            command_result = 1;
            break;

        case _ai_atom_shoot:   /* shoot at a point */
        {
            if ( !complex_control )
                return command_result;
            int shoot_point_index = command->point1_index;
            if ( shoot_point_index < 0 || shoot_point_index >= command_list->points.count )
                return command_result;
            complex_control->shoot_at_target = 1;
            ai_command_point_definition *shoot_point = &points[shoot_point_index];
            complex_control->shoot_target.n[0] = shoot_point->position.n[0];
            complex_control->shoot_target.n[1] = shoot_point->position.n[1];
            complex_control->shoot_target.n[2] = shoot_point->position.n[2];
            complex_control->shoot_burst_length = command->parameter1;
            command_result = 1;
            break;
        }

        case _ai_atom_grenade:   /* throw a grenade at a point */
        {
            if ( !complex_control )
                return command_result;
            int grenade_type = (uint16_t)variant_definition_data->grenade_combat.grenade_type;
            if ( grenade_type == 0xFFFF )
                return command_result;
            int grenade_point_index = command->point1_index;
            if ( grenade_point_index < 0 || grenade_point_index >= command_list->points.count )
                return command_result;
            ai_command_point_definition *grenade_point = &points[grenade_point_index];
            unit_add_grenade_type_to_inventory(actor->meta.unit_index, grenade_type, 1);
            complex_control->grenade_throw_started = 0;
            complex_control->grenade_throw_depress_trigger = 0;
            complex_control->grenade_target.n[0] = grenade_point->position.n[0];
            complex_control->grenade_target.n[1] = grenade_point->position.n[1];
            float grenade_target_z = grenade_point->position.n[2];
            complex_control->grenade_throw_trajectory_type = 0;
            complex_control->grenade_target.n[2] = grenade_target_z;
            int16_t trajectory_type = command->atom_modifier;
            if ( (unsigned int)trajectory_type <= 2 )
                complex_control->grenade_throw_trajectory_type = trajectory_type;
            simple_control->pause_timer = 60;
            command_result = 1;
            break;
        }

        case _ai_atom_vehicle:   /* enter the nearest vehicle */
        {
            if ( unit_index != actor->meta.unit_index )
                return command_result;
            int16_t seat_desire_type = -1;
            uint16_t candidate_count = 0;
            /* DEVIATION: decompiler flattened the 16x 8-byte candidate records into a punned char[160];
             * untangled into the typed record array (see vehicle_possibility.h). Extra 32 bytes of the
             * original frame slot were never written. */
            vehicle_possibility vehicle_candidates[16];
            object_iterator vehicle_cursor;
            object_iterator_new(&vehicle_cursor, object_mask_vehicle, 0);
            if ( object_iterator_next(&vehicle_cursor) )
            {
                float *actor_position = &actor->input.position.body_position.n[0];   /* actor body position */
                do
                {
                    real_point3d vehicle_origin;
                    object_get_origin(vehicle_cursor.index, &vehicle_origin);
                    float delta_x = vehicle_origin.n[0] - actor_position[0];
                    float delta_z = vehicle_origin.n[2] - actor_position[2];
                    float delta_y = vehicle_origin.n[1] - actor_position[1];
                    float distance_sq = delta_x * delta_x + (delta_z * delta_z + delta_y * delta_y);
                    float search_radius = command->parameter1;
                    if ( search_radius == 0.0 || distance_sq < (double)(search_radius * search_radius) )
                    {
                        int slot = (int16_t)candidate_count++;
                        vehicle_candidates[slot].distance_squared = distance_sq;
                        vehicle_candidates[slot].vehicle_index = vehicle_cursor.index;
                        if ( candidate_count >= 0x10u )
                            break;
                    }
                }
                while ( object_iterator_next(&vehicle_cursor) );
            }
            int candidate_total = (int16_t)candidate_count;
            if ( (int16_t)candidate_count > 1 )
                qsort(vehicle_candidates, (int16_t)candidate_count, sizeof(vehicle_possibility), vehicle_possibility_qsort);
            if ( (unsigned int)command->atom_modifier <= 4 )
                seat_desire_type = command->atom_modifier;
            if ( (int16_t)candidate_count <= 0 )
                return command_result;
            int candidate_index = 0;
            while ( !actor_action_try_to_enter_vehicle(
                        actor_index,
                        vehicle_candidates[candidate_index].vehicle_index,
                        "",   /* any seat (0x8200155A empty string) */
                        seat_desire_type,
                        0,
                        nullptr) )
            {
                candidate_index = (int16_t)(candidate_index + 1);
                if ( candidate_index >= candidate_total )
                    return 0;
            }
            simple_control->metadata_flags |= (1u << _obey_metadata_action_changed_bit);
            command_result = 1;
            break;
        }

        case _ai_atom_running_jump:   /* board / mount */
        {
            if ( unit_index == actor->meta.unit_index && actor->input.vehicle_index != -1 )
                return command_result;
            /* 0xE3 = ~0x1C: clear the three jump-state bits, then latch an UNtargeted jump */
            simple_control->simple_control_flags =
                (simple_control->simple_control_flags
                 & ~((1u << _obey_simple_jump_bit)
                   | (1u << _obey_simple_jump_jumped_bit)
                   | (1u << _obey_simple_jump_targeted_bit)))
                | (1u << _obey_simple_jump_bit);
            unsigned char face_forward;
            if ( unit_index == actor->meta.unit_index )
            {
                face_forward = (actor->control.moving != 0 && actor->control.moving_facing_direction == 0) ? 1 : 0;
            }
            else
            {
                object_datum *unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
                if ( unit_object->object.parent_object_index != -1 )
                {
                    face_forward = 0;
                }
                else
                {
                    /* dot(forward, translational_velocity) — despite the original local's name, this
                     * tests "already moving roughly the way it's facing," not anything about being upright. */
                    float upright_dot =
                        unit_object->object.forward.n[0] * unit_object->object.translational_velocity.n[0]
                      + unit_object->object.forward.n[2] * unit_object->object.translational_velocity.n[2]
                      + unit_object->object.forward.n[1] * unit_object->object.translational_velocity.n[1];
                    face_forward = 1;
                    if ( upright_dot <= 0.06666667 )
                        face_forward = 0;
                }
            }
            simple_control->pause_timer = 60;
            /* not a flag word: directmovement.facing is the facing-MODE code (0 = base facing,
             * 1 = reversed, 2/3 = perpendicular; see action_obey_directmovement_update_facing).
             * Any value > 3 means "no facing override" — 0xA is that out-of-domain sentinel.
             * No DB enum names this domain (enum_oracle: no candidate above noise; the same
             * domain is adjudicated BLOCKED in actor_move_calculate_movement.c). */
            simple_control->___u5.directmovement.facing = face_forward != 0 ? 0 : 0xA;
            command_result = 1;
            break;
        }

        case _ai_atom_targeted_jump:   /* jump */
        {
            if ( unit_index == actor->meta.unit_index && actor->input.vehicle_index != -1 )
                return command_result;
            unsigned char control_flags = simple_control->simple_control_flags;
            simple_control->___u5.directmovement.facing = 0;
            /* 0xE3 = ~0x1C: clear the three jump-state bits, then latch a TARGETED jump
             * (this atom carries explicit velocity parameters — see obey_simple_control_flags.h). */
            simple_control->simple_control_flags =
                (control_flags
                 & ~((1u << _obey_simple_jump_bit)
                   | (1u << _obey_simple_jump_jumped_bit)
                   | (1u << _obey_simple_jump_targeted_bit)))
                | (1u << _obey_simple_jump_bit)
                | (1u << _obey_simple_jump_targeted_bit);
            simple_control->___u5.directmovement.vector.n[0] = command->parameter1;
            float jump_vertical = command->parameter2;
            simple_control->pause_timer = 60;
            simple_control->___u5.directmovement.vector.n[1] = jump_vertical;
            command_result = 1;
            break;
        }

        case _ai_atom_script:   /* wake an hs script */
        {
            int script_reference_index = command->script_reference_index;
            if ( script_reference_index < 0 || script_reference_index >= global_scenario->ai_script_references.count )
                return command_result;
            return hs_wake_by_name(((ai_script_reference_definition *)global_scenario->ai_script_references.address)[script_reference_index].script_name);
        }

        case _ai_atom_animate:   /* play a user animation */
        {
            int animation_reference_index = command->animation_reference_index;
            if ( animation_reference_index == -1 )
                return command_result;
            ai_animation_reference_definition *animation_reference =
                (ai_animation_reference_definition *)global_scenario->ai_animation_references.address + animation_reference_index;
            int animation_graph_index = animation_reference->animation_graph.index;
            if ( animation_graph_index == -1 )
            {
                /* default to the unit's own animation graph. */
                object_datum *unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
                _object_definition *unit_definition_data = TAG_GET(_object_definition, unit_object->definition_index);
                animation_graph_index = unit_definition_data->animation_graph.index;
            }
            unsigned char loop_animation = 0;
            unsigned char force_sync = 0;
            unsigned char interpolate = 1;
            switch ( (uint16_t)command->atom_modifier )
            {
                case _ai_atom_animate_modifier_absolute_movement: loop_animation = 1; break;
                case _ai_atom_animate_modifier_absolute_movement_no_collision: loop_animation = 1; force_sync = 1; break;
                case _ai_atom_animate_modifier_no_interpolation_relative_movement: interpolate = 0; break;
                case _ai_atom_animate_modifier_no_interpolation_absolute_movement: loop_animation = 1; interpolate = 0; break;
                case _ai_atom_animate_modifier_no_interpolation_absolute_movement_no_collision: loop_animation = 1; force_sync = 1; interpolate = 0; break;
                default: break;   /* _ai_atom_animate_modifier_relative_movement or >=6: no loop, no sync, interpolate */
            }
            /* param 3 is the animation name (char[32] @offset 0 of the reference def); disasm passes the
               struct base directly. Pass the name field so the const char* type is honest. */
            if ( !unit_start_user_animation(unit_index, animation_graph_index, animation_reference->animation_name, interpolate) )
                return command_result;
            /* mask 1u = biped only, so +1228 is biped_datum.biped.flags (DB: _biped_datum.flags @0). */
            biped_datum *animation_biped = object_try_and_get_and_verify_type(unit_index, object_mask_biped);
            if ( animation_biped )
            {
                /* Bit names per biped_datum_flags.h: the loop bit maps onto _biped_absolute_movement_bit
                   and the sync bit onto _biped_no_collision_bit (scripted user animations drive the biped
                   absolutely / non-colliding). */
                unsigned int animation_flags = animation_biped->biped.flags;
                unsigned int with_loop = animation_flags | (1u << _biped_absolute_movement_bit);
                if ( !loop_animation )
                    with_loop = animation_flags & ~(1u << _biped_absolute_movement_bit);
                animation_biped->biped.flags = with_loop;
                unsigned int with_sync = with_loop | (1u << _biped_no_collision_bit);
                if ( !force_sync )
                    with_sync = with_loop & ~(1u << _biped_no_collision_bit);
                animation_biped->biped.flags = with_sync;
            }
            command_result = 1;
            break;
        }

        case _ai_atom_recording:   /* play a recorded animation */
        {
            int recording_reference_index = command->recording_reference_index;
            if ( recording_reference_index < 0 )
                return command_result;
            if ( recording_reference_index >= global_scenario->ai_recording_references.count )
                return command_result;
            int16_t recorded_animation = scenario_get_animation_by_name(
                global_scenario,
                ((ai_recording_reference_definition *)global_scenario->ai_recording_references.address)[recording_reference_index].recording_name);
            if ( recorded_animation == -1 )
                return command_result;
            return recorded_animation_play(unit_index, recorded_animation);
        }

        case _ai_atom_action:   /* play an "action" (communication) animation */
        {
            if ( !complex_control )
                return command_result;
            complex_control->play_action = 0;
            int16_t action_animation_impulse;
            int16_t action_communication_type;
            switch ( (uint16_t)command->atom_modifier )
            {
                case _ai_atom_action_modifier_berserk:
                    complex_control->action_animation_impulse = _unit_animation_impulse_berserk;
                    complex_control->play_action = 1;
                    complex_control->action_communication_type = _ai_communication_berserk;
                    return 1;
                /* recovered: these are unit_animation_impulse ids (DB enum
                 * _E1348C23B6F79F01EF2735D5453AE34B), the argument of actor_move_animation_impulse —
                 * each modifier maps to the impulse of the same name. */
                case _ai_atom_action_modifier_surprise_front: action_animation_impulse = _unit_animation_impulse_surprise_front; action_communication_type = _ai_communication_surprise; break;
                case _ai_atom_action_modifier_surprise_back:  action_animation_impulse = _unit_animation_impulse_surprise_back;  action_communication_type = _ai_communication_surprise; break;
                case _ai_atom_action_modifier_evade_left:     action_animation_impulse = _unit_animation_impulse_evade_left;    action_communication_type = -1; break;
                case _ai_atom_action_modifier_evade_right:    action_animation_impulse = _unit_animation_impulse_evade_right;   action_communication_type = -1; break;
                case _ai_atom_action_modifier_dive_forward:   action_animation_impulse = _unit_animation_impulse_dive_front;    action_communication_type = _ai_communication_dive; break;
                case _ai_atom_action_modifier_dive_back:      action_animation_impulse = _unit_animation_impulse_dive_back;     action_communication_type = _ai_communication_dive; break;
                case _ai_atom_action_modifier_dive_left:      action_animation_impulse = _unit_animation_impulse_dive_left;     action_communication_type = _ai_communication_dive; break;
                case _ai_atom_action_modifier_dive_right:     action_animation_impulse = _unit_animation_impulse_dive_right;    action_communication_type = _ai_communication_dive; break;
                case _ai_atom_action_modifier_vehicle_woohoo: action_animation_impulse = -1; action_communication_type = _ai_communication_vehicle_woohoo; break;
                case _ai_atom_action_modifier_vehicle_scared: action_animation_impulse = -1; action_communication_type = _ai_communication_vehicle_scared; break;
                default: return complex_control->play_action;
            }
            complex_control->action_communication_type = action_communication_type;
            complex_control->action_animation_impulse = action_animation_impulse;
            complex_control->play_action = 1;
            return complex_control->play_action;
        }

        case _ai_atom_vocalize:   /* speak a vocalization */
        {
            int16_t vocalization_type_reference[4];
            int sound_definition_index_reference = -1;
            vocalization_type_reference[0] = command->atom_modifier;
            int16_t speech_result = unit_test_speech(unit_index, _unit_speech_scripted, 1u, 1u, nullptr,
                                                     vocalization_type_reference, &sound_definition_index_reference);
            if ( speech_result <= 0 )
                return command_result;
            unit_speech_item speech_item;
            memset(&speech_item, 0, sizeof(speech_item));
            speech_item.priority = _unit_speech_scripted;
            speech_item.vocalization_type = vocalization_type_reference[0];
            speech_item.sound_definition_index = sound_definition_index_reference;
            ai_communication_packet_new(&speech_item.ai);
            unit_speak(unit_index, speech_result, &speech_item);
            command_result = 1;
            break;
        }

        case _ai_atom_targeting:   /* set / clear a metadata flag */
            if ( (uint16_t)command->atom_modifier )
                simple_control->metadata_flags &= ~1u;
            else
                simple_control->metadata_flags |= 1u;
            command_result = 1;
            break;

        case _ai_atom_initiative:   /* set an actor state byte (allow combat status) */
            if ( unit_index != actor->meta.unit_index )
                return command_result;
            actor->state.action_data.___u0.obey.initiative = (command->atom_modifier == 0);
            command_result = 1;
            break;

        case _ai_atom_wait:   /* no-op */
            command_result = 1;
            break;

        case _ai_atom_loop:   /* jump to another command in the list */
        {
            int target_command_index = command->command_index;
            if ( target_command_index < 0 || target_command_index >= command_count
              || target_command_index == current_command_index )
                return command_result;
            command_result = 1;
            break;
        }

        case _ai_atom_die:   /* set a unit flag word (object_datum.object.damage_flags bits 0x20/0x40) */
        {
            object_datum *unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
            uint16_t new_unit_flags = unit_object->object.damage_flags | (1u << _object_die_act_of_god_silent_bit);
            if ( (uint16_t)command->atom_modifier != 1 )
                new_unit_flags = unit_object->object.damage_flags | (1u << _object_die_act_of_god_bit);
            unit_object->object.damage_flags = new_unit_flags;
            command_result = 1;
            break;
        }

        case _ai_atom_move_immediate:   /* set facing */
        {
            int16_t facing = command->atom_modifier;
            if ( (unsigned int)facing >= 4 )
                simple_control->___u5.directmovement.facing = 0;
            else
                simple_control->___u5.directmovement.facing = facing;
            action_obey_directmovement_update_facing(actor_index, unit_index, simple_control);
            float turn_duration = command->parameter1;
            simple_control->simple_control_flags |= 3u;
            simple_control->pause_timer = (int)(turn_duration * (float)30.0);
            command_result = 1;
            break;
        }

        case _ai_atom_set_radius:   /* set destination radius */
            if ( !complex_control || command->parameter1 <= 0.0 )
                return command_result;
            complex_control->destination_radius_valid = 1;
            complex_control->destination_radius = command->parameter1;
            command_result = 1;
            break;

        case _ai_atom_teleport:   /* teleport the unit to a point, facing a second point */
        {
            int position_point_index = command->point1_index;
            if ( position_point_index < 0 || position_point_index >= command_list->points.count )
                return command_result;
            const real_point3d *position = &points[position_point_index].position;
            real_vector3d facing_vector;
            unit_get_facing_vector(unit_index, &facing_vector);
            int facing_point_index = command->point2_index;
            if ( facing_point_index >= 0 && facing_point_index < command_list->points.count )
            {
                ai_command_point_definition *facing_point = &points[facing_point_index];
                int *unit_object = object_try_and_get_and_verify_type(unit_index, object_mask_biped);
                /* recovered: offset 756 (word 189) of the unit's tag -> biped_definition.biped.flags,
                 * bits 0x4/0x40 = _biped_flying_bit / _biped_climbs_anything_bit. The earlier
                 * "could be a vehicle_definition" hedge was wrong: object_try_and_get_and_verify_type
                 * above filters on object_mask_biped, so the tag here is always a biped_definition. */
                biped_definition *unit_tag = unit_object ? TAG_GET(biped_definition, *unit_object) : nullptr;
                facing_vector.n[0] = facing_point->position.n[0] - position->n[0];
                facing_vector.n[1] = facing_point->position.n[1] - position->n[1];
                facing_vector.n[2] = facing_point->position.n[2] - position->n[2];
                int degenerate;
                /* flying or climb-anything bipeds may legitimately face up/down, so keep Z */
                if ( unit_tag && (unit_tag->biped.flags & ((1u << _biped_flying_bit)
                                                         | (1u << _biped_climbs_anything_bit))) != 0 )
                {
                    degenerate = normalize3d(&facing_vector) == 0.0;
                }
                else
                {
                    facing_vector.n[2] = 0.0;
                    degenerate = normalize2d((real_vector2d *)&facing_vector) == 0.0;
                }
                if ( degenerate )
                    unit_get_facing_vector(unit_index, &facing_vector);
            }
            object_set_position(unit_index, position, &facing_vector, nullptr);
            object_reset(unit_index);
            object_compute_node_matrices_recursive(unit_index);
            int actor_unit_index = actor->meta.unit_index;
            if ( unit_index == actor_unit_index )
            {
                actor_input_sample_position(actor_index, actor_unit_index, &actor->input.position);
                actor_move_halt(actor_index);
            }
            command_result = 1;
            break;
        }

        default:
            return command_result;
    }

    return command_result;
}
