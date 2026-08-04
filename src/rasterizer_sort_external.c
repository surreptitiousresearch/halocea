/* rasterizer_sort_external @0x8376880C — sort this frame's transparent geometry groups for external
 * (non-portal-local) draw order: seed the sorted-indices array with identity order, qsort it via
 * group_sorted_indices_cmpfn, then stamp each group's resulting rank into its own sorted_index field. */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"


extern int group_sorted_indices_cmpfn(const int16_t *group_index1, const int16_t *group_index2);
extern void qsort(void *base, unsigned int count, unsigned int size, int (__fastcall *compare)(const void *, const void *));

void rasterizer_sort_external(void)
{
    for ( __int16 i = 0; i < transparent_geometry_group_count; ++i )
        transparent_geometry_group_sorted_indices[i] = i;

    qsort(transparent_geometry_group_sorted_indices, transparent_geometry_group_count, 2u,
        (int (__fastcall *)(const void *, const void *))group_sorted_indices_cmpfn);

    for ( int i = 0; i < transparent_geometry_group_count; ++i )
        transparent_geometry_groups[transparent_geometry_group_sorted_indices[i]].sorted_index = i;
}
