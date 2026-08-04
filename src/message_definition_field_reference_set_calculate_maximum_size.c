/* message_definition_field_reference_set_calculate_maximum_size @0x837A0A58 — sums each field reference's
 * properties->maximum_size across the set.
 *
 * DEVIATION: the decompiler unrolled the loop two-at-a-time and mis-labeled its own locals (e.g. calling a
 * `field_references[i].properties` load "baseline_offset"); disasm_range(0x837A0A58,0x837A0ADC), cross-
 * checked against raw instruction operands (var_1D0/var_1B0 resolve to r1+0x50/r1+0x70), confirms every
 * load lands on `.properties` (offset 0) before dereferencing `->maximum_size` (offset 0x5C), never the
 * true baseline_offset field (offset 8). Reproduced as an equivalent single-pass loop. */

#include "headers/message_definition_field_reference_set.h"

int message_definition_field_reference_set_calculate_maximum_size(const _message_definition_field_reference_set *const field_reference_set)
{
    int total = 0;

    for ( int i = 0; i < field_reference_set->count; ++i )
        total += field_reference_set->field_references[i].properties->maximum_size;

    return total;
}
