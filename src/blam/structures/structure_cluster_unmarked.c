/* structure_cluster_unmarked @0x83744610 — true when a cluster's stamped magic number doesn't match this
 * frame's marker (i.e. it hasn't been visited/marked yet this pass). */

#include <stdint.h>
#include "headers/structure_globals.h"

extern structure_globals_t structure_globals;

uint8_t structure_cluster_unmarked(int16_t cluster_index)
{
    return structure_globals.cluster_marker != structure_globals.cluster_magic_numbers[cluster_index];
}
