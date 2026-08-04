/* filter_invalid_list_indices @0x83780640 — sort indices ascending, then count the leading run of valid
 * (non -1) entries; -1 sorts last, so this counts every valid index. */

#include <stdlib.h>

extern int list_indices_sort_proc(const void *index1, const void *index2);

int filter_invalid_list_indices(int *indices, int number_of_items)
{
    int count = 0;

    qsort(indices, number_of_items, sizeof(int), list_indices_sort_proc);
    if ( number_of_items > 0 )
    {
        int *index = indices;

        do
        {
            if ( *index == -1 )
                break;
            ++count;
            ++index;
        } while ( count < number_of_items );
    }
    return count;
}
