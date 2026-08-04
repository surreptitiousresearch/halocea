/* create_crashing_effects @0x8375FE54 — when a vehicle's velocity changes abruptly (a crash/landing),
 * spawn the crash damage effect and impulse sound. The impact speed is the magnitude of the velocity
 * delta (current object velocity minus the velocity before the physics step). Effects only fire if the
 * speed exceeds a small threshold AND at least one of the collision model's mass points is flagged as a
 * ground/crash contact. The damage and sound scale linearly with how far the speed exceeds the threshold.
 *
 * The damage effect comes from the global falling-damage table; the sound from the vehicle definition. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_globals_falling_damage.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/game_globals_tag.h"
#include "headers/global_tag_instances.h"
#include "headers/vehicle_definition.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_datum.h"
#include "headers/damage_data.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"
#include "headers/real_point3d.h"
extern float __fsqrts(float);


extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);

void create_crashing_effects(int vehicle_index, const real_vector3d *old_velocity, mass_point_datum *mass_points)
{
    object_datum *vehicle = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    int crash_damage_index = ((game_globals_falling_damage *)global_game_globals->falling_damage.address)->vehicle_hit_environment_damage_effect.index;
    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle->definition_index);
    int crash_sound_index = vehicle_def->vehicle.crash_sound.index; /* +0x3CC */

    if (crash_damage_index == -1 && crash_sound_index == -1)
        return;

    real_vector3d velocity_delta;
    velocity_delta.n[0] = vehicle->object.translational_velocity.n[0] - old_velocity->n[0];
    velocity_delta.n[2] = vehicle->object.translational_velocity.n[2] - old_velocity->n[2];
    velocity_delta.n[1] = vehicle->object.translational_velocity.n[1] - old_velocity->n[1];

    float impact_speed = __fsqrts(velocity_delta.n[1] * velocity_delta.n[1]
                       + (velocity_delta.n[2] * velocity_delta.n[2] + velocity_delta.n[0] * velocity_delta.n[0]));
    if (impact_speed <= 0.02f)
        return;

    /* def+140 is object.physics.index (a physics tag, not the collision model — earlier comment corrected) */
    int mass_point_count = TAG_GET(physics_definition, vehicle_def->object.physics.index)->mass_points.count;
    unsigned __int8 found_ground_contact = 0;
    if (mass_point_count > 0)
    {
        int i = 0;
        while ((mass_points[i].flags & 2) == 0)
        {
            i = (__int16)(i + 1);
            if (i >= mass_point_count)
                goto done_scan;
        }
        found_ground_contact = 1;
    }
done_scan:
    if (!found_ground_contact)
        return;

    float scale = (impact_speed - 0.02f) * 45.454544f;

    if (crash_damage_index != -1)
    {
        damage_data damage;
        damage_data_new(&damage, crash_damage_index);
        if (scale >= 0.0f)
            damage.scale = (scale <= 1.0f) ? scale : 1.0f;
        else
            damage.scale = 0.0f;

        damage.origin.n[0] = vehicle->object.bounding_sphere_center.n[0];
        damage.origin.n[1] = vehicle->object.bounding_sphere_center.n[1];
        damage.direction = velocity_delta;
        damage.origin.n[2] = vehicle->object.bounding_sphere_center.n[2];
        object_cause_damage(&damage, vehicle_index, -1, -1, -1, nullptr);
    }

    if (crash_sound_index != -1)
    {
        float sound_scale;
        if (scale >= 0.0f)
            sound_scale = (scale <= 1.0f) ? scale : 1.0f;
        else
            sound_scale = 0.0f;
        object_impulse_sound_new(vehicle_index, crash_sound_index, -1, global_origin3d, global_forward3d,
                                 sound_scale, found_ground_contact);
    }
}
