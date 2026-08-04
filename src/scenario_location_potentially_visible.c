/* scenario_location_potentially_visible @0x83703868 — whether a BSP location's cluster is in the combined
 * potentially-visible set of all players this frame (including remote players in network games). */

#include <stdint.h>
#include "headers/location.h"
#include "headers/bit_vector.h"

extern unsigned int *players_get_combined_pvs(void);

uint8_t scenario_location_potentially_visible(const location *location)
{
    int cluster_index = location->cluster_index;
    return BIT_VECTOR_TEST_FLAG(players_get_combined_pvs(), cluster_index);
}
