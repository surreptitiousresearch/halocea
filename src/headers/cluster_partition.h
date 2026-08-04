#pragma once
/* cluster_partition — spatial cluster reference bookkeeping (cluster_partition.c) */

#include "data_array.h"

typedef struct cluster_partition
{
    int        *cluster_first_data_references;  /* 0x0 */
    data_array *data_reference_data;            /* 0x4 */
    data_array *cluster_reference_data;         /* 0x8 */
} cluster_partition;

extern void cluster_partition_new(cluster_partition *partition, const char *name);
