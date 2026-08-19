/* decal_get_first_decal_index @0x83740850 */
/* decal_get_first_decal_index 0x83740850 — head decal index for one cluster within one decal layer
 * (-1 when the layer/cluster bucket is empty). */

#include <stdint.h>
#include "headers/decal_globals.h"

int decal_get_first_decal_index(int16_t cluster_index, int16_t layer)
{
    return decal_globals->first_decal_indices[layer][cluster_index];
}
