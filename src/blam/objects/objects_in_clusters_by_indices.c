/* objects_in_clusters_by_indices @0x836EF6D8 — expand a set of structure clusters into the list of
 * distinct object indices residing in them, filtered by class flags. For each cluster it walks the
 * collideable object partition (class bit 0) and/or the non-collideable object partition (class bit 1),
 * deduping objects across clusters with the global object marker stamp (object.magic_number). Writes
 * up to maximum_object_count indices to object_indices and returns the count written.
 *
 * class_flags == 0 is treated as "all" (0xFF). */

#include <stdint.h>
#include "headers/cluster_partition.h"
#include "headers/object_globals.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/object_class.h"

#include "headers/cluster_partition.h"
extern int cluster_partition_get_first_datum(const cluster_partition *partition, int *reference_index, int16_t cluster_index);
extern int cluster_partition_get_next_datum(const cluster_partition *partition, int *reference_index);

int16_t objects_in_clusters_by_indices(unsigned int class_flags, int16_t cluster_count,
        const int16_t *cluster_indices, int16_t maximum_object_count, int *object_indices)
{
    unsigned char effective_flags = (unsigned char)class_flags;
    int accepted = 0;
    int collideable_reference;
    int noncollideable_reference;
    int c;

    if ( !class_flags )
        effective_flags = 0xFF;   /* all classes — binary uses the full byte; only bits 0-1 (object_class) are defined */

    ++global_object_marker;
    object_globals->object_marker_initialized = 1;

    if ( cluster_count > 0 )
    {
        for ( c = 0; c < cluster_count; c = (int16_t)(c + 1) )
        {
            int16_t cluster_index = cluster_indices[c];

            if ( (effective_flags & (1u << _object_class_collideable)) != 0 )
            {
                int index;
                for ( index = cluster_partition_get_first_datum(&collideable_object_cluster_partition,
                              &collideable_reference, cluster_index);
                      index != -1;
                      index = cluster_partition_get_next_datum(&collideable_object_cluster_partition,
                              &collideable_reference) )
                {
                    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, index)->datum;
                    if ( object->object.magic_number != global_object_marker )
                    {
                        object->object.magic_number = global_object_marker;
                        if ( (int16_t)accepted >= maximum_object_count )
                            goto done;
                        object_indices[(int16_t)accepted] = index;
                        accepted = (int16_t)(accepted + 1);
                    }
                }
            }

            if ( (effective_flags & (1u << _object_class_noncollideable)) != 0 )
            {
                int index;
                for ( index = cluster_partition_get_first_datum(&noncollideable_object_cluster_partition,
                              &noncollideable_reference, cluster_index);
                      index != -1;
                      index = cluster_partition_get_next_datum(&noncollideable_object_cluster_partition,
                              &noncollideable_reference) )
                {
                    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, index)->datum;
                    if ( object->object.magic_number != global_object_marker )
                    {
                        object->object.magic_number = global_object_marker;
                        if ( (int16_t)accepted >= maximum_object_count )
                            goto done;
                        object_indices[(int16_t)accepted] = index;
                        accepted = (int16_t)(accepted + 1);
                    }
                }
            }
        }
    }

done:
    object_globals->object_marker_initialized = 0;
    return accepted;
}
