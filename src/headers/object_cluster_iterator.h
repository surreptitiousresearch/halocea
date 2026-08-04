#pragma once
/* object_cluster_iterator — walks the structure-BSP clusters an object straddles. Seeded by
 * object_get_first_cluster; advanced by cluster_partition_get_next_cluster. Layout from the database. */

struct cluster_partition;

typedef struct object_cluster_iterator
{
    struct cluster_partition *cluster_partition; /* 0x00 */
    int                       reference_index;   /* 0x04 */
} object_cluster_iterator;
