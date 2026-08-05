/* biped_vehicle_speech @0x837AE31C — make an AI biped riding a "speechful" vehicle (definition flag
 * 0x40 at def word 95) shout during airborne jumps. Gated on: the rider has an actor (+500), is not in
 * the exiting state (29), its mount timer (+802) exceeds 120, the vehicle is moving fast enough
 * (+1232 > 30), and a 15-tick speech throttle (+1272) has elapsed. It probes straight down for ground
 * within 8 units; if none (the vehicle is airborne) it predicts the landing trajectory (velocity*60
 * minus a gravity term) and probes along it. The chosen communication line is: 40 when there is no
 * good landing (steep/none), else 39 when the vehicle is tilted (up.z <= 0.6) or spinning
 * (angular speed >= 3 deg), else 38.
 *
 * DEVIATION: both biped_find_ground_surface calls are mis-rendered by the FPR-shadow ABI (the float
 * ground_distance reserves r4). Corrected from disasm per the DB prototype
 * (biped_index, ground_distance, ground_direction, surface_point, surface_normal):
 *   call 1: (biped_index, 8.0, global_down3d, nullptr, nullptr)
 *   call 2: (biped_index, 8.0, &predicted_direction, nullptr, &landing_normal). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_definition_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/unit_animation_state.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"
#include "headers/ai_information_data.h"
extern float __fsqrts(float);

struct ai_information_data;


extern int game_time_get(void);
extern int biped_find_ground_surface(int biped_index, float ground_distance, const real_vector3d *ground_direction, real_point3d *surface_point, real_vector3d *surface_normal);
extern float normalize3d(real_vector3d *v);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

void biped_vehicle_speech(int biped_index)
{
    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    vehicle_datum *vehicle = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
        biped->object.parent_object_index)->datum;

    if (((TAG_GET(unit_definition, vehicle->definition_index)->unit.flags) & (1u << _unit_definition_causes_passenger_dialogue_bit)) == 0
        || biped->unit.actor_index == -1
        || (uint8_t)biped->unit.animation.state == _unit_state_ai_impulse
        || biped->unit.gunner_inactive_ticks <= 120)
        return;

    /* +1232 is vehicle.airborne_ticks (DB rel 4), not a speed */
    if (vehicle->vehicle.airborne_ticks <= 0x1Eu)
        return;

    int now = game_time_get();
    int last_speech = biped->biped.last_falling_communication_time;
    if (last_speech != -1 && last_speech + 15 >= now)
        return;
    biped->biped.last_falling_communication_time = now;

    bool will_land_badly = false;
    if (biped_find_ground_surface(biped_index, 8.0f, global_down3d, nullptr, nullptr) == -1)
    {
        will_land_badly = true;
        real_point3d predicted_direction;
        predicted_direction.n[0] = vehicle->object.translational_velocity.n[0] * 60.0f;
        predicted_direction.n[1] = vehicle->object.translational_velocity.n[1] * 60.0f;
        predicted_direction.n[2] = vehicle->object.translational_velocity.n[2] * 60.0f - global_gravity * 1800.0f;
        if (normalize3d((real_vector3d *)&predicted_direction) > 0.0f)
        {
            real_vector3d landing_normal;
            if (biped_find_ground_surface(biped_index, 8.0f, (real_vector3d *)&predicted_direction,
                                          nullptr, &landing_normal) != -1)
                will_land_badly = landing_normal.n[2] <= 0.30000001f;
        }
    }

    int16_t communication;
    if (will_land_badly)
    {
        communication = _ai_communication_vehicle_falling;
    }
    else if (vehicle->object.up.n[2] <= 0.60000002f)
    {
        communication = _ai_communication_vehicle_scared;
    }
    else
    {
        communication = _ai_communication_vehicle_woohoo;
        float angular_speed = __fsqrts(vehicle->object.angular_velocity.n[1] * vehicle->object.angular_velocity.n[1]
                                       + (vehicle->object.angular_velocity.n[2] * vehicle->object.angular_velocity.n[2]
                                          + vehicle->object.angular_velocity.n[0] * vehicle->object.angular_velocity.n[0]));
        if (angular_speed >= 0.052359879f)
            communication = _ai_communication_vehicle_scared;
    }

    ai_communication_event(communication, biped_index, -1, -1, -1, -1, nullptr);
}
