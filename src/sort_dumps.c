/* sort_dumps @0x836EE468 — qsort comparator ordering memory dumps by total_size, largest first.
 *
 * DEVIATION: the decompiler emits the descending compare as a __PAIR64__ borrow expression; it
 * reduces to the sign of (dump2->total_size - dump1->total_size). */

#include "headers/dump_datum.h"

int sort_dumps(const dump_datum *dump1, const dump_datum *dump2)
{
    if ( dump1->total_size < dump2->total_size )
        return 1;
    if ( dump1->total_size > dump2->total_size )
        return -1;
    return 0;
}
