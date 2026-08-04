/* scenario_location_potentially_visible_local @0x83703810 — whether a BSP location's cluster is in the
 * combined potentially-visible set of the local (split-screen) players this frame. */

#include <stdint.h>
#include "headers/location.h"
#include "headers/bit_vector.h"

extern unsigned int *players_get_combined_pvs_local(void);

uint8_t scenario_location_potentially_visible_local(const location *location)
{
    int cluster_index = location->cluster_index;
    return BIT_VECTOR_TEST_FLAG(players_get_combined_pvs_local(), cluster_index);
}
