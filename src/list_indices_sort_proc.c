/* list_indices_sort_proc @0x83780608 — qsort comparator that sorts -1 ("no selection") entries to the end. */

int list_indices_sort_proc(const void *index1, const void *index2)
{
    int a = *(const int *)index1;
    int b = *(const int *)index2;
    if ( a == -1 )
        return b != -1;
    if ( b == -1 )
        return -1;
    return 0;
}
