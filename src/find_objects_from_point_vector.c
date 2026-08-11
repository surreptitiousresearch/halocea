/* find_objects_from_point_vector @0x836EE2B8 — collect collideable objects visible from a point's cluster
 * PVS (all clusters potentially visible from the camera's cluster, not actually a directional/vector
 * query despite the name), invoking `recursive_object_adder` for each newly-marked object. Returns the
 * number of objects added.
 *
 * DEVIATION: `direction` is a genuinely unused parameter — confirmed via disasm (r4 is never saved or
 * read anywhere in the function body); kept in the signature to match the DB prototype and call sites. */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/structure_bsp.h"
#include "headers/object_globals.h"
#include "headers/cluster_partition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


#include "headers/structure_bsp.h"
#include "headers/cluster_partition.h"
extern int16_t scenario_cluster_index_from_point(const real_point3d *point);
extern unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index);
extern int cluster_partition_get_first_datum(const cluster_partition *partition, int *reference_index, int16_t cluster_index);
extern int cluster_partition_get_next_datum(const cluster_partition *partition, int *reference_index);
extern int recursive_object_adder(int object_index, uint8_t (*add_object_function)(int object_index, void *custom_data), void *custom_data, int object_count, int maximum_count, int *object_indices);

int find_objects_from_point_vector(const real_point3d *position, const real_vector3d *direction,
                                   uint8_t (*add_object_function)(int, void *),
                                   void *custom_data, int maximum_object_count, int *object_indices)
{
    int object_count = 0;

    int16_t cluster_index = scenario_cluster_index_from_point(position);
    if (cluster_index == -1)
        return object_count;

    ++global_object_marker;
    object_globals->object_marker_initialized = 1;

    unsigned int *cluster_pvs = structure_bsp_get_cluster_pvs(global_structure_bsp, cluster_index);

    int16_t word_count = (int16_t)BIT_VECTOR_SIZE_IN_LONGS(global_structure_bsp->clusters.count);
    for (int16_t word_index = 0; word_index < word_count; ++word_index)
    {
        if (!cluster_pvs[word_index])
            continue;

        int total_clusters = global_structure_bsp->clusters.count;
        int block_end = 32 * word_index + 32;
        if (block_end > total_clusters)
            block_end = total_clusters;

        for (int cluster = 32 * word_index; cluster < block_end; ++cluster)
        {
            if (!BIT_VECTOR_TEST_FLAG(cluster_pvs, cluster))
                continue;

            int reference_index;
            for (int object_index = cluster_partition_get_first_datum(&collideable_object_cluster_partition,
                                                                      &reference_index, cluster);
                 object_index != -1;
                 object_index = cluster_partition_get_next_datum(&collideable_object_cluster_partition,
                                                                 &reference_index))
            {
                object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
                if (object->object.magic_number != global_object_marker)
                {
                    object->object.magic_number = global_object_marker;
                    object_count = recursive_object_adder(object_index, add_object_function, custom_data,
                                                          object_count, maximum_object_count, object_indices);
                }
            }
        }
    }

    object_globals->object_marker_initialized = 0;
    return object_count;
}
