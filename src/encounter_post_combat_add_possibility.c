/* encounter_post_combat_add_possibility @0x8370B168 (true entry; funcs table's 0x8370B174 lands one
 * instruction into the prologue, right after `mr r5,r3` copies possibility_array off r3 — the classic
 * funcs-table-mid-prologue artifact). Inserts a new weighted possibility into the fixed 2-element sorted array
 * (descending by weight), shifting lower-weighted entries down and dropping the last one if the array is full.
 * Returns whether the new entry was inserted.
 *
 * Deviation: the decompiler fabricated a phantom 6th parameter ("a6") and mislabeled which named parameter
 * feeds which struct field, because it lost track of possibility_array's r3->r5 prologue copy and
 * double-counted a GPR slot. Verified via disasm that the real 5-parameter signature (matching the DB's own
 * funcs.prototype) stores prop_index -> .prop_index and unit_index -> .unit_index directly, with no swap.
 *
 * Surprising-but-verified behavior: the original loop has no break after inserting at i==0, so when a new
 * possibility beats slot 0 the loop falls through to i==1 and re-checks against the just-shifted-down old
 * slot-0 value — which the new weight is, by construction, also greater than — so it overwrites slot 1 with
 * the SAME new values too, leaving both slots identical. Confirmed via disasm (no branch skips the second
 * iteration), so this is reproduced faithfully rather than "corrected" to a single insert. */

#include "headers/post_combat_possibility.h"
#include <stdint.h>

uint8_t encounter_post_combat_add_possibility(post_combat_possibility *possibility_array,
    int actor_index, float weight, int prop_index, int unit_index)
{
    uint8_t inserted = 0;

    for ( __int16 i = 0; i < 2; i++ )
    {
        post_combat_possibility *slot = &possibility_array[i];
        if ( weight > slot->weight )
        {
            if ( i < 1 )
                possibility_array[1] = possibility_array[0];

            slot->weight = weight;
            slot->actor_index = actor_index;
            slot->prop_index = prop_index;
            slot->unit_index = unit_index;
            inserted = 1;
        }
    }

    return inserted;
}
