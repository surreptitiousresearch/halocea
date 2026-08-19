/* sort_dumps @0x836EE468 — qsort comparator ordering memory dumps by total_size, largest first.
 *
 * DEVIATION: the decompiler emits the descending compare as a __PAIR64__ borrow expression; it
 * reduces to the sign of (dump2->total_size - dump1->total_size). */

/* params respelled to CRT-conformant const void* 2026-08-11 (UNIT #106). objects_dump_memory.c
 * hands this straight to qsort, whose comparator slot is int (*)(const void *, const void *);
 * clang makes that mismatch an ERROR, and an error-severity diagnostic excludes the whole TU from
 * every libclang scanner's findings (castaudit ledgers such TUs in .sweep/castaudit_parse_errors
 * .txt), so one declaration was blinding a scanner battery for a TU it otherwise covers. Funcptr
 * casts are forbidden here — the interface is fixed instead, exactly as
 * actor_perception_qsort_compare_optional_props.c was on 2026-07-30. ABI-identical and therefore
 * faithful: the binary reads two incoming pointers, lwz r11,8(r3) / lwz r10,8(r4) @0x836EE468,
 * and carries no distinction between the two spellings. Sort semantics unchanged. */

#include "headers/dump_datum.h"

int sort_dumps(const void *p1, const void *p2)
{
    const dump_datum *dump1 = p1;
    const dump_datum *dump2 = p2;
    if ( dump1->total_size < dump2->total_size )
        return 1;
    if ( dump1->total_size > dump2->total_size )
        return -1;
    return 0;
}
