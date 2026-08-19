/* decal_set_first_decal_index @0x83740198 */
#include <stdint.h>
#include "headers/decal_globals.h"
#include "headers/blam_data_globals.h"


void decal_set_first_decal_index(int16_t cluster_index, int16_t layer, int decal_index)
{
    decal_globals->first_decal_indices[layer][cluster_index] = decal_index;
}
