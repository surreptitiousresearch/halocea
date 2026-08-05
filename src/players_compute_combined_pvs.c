/* players_compute_combined_pvs @0x836A9548 — ORs together the potential-visible-set (PVS) bit vectors
 * of the structure-BSP clusters that every player's unit currently occupies, producing the combined
 * set of clusters that must be rendered/simulated. In the editor it uses the free camera's cluster
 * instead. The activating-cluster (a script/trigger-forced cluster) is always folded in as well. When
 * local_only is set, non-local (network) players are skipped. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/structure_bsp.h"
#include "headers/observer_result.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern void *memset(void *dst, int value, unsigned int size);
extern uint8_t game_in_editor(void);
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern int16_t scenario_cluster_index_from_point(const real_point3d *point);
extern int objects_get_activating_cluster_index(void);
extern int object_get_ultimate_parent(int object_index);
extern unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index);
extern void bit_vector_or(int16_t count, const unsigned int *a, const unsigned int *b, unsigned int *result);

static object_datum *object_datum_get(int object_index)
{
    return (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
}

void players_compute_combined_pvs(unsigned int *combined_pvs, uint8_t local_only)
{
    structure_bsp *bsp = global_structure_bsp;
    memset(combined_pvs, 0, 0x40u);

    if (game_in_editor())
    {
        const observer_result *camera = observer_get_camera(0);
        int cluster_index = scenario_cluster_index_from_point(&camera->position); /* attested int16_t return — cast removed */
        if (cluster_index == -1)
            return;
        const unsigned int *pvs = structure_bsp_get_cluster_pvs(bsp, cluster_index);
        bit_vector_or(bsp->clusters.count, combined_pvs, pvs, combined_pvs);
        return;
    }

    /* (int16_t) models extsh r11,r26 @0x836A9628 — narrowed before the -1 compare */
    int16_t activating_cluster_index = (int16_t)objects_get_activating_cluster_index();
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for (player_datum *player = data_iterator_next(&iterator); player;
         player = data_iterator_next(&iterator))
    {
        if (local_only && (uint16_t)player->local_player_index == 0xFFFF)
            continue;

        int unit_index = player->unit_index;
        if (unit_index != -1)
        {
            int ultimate_parent = object_get_ultimate_parent(unit_index);
            int cluster_index = (uint16_t)object_datum_get(ultimate_parent)->object.location.cluster_index;
            if (cluster_index != 0xFFFF)
                player->cluster_index = cluster_index;
        }

        int cluster_index = (uint16_t)player->cluster_index;
        if (cluster_index != 0xFFFF)
        {
            const unsigned int *pvs = structure_bsp_get_cluster_pvs(bsp, cluster_index);
            bit_vector_or(bsp->clusters.count, combined_pvs, pvs, combined_pvs);
        }
    }

    if (activating_cluster_index != -1)
    {
        const unsigned int *pvs = structure_bsp_get_cluster_pvs(bsp, activating_cluster_index);
        bit_vector_or(bsp->clusters.count, combined_pvs, pvs, combined_pvs);
    }
}
