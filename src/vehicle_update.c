/* vehicle_update @ 0x83761F70 — per-tick update for a vehicle object. Handles netgame vehicle respawn,
 * the parked/seated short-circuit, parking-brake and airborne flag bookkeeping, the steering-angle
 * solve, the airborne self-righting torque, the speed/steering seek integrators, and finally the
 * per-vehicle-type physics dispatch (tank/jeep/boat/plane/scout/fighter/generic) with suspension,
 * slip/crash effects, vehicle floor/ceiling clamping and falling damage. Returns 1.
 *
 * Vehicle object fields are accessed by raw offset (matching unit_update.c / biped_update.c), since the
 * runtime object/unit struct is kept opaque project-wide. Key offsets: +16 object flags, +92..+100
 * position, +104..+112 linear velocity, +116..+124 orientation forward, +128..+136 orientation up,
 * +140..+148 angular velocity, +548..+556 desired/movement forward, +1228 vehicle flags,
 * +1236 throttle, +1240 yaw rate, +1244 steering position, +1452 respawn timer, +1456 spawn datum index.
 *
 * DEVIATIONS (all verified against disassembly at 0x837626D4-0x83762850):
 *  - The atan2 steering args are rendered as long doubles by the decompiler (PPC double-double ABI
 *    artifact). They are reconstructed here in single precision: x = forward.desired_forward,
 *    y = (up x forward).desired_forward.
 *  - The clamp at 0x83762568 is an inline `fsel`: desired = (throttle >= 0) ? steering_angle : -steering_angle.
 *  - update_human_plane_physics, create_slipping_effects and compute_airborne_ticks take a single
 *    buffer argument; the decompiler invented a second buffer pointer from a stale register. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/tag_block.h"
#include "headers/scenario.h"
#include "headers/structure_bsp.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_falling_damage.h"
#include "headers/game_variant.h"
#include "headers/physics_variable_speed.h"
#include "headers/physics_variable_position.h"
#include "headers/mass_point_datum.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/damage_data.h"
#include "headers/unit_animation_update_data.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/physics_definition.h"
#include "headers/scenario_object_datum.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/real_euler_angles3d.h"

#include <math.h>
#include <stddef.h>
#include "headers/game_engine_type.h"
#include "headers/game_connection.h"
#include "headers/object_flags.h"
#include "headers/vehicle_definition_flags.h"
#include "headers/vehicle_type.h"
#include "headers/vehicle_flags.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"
#include "headers/networked_datum_role.h"
#include "headers/tag_groups.h"


extern int16_t game_connection(void);
extern game_variant *game_engine_get_variant(void);
extern int game_time_get(void);
extern void object_reset(int object_index);
extern tag_block *scenario_get_object_type_scenario_datums(scenario *scenario, int16_t object_type, int *size);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern uint8_t unit_driver_seat_is_filled(int unit_index);
extern int physics_variable_speed_update_seek(float *speed, const physics_variable_speed *definition, float desired_speed, float magnitude);
extern int physics_variable_position_update_seek(float *position, const physics_variable_position *definition, uint8_t cyclical_position, float desired_position, float speed);
extern void slowly_stop_vehicle(int vehicle_index);
extern uint8_t update_suspension(int vehicle_index);
extern void update_human_tank_physics(int vehicle_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points);
extern void update_human_jeep_physics(int vehicle_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points);
extern void update_human_boat_physics(int vehicle_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points);
extern void update_human_plane_physics(int vehicle_index, mass_point_datum *mass_points);
extern void update_alien_scout_physics(int vehicle_index, float steering, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points);
extern void update_alien_fighter_physics_new(int vehicle_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points);
extern void update_alien_fighter_physics_old(int vehicle_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points);
extern void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);
extern void create_ghost_effect(int vehicle_index);
extern void create_slipping_effects(int vehicle_index, mass_point_datum *mass_points);
extern void create_crashing_effects(int vehicle_index, const real_vector3d *old_velocity, mass_point_datum *mass_points);
extern void compute_airborne_ticks(int vehicle_index, mass_point_datum *mass_points);
extern int16_t unit_update_animation(int unit_index, unit_animation_update_data *data);
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);
extern void object_permute_region(int object_index, const char *permutation_name, int16_t desired_region_index, uint8_t active_flag);

uint8_t vehicle_update(int vehicle_index)
{
    vehicle_datum *vehicle = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    vehicle_definition *definition = TAG_GET(vehicle_definition, vehicle->definition_index);

    /* netgame vehicle respawn: when this vehicle has drifted from its spawn point, reset it */
    if ( game_connection() == _game_connection_network_server && vehicle->vehicle.last_controlled_time != -1 )
    {
        game_variant *variant = game_engine_get_variant();
        if ( variant->universal_variant.vehicles_respawn )
        {
            if ( game_time_get() >= vehicle->vehicle.last_controlled_time + variant->universal_variant.vehicles_respawn )
            {
                float drift_x = vehicle->vehicle.spawn_position.n[1] - vehicle->object.position.n[1];
                float drift_y = vehicle->vehicle.spawn_position.n[2] - vehicle->object.position.n[2];
                float drift_z = vehicle->vehicle.spawn_position.n[0] - vehicle->object.position.n[0];
                if ( sqrtf(drift_z * drift_z + (drift_y * drift_y + drift_x * drift_x)) > 1.5f
                    && ((vehicle->object.flags >> _object_at_rest_bit) & 1) == 1 )
                {
                    scenario *scn = global_scenario;
                    if ( scn )
                    {
                        const real_point3d *spawn_position;
                        real_vector3d spawn_forward;
                        int element_size = 0;
                        object_reset(vehicle_index);
                        if ( variant->game_engine_index == game_engine_race )
                        {
                            /* CTF/flag-style: spawn datum lives in the scenario netgame flags block */
                            scenario_netgame_flag *datum = &((scenario_netgame_flag *)scn->netgame_flags.address)[vehicle->vehicle.___u27.vehicle_scenario_datum_index];
                            vector3d_from_angle(&spawn_forward, datum->facing);
                            spawn_position = &datum->position;
                        }
                        else
                        {
                            const tag_block *datums = scenario_get_object_type_scenario_datums(scn, 1, &element_size);
                            scenario_object_datum *element = (scenario_object_datum *)tag_block_get_element_with_size(datums, vehicle->vehicle.___u27.vehicle_scenario_datum_index, element_size);
                            vector3d_from_angle(&spawn_forward, element->rotation.n[0]);
                            spawn_position = &element->position;
                        }
                        object_set_position(vehicle_index, spawn_position, &spawn_forward, global_up3d);
                        int flags = vehicle->object.flags;
                        if ( definition->object.physics.index != -1 )
                            vehicle->object.flags = flags & ~(1u << _object_at_rest_bit);
                        else
                            vehicle->object.flags = flags | (1u << _object_at_rest_bit);
                        if ( definition->object.physics.index != -1 )
                            vehicle->object.position.n[2] += definition->object.bounding_radius * 0.5f;
                    }
                }
                vehicle->vehicle.last_controlled_time = game_time_get();
            }
        }
    }

    /* seated/parked: zero velocities, clear the moving flag, skip physics */
    if ( vehicle->object.parent_object_index != -1 )
    {
        vehicle->object.angular_velocity.n[0] = 0.0f;
        vehicle->object.angular_velocity.n[1] = 0.0f;
        vehicle->object.angular_velocity.n[2] = 0.0f;
        vehicle->object.translational_velocity.n[0] = 0.0f;
        vehicle->object.translational_velocity.n[1] = 0.0f;
        vehicle->object.translational_velocity.n[2] = 0.0f;
        vehicle->object.flags &= ~(1u << _object_at_rest_bit);
        goto animate;
    }

    /* parking brake (_vehicle_control_crouch_bit in +1228) tracks control bit 0x1 in +520 */
    int control = vehicle->unit.control_flags;
    if ( (control & 1) != 0 )
        vehicle->vehicle.flags |= (1u << _vehicle_control_crouch_bit);
    else
        vehicle->vehicle.flags &= ~(1u << _vehicle_control_crouch_bit);

    /* brake/decelerating (_vehicle_control_jump_bit): handbrake control bit 0x2, or throttle opposing current speed */
    float throttle_input = vehicle->unit.throttle.n[0];
    if ( (control & 2) != 0
        || (definition->vehicle.flags & (1u << _vehicle_control_opposite_speed_sets_jump_bit)) != 0
            && (throttle_input > 0.0f && vehicle->vehicle.speed < 0.0f
                || throttle_input < 0.0f && vehicle->vehicle.speed > 0.0f) )
        vehicle->vehicle.flags |= (1u << _vehicle_control_jump_bit);
    else
        vehicle->vehicle.flags &= ~(1u << _vehicle_control_jump_bit);

    /* steering angle = signed yaw error of the desired-forward against the chassis frame.
     * x = forward . desired_forward; y = (up x forward) . desired_forward. */
    float forward_x = vehicle->object.forward.n[0], forward_y = vehicle->object.forward.n[1], forward_z = vehicle->object.forward.n[2];
    float up_x = vehicle->object.up.n[0], up_y = vehicle->object.up.n[1], up_z = vehicle->object.up.n[2];
    float desired_x = vehicle->unit.desired_facing_vector.n[0], desired_y = vehicle->unit.desired_facing_vector.n[1], desired_z = vehicle->unit.desired_facing_vector.n[2];
    float steer_dot = desired_x * forward_x + (forward_y * desired_y + forward_z * desired_z);
    float steer_cross = (forward_z * up_y - forward_y * up_z) * desired_x
                      + (forward_x * up_z - forward_z * up_x) * desired_y
                      + (up_x * forward_y - forward_x * up_y) * desired_z;
    float steering_angle = (float)atan2(steer_cross, steer_dot);

    int simulation_state = (int)vehicle->object.datum_role;
    if ( (simulation_state == _networked_datum_puppet_controlled_by_local_player || simulation_state == _networked_datum_puppet)
        && vehicle->object.is_server_position_valid == 1 && vehicle->object.parent_object_index == -1 )
        unit_driver_seat_is_filled(vehicle_index);

    /* airborne self-righting torque (_vehicle_upending_bit in +1228, active righting axis at +1233) */
    int16_t vehicle_flags = vehicle->vehicle.flags;
    if ( (vehicle_flags & (1u << _vehicle_upending_bit)) != 0
        && vehicle->vehicle.upending_type
        && vehicle->vehicle.upending_ticks < 0x1Eu
        && vehicle->object.up.n[2] <= 0.89999998f )
    {
        uint8_t righting_axis = vehicle->vehicle.upending_type;
        float torque_sign = (righting_axis == 2 || righting_axis == 4) ? 0.3f : -0.3f;

        /* torque components, stored to +140/+144/+148 respectively */
        float torque_a, torque_b, torque_c;
        if ( righting_axis == 4 || righting_axis == 3 )
        {
            torque_a = up_z * forward_y - up_y * forward_z;   /* (up x forward) components */
            torque_b = forward_z * up_x - up_z * forward_x;
            torque_c = up_y * forward_x - forward_y * up_x;
        }
        else
        {
            torque_a = forward_x;
            torque_b = forward_y;
            torque_c = forward_z;
        }

        float roll = vehicle->object.up.n[2] * -2.0f;
        float roll_min = definition->vehicle.flipping_angular_velocity_min;
        if ( roll < roll_min || (roll_min = definition->vehicle.flipping_angular_velocity_max, roll > roll_min) )
            roll = roll_min;
        float torque = roll * torque_sign;

        vehicle->object.flags &= ~(1u << _object_at_rest_bit);

        if ( righting_axis == 2 || righting_axis == 1 )
        {
            float negate = -vehicle->object.forward.n[2];
            torque_a += negate * (up_z * forward_y - up_y * forward_z);
            torque_c += (up_y * forward_x - forward_y * up_x) * negate;
            torque_b += (forward_z * up_x - up_z * forward_x) * negate;
        }

        vehicle->object.angular_velocity.n[0] = torque_a * torque;
        vehicle->object.angular_velocity.n[1] = torque_b * torque;
        vehicle->object.angular_velocity.n[2] = torque_c * torque;

        if ( definition->vehicle.type )
        {
            if ( definition->vehicle.type == _vehicle_type_alien_fighter )
            {
                /* alien fighter: bleed a small downward velocity while righting */
                float velocity_z = vehicle->object.translational_velocity.n[2];
                if ( velocity_z >= -0.0099999998f )
                    velocity_z = -0.0099999998f;
                vehicle->object.translational_velocity.n[2] = velocity_z;
                ++vehicle->vehicle.upending_ticks;
                goto seek_speed;
            }
        }
        else
        {
            /* project linear velocity onto the chassis forward axis */
            float along_forward = forward_y * vehicle->object.translational_velocity.n[1]
                + (vehicle->object.translational_velocity.n[0] * forward_x + forward_z * vehicle->object.translational_velocity.n[2]);
            vehicle->object.translational_velocity.n[0] = along_forward * forward_x;
            vehicle->object.translational_velocity.n[1] = forward_y * along_forward;
            vehicle->object.translational_velocity.n[2] = forward_z * along_forward;
        }
        ++vehicle->vehicle.upending_ticks;
    }
    else
    {
        vehicle->vehicle.upending_ticks = 0;
        vehicle->vehicle.upending_type = 0;
        vehicle->vehicle.flags = vehicle_flags & ~(1u << _vehicle_upending_bit);
    }

