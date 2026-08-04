/* structure_cluster_mark @ 0x83744640 — claim a cluster for the current marking pass. Returns 1 the
 * first time a cluster is seen this pass (stamping its magic number) and 0 on any repeat, so callers
 * process each cluster exactly once. */

#include "headers/structure_globals.h"
#include <stdint.h>

/* attested uint8_t: both callers clrlwi r3,24 (byte-normalization rule) */
uint8_t structure_cluster_mark(int16_t cluster_index)
{
    if ( structure_globals.cluster_magic_numbers[cluster_index] == structure_globals.cluster_marker )
        return 0;
    structure_globals.cluster_magic_numbers[cluster_index] = structure_globals.cluster_marker;
    return 1;
}
