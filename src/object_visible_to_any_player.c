/* object_visible_to_any_player @ 0x836EF980 — used by the garbage collector to decide whether an object
 * may be reclaimed. Returns 1 if the object is active, connected to the map (_object_connected_to_map_bit,
 * not _object_outside_of_map_bit),
 * occupies at least one cluster in the players' combined PVS, AND lies within some local player's view:
 * either inside the object's own bounding sphere or inside a view cone — the object's angular radius
 * atan2(radius, distance) widened by pi/4, tested against the player unit's desired_aiming_vector
 * (the old "node matrix forward" comment was wrong; +560 is unit.desired_aiming_vector per DB). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/object_flags.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/object_cluster_iterator.h"
#include "headers/cluster_partition.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"


#include "headers/data_array.h"
extern unsigned int *players_get_combined_pvs(void);
extern int16_t object_get_first_cluster(object_cluster_iterator *iterator, int object_index);
extern int16_t cluster_partition_get_next_cluster(const cluster_partition *partition, int *reference_index);
extern int data_next_index(const data_array *data, int16_t index);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern float normalize3d(real_vector3d *v);
extern double  atan2(double y, double x);
extern double  cos(double x);

uint8_t object_visible_to_any_player(int object_index)
{
    object_header_datum *header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = header->datum;

    if ( (header->flags & (1u << _object_header_active_bit)) == 0 )
        return 0;
    int flags = object->object.flags;
    if ( (flags & (1u << _object_connected_to_map_bit)) == 0 )
        return 0;
    if ( (flags & (1u << _object_outside_of_map_bit)) != 0 )
        return 0;

    const unsigned int *combined_pvs = players_get_combined_pvs();
    object_cluster_iterator iterator;
    int cluster = object_get_first_cluster(&iterator, object_index);
    if ( cluster == -1 )
        return 0;
    do
    {
        if ( BIT_VECTOR_TEST_FLAG(combined_pvs, cluster) )
            break;
        cluster = cluster_partition_get_next_cluster(iterator.cluster_partition, &iterator.reference_index);
    }
    while ( cluster != -1 );
    if ( cluster == -1 )
        return 0;

    float radius = object->object.bounding_sphere_radius;
    float radius_squared = radius * radius;

    int player_index = data_next_index(player_data, -1);
    if ( player_index == -1 )
        return 0;

    while ( 1 )
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        int unit_index = player->unit_index;
        if ( unit_index != -1 )
        {
            real_point3d head_position;
            unit_get_head_position(unit_index, &head_position);

            float dx = object->object.bounding_sphere_center.n[0] - head_position.n[0];
            float dy = object->object.bounding_sphere_center.n[1] - head_position.n[1];
            float dz = object->object.bounding_sphere_center.n[2] - head_position.n[2];
            if ( (dx * dx + (dz * dz + dy * dy)) >= radius_squared )
            {
                real_vector3d to_object;
                to_object.n[0] = dx;
                to_object.n[1] = dy;
                to_object.n[2] = dz;
                float distance = normalize3d(&to_object);

                unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                                    player->unit_index)->datum;
                float facing_dot = unit->unit.desired_aiming_vector.n[0] * to_object.n[0]
                                 + (unit->unit.desired_aiming_vector.n[2] * to_object.n[2]
                                    + unit->unit.desired_aiming_vector.n[1] * to_object.n[1]);

                float cone = (float)atan2(radius, distance) + 0.78539819f;
                if ( facing_dot > (float)cos(cone) )
                    return 1;
            }
            else
            {
                return 1;  /* player is inside the object's bounding sphere */
            }
        }

        player_index = data_next_index(player_data, player_index);
        if ( player_index == -1 )
            return 0;
    }
}
