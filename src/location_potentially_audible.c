#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/location.h"
#include "headers/blam_data_globals.h"


int location_potentially_audible(location *location)
{
    uint16_t cluster_index = (uint16_t)location->cluster_index;

    if (cluster_index == 0xFFFF)
        return 0;

    return BIT_VECTOR_TEST_FLAG(combined_pas, cluster_index);
}
