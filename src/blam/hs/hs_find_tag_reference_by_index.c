/* hs_find_tag_reference_by_index @0x8372EE78 — find the scenario.hs_references slot whose resolved tag
 * index matches, returning its slot index (-1 if not found or no scenario loaded).
 *
 * DEVIATION: the decompiler indexes hs_references as a raw `int[10*i+9]` array; that's
 * hs_tag_reference[i].reference.index (40-byte stride, index at byte offset 36), per hs_tag_reference.h /
 * tag_reference.h (already used the same way in hs_parse_tag_reference.c). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_tag_reference.h"

int16_t hs_find_tag_reference_by_index(int tag_index)
{
    if ( global_scenario_index == -1 )
        return -1;

    int count = global_scenario->hs_references.count;
    if ( count <= 0 )
        return -1;

    hs_tag_reference *references = (hs_tag_reference *)global_scenario->hs_references.address;
    int i = 0;

    while ( references[i].reference.index != tag_index )
    {
        if ( ++i >= count )
            return -1;
    }
    return i;
}
