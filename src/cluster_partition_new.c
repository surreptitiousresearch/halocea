/* cluster_partition_new @ 0x837DFB48 — allocate the three cluster-reference tables */

#include <stdint.h>
#include "headers/cluster_partition.h"

#include "headers/data_array.h"
extern void *game_state_malloc(const char *name, const char *type, int size);
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern int sprintf_0(char *string, const char *format, ...);

void cluster_partition_new(cluster_partition *partition, const char *name)
{
    char buf[256];
    char ref_name[288];

    partition->cluster_first_data_references = game_state_malloc(name, "cluster references", 2048);
    sprintf_0(buf, "cluster %s", name);
    sprintf_0(ref_name, "%s reference", buf);
    partition->data_reference_data = game_state_data_new(ref_name, 2048, 12);
    sprintf_0(buf, "%s cluster", name);
    sprintf_0(ref_name, "%s reference", buf);
    partition->cluster_reference_data = game_state_data_new(ref_name, 2048, 12);
}
