/* cluster_partition_reconnect @ 0x837DFE08 — insert an object (datum_index) into every structure cluster
 * its bounding sphere touches. structure_clusters_in_sphere returns up to 64 intersected cluster indices
 * (starting from the object's known location cluster). For each: allocate a cluster reference (linked into
 * the object's first_cluster_reference list, recording the cluster at ref+4) and a data reference (linked
 * into that cluster's data list at partition->cluster_first_data_references[cluster], recording datum_index
 * at ref+4).
 *
 * The decompiler muddles structure_clusters_in_sphere's argument list (a float radius sits between GPR
 * args); reconstructed from the disassembly as (cluster_index, position, radius, max=64, out_buffer). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_reference.h"
#include "headers/cluster_partition.h"
#include "headers/real_point3d.h"
#include "headers/location.h"

extern int datum_new(data_array *data);
extern int16_t structure_clusters_in_sphere(int16_t cluster_index, const real_point3d *position, float radius, int16_t maximum_count, int16_t *intersected_indices);

void cluster_partition_reconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference,
    const real_point3d *position, float radius, const location *location)
{
    int16_t intersected_cluster_indices[64];
    int16_t cluster_count = structure_clusters_in_sphere(location->cluster_index, position, radius,
        64, intersected_cluster_indices);
    if ( cluster_count > 64 )
        cluster_count = 64;

    for ( int i = 0; i < cluster_count; ++i )
    {
        int16_t cluster_index = intersected_cluster_indices[i];

        data_array *cluster_reference_data = partition->cluster_reference_data;
        int cluster_reference_index = datum_new(cluster_reference_data);
        if ( cluster_reference_index != -1 )
        {
            data_reference *cluster_reference =
                DATA_ARRAY_ELEMENT(cluster_reference_data, data_reference, cluster_reference_index);
            cluster_reference->datum_index = cluster_index;
            cluster_reference->next_reference_index = *first_cluster_reference;
            *first_cluster_reference = cluster_reference_index;
        }

        data_array *data_reference_data = partition->data_reference_data;
        int *cluster_first_data_references = partition->cluster_first_data_references;
        int data_reference_index = datum_new(data_reference_data);
        if ( data_reference_index != -1 )
        {
            data_reference *data_ref =
                DATA_ARRAY_ELEMENT(data_reference_data, data_reference, data_reference_index);
            data_ref->datum_index = datum_index;
            data_ref->next_reference_index = cluster_first_data_references[cluster_index];
            cluster_first_data_references[cluster_index] = data_reference_index;
        }
    }
}