seek_speed:
    {
        float *throttle = &vehicle->vehicle.speed;
        const physics_variable_speed *speed_def = &definition->vehicle.speed;
        float *seek_value = &vehicle->vehicle.speed;
        float seek_target;
        float seek_scale = 1.0f;
        if ( (vehicle->vehicle.flags & (1u << _vehicle_control_jump_bit)) != 0 )
        {
            seek_target = 0.0f;
        }
        else
        {
            physics_variable_speed_update_seek(&vehicle->vehicle.speed, &definition->vehicle.speed, vehicle->unit.throttle.n[0], 1.0f);
            seek_target = vehicle->unit.throttle.n[1];
            speed_def = &definition->vehicle.slide;
            seek_value = &vehicle->vehicle.slide;
        }
        physics_variable_speed_update_seek(seek_value, speed_def, seek_target, seek_scale);

        float *steering_position = &vehicle->vehicle.turn;
        if ( definition->vehicle.type )
        {
            const physics_variable_position *turn = &definition->vehicle.turn;
            float desired_position = (*throttle >= 0.0f) ? steering_angle : -steering_angle;
            float limit = turn->minimum_position * DEG_TO_RAD;   /* recovered: definition->vehicle.turn.minimum_position -> turn->minimum_position */
            if ( desired_position < limit || (limit = turn->maximum_position * DEG_TO_RAD, desired_position > limit) )   /* recovered: definition->vehicle.turn.maximum_position -> turn->maximum_position */
                desired_position = limit;
            physics_variable_position_update_seek(
                &vehicle->vehicle.turn,
                turn,
                0,
                desired_position,
                definition->vehicle.turn_rate * DEG_TO_RAD * SECONDS_PER_TICK);
        }
        else
        {
            float target;
            float scale;
            if ( *throttle == 0.0f )
            {
                scale = 1.0f;
                target = 0.0f;
            }
            else
            {
                float normalized = steering_angle * 0.63661975f;   /* 2/pi */
                if ( normalized >= -1.0f )
                {
                    if ( normalized > 1.0f )
                        normalized = 1.0f;
                }
                else
                {
                    normalized = -1.0f;
                }
                target = normalized * definition->vehicle.speed.maximum_forward_speed;
                scale = 2.0f;
            }
            physics_variable_speed_update_seek(&vehicle->vehicle.turn, &definition->vehicle.speed, target, scale);
        }

        /* if the vehicle is being driven and any control is engaged, clear the parked flag */
        if ( definition->object.physics.index != -1 )
        {
            int capabilities = definition->vehicle.flags;
            if ( (capabilities & (1u << _vehicle_speed_wakes_physics_bit)) != 0 && *throttle != 0.0f
                || (capabilities & (1u << _vehicle_turn_wakes_physics_bit)) != 0 && *steering_position != 0.0f
                || (capabilities & (1u << _vehicle_driver_power_wakes_physics_bit)) != 0 && vehicle->unit.seat_power[0] != 0.0f
                || (capabilities & (1u << _vehicle_gunner_power_wakes_physics_bit)) != 0 && vehicle->unit.seat_power[1] != 0.0f
                || (capabilities & (1u << _vehicle_slide_wakes_physics_bit)) != 0 && vehicle->vehicle.slide != 0.0f )
            {
                vehicle->object.flags &= ~(1u << _object_at_rest_bit);
            }
        }

        if ( definition->object.physics.index == -1 || (vehicle->object.flags & (1u << _object_at_rest_bit)) != 0 )
        {
            /* parked: only coast the wheels to a stop */
            if ( vehicle->vehicle.stop_time > 0 )
            {
                slowly_stop_vehicle(vehicle_index);
                update_suspension(vehicle_index);
            }
        }
        else
        {
            real_vector3d old_velocity;
            old_velocity.n[0] = vehicle->object.translational_velocity.n[0];
            old_velocity.n[1] = vehicle->object.translational_velocity.n[1];
            old_velocity.n[2] = vehicle->object.translational_velocity.n[2];

            /* DEVIATION: both scratch arrays were reconstructed with the wrong element type and
             * far too few elements. The frame (stwu 0x3340) places powered_mass_points at r1+0xD0
             * and mass_points at r1+0xCD0, with the saved FPRs starting at r1+0x32D0:
             *   0xCD0 - 0xD0   = 0xC00  = 3072 = 32 * sizeof(powered_mass_point_datum) (96)
             *   0x32D0 - 0xCD0 = 0x2600 = 9728 = 32 * sizeof(mass_point_datum)        (304)
             * so each is a 32-element array, and the powered array really is
             * powered_mass_point_datum — which is also what all six physics updaters take, so the
             * casts below are gone. (The r1+0x340/0x1340/0x2340 references are __chkstk page
             * probes for this 13 KB frame, not locals.) */
            powered_mass_point_datum powered_mass_points[32];
            mass_point_datum         mass_points[32];
            unsigned int vehicle_type = definition->vehicle.type;
            if ( vehicle_type <= _vehicle_type_turret )
            {
                switch ( vehicle_type )
                {
                    case _vehicle_type_human_jeep:
                        update_human_jeep_physics(vehicle_index, powered_mass_points, mass_points);
                        break;
                    case _vehicle_type_human_boat:
                        update_human_boat_physics(vehicle_index, powered_mass_points, mass_points);
                        break;
                    case _vehicle_type_human_plane:
                        update_human_plane_physics(vehicle_index, mass_points);
                        break;
                    case _vehicle_type_alien_scout:
                        update_alien_scout_physics(vehicle_index, steering_angle, powered_mass_points, mass_points);
                        break;
                    case _vehicle_type_alien_fighter:
                    {
                        /* alien fighter: the referenced physics tag's radius selects the new/old model */
                        physics_definition *chassis_physics =
                            TAG_GET(physics_definition, definition->object.physics.index);
                        if ( chassis_physics->radius <= 0.0f )
                            update_alien_fighter_physics_new(vehicle_index, powered_mass_points, mass_points);
                        else
                            update_alien_fighter_physics_old(vehicle_index, powered_mass_points, mass_points);
                    }
                        create_ghost_effect(vehicle_index);
                        break;
                    default: /* human_tank (0) or turret (6) */
                        if ( vehicle_type )
                            physics_update(vehicle_index, NULL, mass_points, NULL, NULL);
                        else
                            update_human_tank_physics(vehicle_index, powered_mass_points, mass_points);
                        break;
                }
            }

            if ( !g_running_simulated_update )
                create_slipping_effects(vehicle_index, mass_points);
            if ( !update_suspension(vehicle_index) && !g_running_simulated_update )
                create_crashing_effects(vehicle_index, &old_velocity, mass_points);
            compute_airborne_ticks(vehicle_index, mass_points);

            int object_flags = vehicle->object.flags;
            if ( (object_flags & (1u << _object_at_rest_bit)) != 0 )
                vehicle->vehicle.stop_time = 15;

            /* vehicle floor/ceiling soft barrier for planes/alien_fighters (the flying vehicle types) */
            if ( (object_flags & (1u << _object_no_collisions_bit)) == 0
              && (definition->vehicle.type == _vehicle_type_human_plane
                  || definition->vehicle.type == _vehicle_type_alien_fighter) )
            {
                float floor = global_structure_bsp->vehicle_floor;
                float ceiling = global_structure_bsp->vehicle_ceiling;
                if ( floor != 0.0f && vehicle->object.position.n[2] < floor )
                    vehicle->object.translational_velocity.n[2] += ((floor - vehicle->object.position.n[2]) * 0.015625f
                        - vehicle->object.translational_velocity.n[2] * 0.0625f) * vehicle->unit.seat_power[0];
                if ( ceiling != 0.0f && vehicle->object.position.n[2] > ceiling )
                    vehicle->object.translational_velocity.n[2] = -((( vehicle->object.position.n[2] - ceiling) * 0.015625f
                        + vehicle->object.translational_velocity.n[2] * 0.0625f) * vehicle->unit.seat_power[0]) - vehicle->object.translational_velocity.n[2];
            }
        }
    }

    /* falling damage to seated occupants */
    if ( (definition->vehicle.flags & (1u << _vehicle_kills_riders_at_terminal_velocity_bit)) != 0 && !g_running_simulated_update )
    {
        game_globals_falling_damage *falling_damage = (game_globals_falling_damage *)global_game_globals->falling_damage.address;
        if ( vehicle->object.translational_velocity.n[2] < -falling_damage->runtime_maximum_falling_velocity )
        {
            object_datum *seat_object = NULL;
            for ( int seat = vehicle->object.first_child_object_index; seat != -1; seat = seat_object->object.next_object_index )
            {
                damage_data damage;
                seat_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, seat)->datum;
                damage_data_new(&damage, falling_damage->maximum_distance_damage.index);
                object_cause_damage(&damage, seat, -1, -1, -1, NULL);
            }
        }
    }

animate:
    if ( definition->object.animation_graph.index != -1 )
    {
        unit_animation_update_data animation;
        /* recovered: animation.opaque = 0 -> zero both members (2-byte struct) */
        animation.state_desired = 0;
        animation.crouching = 0;
        unit_update_animation(vehicle_index, &animation);
    }

    /* motion-blur region toggle keyed on yaw rate vs threshold */
    uint8_t blur = fabsf(vehicle->vehicle.speed) >= definition->vehicle.blur_speed;
    if ( blur == (vehicle->vehicle.flags & (1u << _vehicle_blurred_bit)) )
        return 1;
    object_permute_region(vehicle_index, "~blur", -1, blur);
    if ( blur )
        vehicle->vehicle.flags |= (1u << _vehicle_blurred_bit);
    else
        vehicle->vehicle.flags &= ~(1u << _vehicle_blurred_bit);
    return 1;
}
