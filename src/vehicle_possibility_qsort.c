/* vehicle_possibility_qsort @0x837DCD20 - qsort comparator over records whose leading float is the sort key. */

/* params respelled to CRT-conformant const void* 2026-07-30 (C4113/C4090 at the qsort call site;
 * funcptr casts are forbidden - the interface is fixed instead). Sort semantics unchanged. */
int vehicle_possibility_qsort(const void *p1, const void *p2)
{
    const float *a = p1;
    const float *b = p2;
    if (*a >= *b)
        return *a > *b;
    return -1;
}
