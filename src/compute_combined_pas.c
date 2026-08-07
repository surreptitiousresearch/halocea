/* compute_combined_pas @0x83714288 — build the combined potentially-audible-set (PAS) bit vector for
 * this mix frame. Starts cleared, then for each local player whose camera is inside a valid cluster,
 * marks every cluster whose encoded sound distance from the camera's cluster is under 256 world units.
 * The result drives which background/looping sounds are audible.
 *
 * observer_result.location.cluster_index lands at byte +16. Encoded distance is the low 7 bits of the
 * packed value, scaled by 2.015748 to recover world units. */

#include <stdint.h>
#include <string.h>
#include "headers/bit_vector.h"
#include "headers/structure_bsp.h"
#include "headers/observer_result.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern uint8_t structure_bsp_get_cluster_encoded_sound_distance(structure_bsp *structure_bsp, int16_t from_cluster_index, int16_t to_cluster_index);

void compute_combined_pas(void)
{
    structure_bsp *bsp = global_structure_bsp;
    int16_t local_player_index;
    int16_t i;

    memset(combined_pas, 0, 4 * BIT_VECTOR_SIZE_IN_LONGS(bsp->clusters.count));

    for ( i = 0, local_player_index = 0; i < 2; local_player_index = ++i )
    {
        if ( local_player_get_player_index(local_player_index) == -1 )
            continue;

        {
            const observer_result *camera = observer_get_camera(local_player_index);
            int16_t camera_cluster = (uint16_t)camera->location.cluster_index; /* recovered: *(_WORD *)((char *)camera + 16) -> location.cluster_index */
            int cluster;

            if ( camera_cluster == 0xFFFF )
                continue;

            for ( cluster = 0; cluster < bsp->clusters.count; ++cluster )
            {
                int encoded = structure_bsp_get_cluster_encoded_sound_distance(bsp, cluster, camera_cluster);
                if ( ((float)(encoded & 0x7F) * (float)2.015748) < 256.0 )
                    BIT_VECTOR_SET_FLAG(combined_pas, cluster);
            }
        }
    }
}
