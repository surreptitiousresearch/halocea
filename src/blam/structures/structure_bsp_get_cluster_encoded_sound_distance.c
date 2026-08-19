/* structure_bsp_get_cluster_encoded_sound_distance @ 0x837B49A0 — encoded sound attenuation distance
 * between two clusters. The values are stored as one byte per unordered cluster pair in the upper
 * triangle of a cluster x cluster matrix; same-cluster is distance 0. The pair (i<j) maps to
 * triangular index (n-1)*i - i*(i+1)/2 + j - 1, where n = cluster count.
 *
 * Deviation: the decompiler rendered the cluster count as HIWORD(clusters.count); the disassembly
 * (lwz 0x134; addi -1) confirms it is simply clusters.count - 1. */

#include <stdint.h>
#include "headers/structure_bsp.h"

uint8_t structure_bsp_get_cluster_encoded_sound_distance(structure_bsp *structure_bsp,
                                                     int16_t from_cluster_index, int16_t to_cluster_index)
{
    if ( from_cluster_index == to_cluster_index )
        return 0;

    if ( from_cluster_index > to_cluster_index )
    {
        int16_t temp = from_cluster_index;
        from_cluster_index = to_cluster_index;
        to_cluster_index = temp;
    }

    return *((unsigned char *)structure_bsp->sound_cluster_data.address
             + (int16_t)((structure_bsp->clusters.count - 1) * from_cluster_index
                         - (from_cluster_index + 1) * from_cluster_index / 2
                         + to_cluster_index - 1));
}
