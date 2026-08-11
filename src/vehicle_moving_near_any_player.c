/* vehicle_moving_near_any_player @0x83760BE8 — return true if any vehicle is moving and close to an
 * on-foot player. First it snapshots every local player who is on foot (their unit's
 * object.parent_object_index at +284 is -1), recording the unit index and bounding-sphere center. Then it
 * walks every vehicle object: for each recorded player (skipping the vehicle that player is riding) it
 * tests whether the vehicle is within 10 world units (distance^2 < 100) and moving faster than 1/30
 * u/tick (speed^2 >= (1/30)^2). The first such vehicle makes the function return 1; otherwise it
 * returns 0.
 *
 * DEVIATION: the decompiler packed the per-player index/position scratch arrays into the object_iterator
 * stack array (v19 / &v20[1]+) and under-sized object_iterator to [2]; these are reproduced as explicit
 * local arrays since the iterator only ever touches its own struct. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_iterator.h"
#include "headers/player_datum.h"
#include "headers/real_point3d.h"
#include "headers/unit_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern int16_t local_player_get_next(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);

uint8_t vehicle_moving_near_any_player(void)
{
    int player_unit_indices[4];
    real_point3d player_positions[4];
    int16_t player_count = 0;
    char none_found = 1;

    for (int16_t i = local_player_get_next(-1); i != -1; i = local_player_get_next(i))
    {
        int player_index = local_player_get_player_index(i);
        if (player_index == -1)
            continue;
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        int unit_index = player->unit_index;
        if (unit_index == -1)
            continue;
        unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        if (unit->object.parent_object_index == -1) /* on foot: not riding a vehicle */
        {
            player_unit_indices[player_count] = unit_index;
            player_positions[player_count].n[0] = unit->object.bounding_sphere_center.n[0];
            player_positions[player_count].n[1] = unit->object.bounding_sphere_center.n[1];
            player_positions[player_count].n[2] = unit->object.bounding_sphere_center.n[2];
            player_count++;
        }
    }

    if (player_count)
    {
        object_iterator iterator;
        object_iterator_new(&iterator, object_mask_vehicle, 0); /* walk all vehicle objects */
        while (1)
        {
            vehicle_datum *vehicle = object_iterator_next(&iterator);
            if (!vehicle)
                break;
            for (int p = 0; p < player_count; p++)
            {
                unit_datum *player_unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player_unit_indices[p])->datum;
                if (player_unit->object.parent_object_index != iterator.index) /* not the vehicle this player is riding */
                {
                    float dx = vehicle->object.bounding_sphere_center.n[0] - player_positions[p].n[0];
                    float dy = vehicle->object.bounding_sphere_center.n[1] - player_positions[p].n[1];
                    float dz = vehicle->object.bounding_sphere_center.n[2] - player_positions[p].n[2];
                    float distance_sq = dz * dz + (dx * dx + dy * dy);
                    float speed_sq = vehicle->object.translational_velocity.n[2] * vehicle->object.translational_velocity.n[2]
                                     + (vehicle->object.translational_velocity.n[0] * vehicle->object.translational_velocity.n[0]
                                        + vehicle->object.translational_velocity.n[1] * vehicle->object.translational_velocity.n[1]);
                    if (distance_sq < 100.0f && speed_sq >= 0.0011111111f)
                    {
                        none_found = 0;
                        break;
                    }
                }
            }
            if (!none_found)
                break;
        }
    }

    return none_found == 0;
}
