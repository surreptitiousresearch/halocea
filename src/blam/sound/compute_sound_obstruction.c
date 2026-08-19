/* compute_sound_obstruction @ 0x837143F8 — estimate how much a sound is obstructed/occluded from
 * the listener's camera. Starts pessimistic (0.6 obstruction, full occlusion); if the source's
 * cluster is within sound-propagation distance of the camera's cluster and is potentially visible
 * (PVS), it relaxes to 0.45 and clears obstruction entirely on a clear line of sight. Occlusion is
 * then scaled by the propagation distance vs the sound's travel distance. Writes the result into
 * source->obstruction / source->occlusion. */

#include <stdint.h>
#include "headers/sound_source.h"
#include "headers/observer_result.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"
#include "headers/bit_vector.h"

#include "headers/structure_bsp.h"
#include "headers/real_point3d.h"
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern uint8_t structure_bsp_get_cluster_encoded_sound_distance(structure_bsp *structure_bsp, int16_t from_cluster_index, int16_t to_cluster_index);
extern unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);


void compute_sound_obstruction(int16_t local_player_index, sound_source *source, float distance)
{
    const observer_result *camera = observer_get_camera(local_player_index);
    int source_cluster = (uint16_t)source->location.game_location.cluster_index;
    int camera_cluster;
    int encoded_distance;
    float propagation_distance;

    source->obstruction = 0.60000002f;
    source->occlusion = 1.0f;

    if ( source_cluster == 0xFFFF )
        return;

    camera_cluster = (uint16_t)camera->location.cluster_index;
    if ( camera_cluster == 0xFFFF )
        return;

    encoded_distance = structure_bsp_get_cluster_encoded_sound_distance(global_structure_bsp, camera_cluster, source_cluster) & 0x7F;
    propagation_distance = (float)encoded_distance * 2.015748f;
    if ( propagation_distance >= 256.0f )
        return;

    /* potentially visible from the camera cluster → relax obstruction, test line of sight */
    {
        int src_cluster = source->location.game_location.cluster_index;
        unsigned int *pvs = structure_bsp_get_cluster_pvs(global_structure_bsp, camera->location.cluster_index);
        if ( BIT_VECTOR_TEST_FLAG(pvs, src_cluster) )
        {
            real_vector3d delta;
            collision_result result;
            source->obstruction = 0.44999999f;
            delta.n[0] = source->location.position.n[0] - camera->position.n[0];
            delta.n[1] = source->location.position.n[1] - camera->position.n[1];
            delta.n[2] = source->location.position.n[2] - camera->position.n[2];
            if ( !collision_test_vector(_collision_test_environment_flags | (1u << _collision_test_objects_bit)
                                       | (1u << _collision_test_objects_bipeds_bit) | (1u << _collision_test_objects_machines_bit), &camera->position, &delta, -1, &result) )
            {
                source->obstruction = 0.0f;
                source->occlusion = 0.0f;
            }
        }
    }

    if ( source->obstruction != 0.0f )
    {
        float base = 1.0f - distance / (propagation_distance + distance);
        float scaled = base * 1.4f;
        source->occlusion = base;
        if ( scaled >= 0.0f )
        {
            if ( scaled > 1.0f )
                scaled = 1.0f;
            source->occlusion = scaled;
        }
        else
        {
            source->occlusion = 0.0f;
        }
    }
}
