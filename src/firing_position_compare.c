/* firing_position_compare @0x837EF858 — qsort-style comparator over global_temporary_sort_firing_position_array:
 * ranks invalid positions above valid ones, then rejected positions above accepted ones, then lower-evaluation
 * positions above higher-evaluation ones.
 *
 * Deviation: the decompiler rendered both the valid- and rejected- branches as the same "sign-trick" bit
 * expression, but verified via disasm (rlwinm masking a different bit than the valid branch's clrrwi) that the
 * rejected branch's real result is `rejected1 != 0`, not the decompiler's literal `(x & ~x) >> 31` formula
 * (which is tautologically always 0 and would make the branch dead code). The valid branch's decompiled
 * formula was independently confirmed correct by the same disasm trace. */

#include <stdint.h>
#include "headers/firing_position.h"
#include "headers/blam_data_globals.h"


/* return respelled uint8_t->int 2026-07-30: disasm returns srwi r3,r8,31 / li r3,0 (width-agnostic 0/1,
 * no byte-normalize); harmonizes the qsort_4byte compare slot with compare_surface_indices (C4133) */
int firing_position_compare(int index1, int index2)
{
    const firing_position *a = &global_temporary_sort_firing_position_array[index1];
    const firing_position *b = &global_temporary_sort_firing_position_array[index2];

    if ( a->valid != b->valid )
        return a->valid == 0;

    if ( a->rejected != b->rejected )
        return a->rejected != 0;

    return a->evaluation < b->evaluation;
}
