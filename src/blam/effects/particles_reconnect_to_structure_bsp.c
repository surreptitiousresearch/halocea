/* particles_reconnect_to_structure_bsp @0x8373DA78 — after a structure-BSP switch, recompute each live
 * particle's BSP location (112-byte datums: flags word +2 [_particle_attached_to_local_player_bit = first-person
 * weapon], object index +8, node
 * word +12, first-person player byte +15, location @40, free position @48): attached particles use their
 * node matrix position (first-person weapon node or object node), free particles their stored position.
 * Particles whose owner object is gone or whose new location has no cluster are deleted. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/location.h"
#include "headers/particle_datum.h"
#include "headers/particle_flags.h"
#include "headers/real_matrix4x3.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void datum_delete(data_array *data, int index);

void particles_reconnect_to_structure_bsp(void)
{
    for ( int particle_index = data_next_index(particle_data, -1); particle_index != -1;
          particle_index = data_next_index(particle_data, particle_index) )
    {
        particle_datum *particle = DATA_ARRAY_ELEMENT(particle_data, particle_datum, particle_index);
        int object_index = particle->object_index;

        const real_point3d *position = 0;
        if ( object_index == -1 )
        {
            position = &particle->position;
        }
        else if ( (particle->flags & (1u << _particle_attached_to_local_player_bit)) != 0 )
        {
            position = &first_person_weapon_get_node_matrix(particle->local_player_index,
                                                            particle->node_index)->position;
        }
        else if ( object_try_and_get_and_verify_type(object_index, object_mask_all) )
        {
            position = &object_get_node_matrix(object_index, particle->node_index)->position;
        }

        if ( position )
        {
            scenario_location_from_point(&particle->location, position);
            /* cluster_index (BE __int16) sentinel compare kept unsigned */
            if ( (uint16_t)particle->location.cluster_index != 0xFFFF )
                continue;
        }
        datum_delete(particle_data, particle_index);
    }
}
