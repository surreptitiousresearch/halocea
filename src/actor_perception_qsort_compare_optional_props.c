/* actor_perception_qsort_compare_optional_props @0x837D6110 - qsort comparator over records whose 3rd float
 * (index 2) is the sort key (descending is not implied here - standard ascending 3-way compare). */

/* params respelled to CRT-conformant const void* 2026-07-30 (C4113/C4090 at the qsort call sites;
 * funcptr casts are forbidden - the interface is fixed instead). Sort semantics unchanged. */
int actor_perception_qsort_compare_optional_props(const void *p1, const void *p2)
{
    const float *a = p1;
    const float *b = p2;
    if (a[2] >= b[2])
        return a[2] > b[2];
    return -1;
}
