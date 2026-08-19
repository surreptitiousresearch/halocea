/* structure_cluster_mark @ 0x83744640 — claim a cluster for the current marking pass. Returns 1 the
 * first time a cluster is seen this pass (stamping its magic number) and 0 on any repeat, so callers
 * process each cluster exactly once.
 *
 * cluster_index is used raw: extsh r10,r3 @0x83744644 and slwi r10,r10,2 @0x8374464C feed
 * lwzx r8,r10,r9 @0x83744658 with no bound compare and no -1 test. The function's only compare,
 * cmpw cr6,r8,r11 @0x8374465C, is loaded-magic vs cluster_marker — not the index. This is as-built,
 * and the unclustered sentinel is self-handling rather than unchecked: cluster_magic_numbers sits at
 * structure_globals+0x08 (addi r9,r11,8 @0x83744650) and cluster_marker at +0x04, so
 * cluster_magic_numbers[-1] aliases cluster_marker exactly. Index -1 therefore loads cluster_marker,
 * compares it against itself, takes beq @0x83744660 and returns 0 with no store executed — a
 * deterministic "already marked, skip" that stays inside the struct. Both callers rely on it: each
 * gates its cluster-partition iterator behind this return, so a traversed leaf whose cluster_index is
 * -1 is skipped and never reaches cluster_partition_get_first_datum. Do not add an index guard here;
 * it would be a branch the binary does not contain. */

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
