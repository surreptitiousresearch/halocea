/* structure_bsp_get_cluster_encoded_sound_data @0x837B4960 — packed lower-triangular cluster-to-cluster
 * sound-distance table lookup.
 *
 * DEVIATION: the decompiler renders the cluster count read as `HIWORD(structure_bsp->clusters.count)`;
 * disasm shows a plain full-word `lwz` of clusters.count, reproduced here without the HIWORD truncation. */

#include <stdint.h>
#include "headers/structure_bsp.h"

uint8_t * structure_bsp_get_cluster_encoded_sound_data(structure_bsp *structure_bsp, int16_t row_index, int16_t column_index)
{
    return (uint8_t *)structure_bsp->sound_cluster_data.address
         + (int16_t)((structure_bsp->clusters.count - 1) * row_index - (row_index + 1) * row_index / 2 + column_index - 1);
}
