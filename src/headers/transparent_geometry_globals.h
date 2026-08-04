#pragma once

/* Layout not needed here — only pointers/counters are touched at init. */
typedef struct transparent_geometry_group transparent_geometry_group;

extern transparent_geometry_group *transparent_geometry_groups;
extern transparent_geometry_group *transparent_geometry_groups2;
extern short *transparent_geometry_group_sorted_indices;
extern int transparent_geometry_group_count;
extern int transparent_geometry_group_count2;
