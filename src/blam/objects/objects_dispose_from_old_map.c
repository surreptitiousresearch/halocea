/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* objects_dispose_from_old_map @ 0x836EEC20 — free every live
 * object's pool block, delete its header datum, then invalidate the pool and partitions.
 * The per-datum address math (raw offsets into object_header_data->data) is reproduced
 * from the decompiler. */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/cluster_partition.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"
typedef struct memory_pool memory_pool;
#include "headers/cluster_partition.h"
#include "headers/data_array.h"
extern void damage_dispose_from_old_map(void);
extern void widgets_dispose_from_old_map(void);
extern void object_types_dispose_from_old_map(void);
extern void lights_dispose_from_old_map(void);
extern int data_next_index(const data_array *data, int16_t index);
extern void memory_pool_block_free(memory_pool *pool, void **reference);
extern void datum_delete(data_array *data, int index);
extern void data_make_invalid(data_array *data);
extern void cluster_partition_make_invalid(cluster_partition *partition);
void objects_dispose_from_old_map(void)
{
    damage_dispose_from_old_map();
    widgets_dispose_from_old_map();
    object_types_dispose_from_old_map();
    lights_dispose_from_old_map();

    if ( object_header_data->valid )
    {
        int i;
        for ( i = data_next_index(object_header_data, -1); i != -1; i = data_next_index(object_header_data, i) )
        {
            /* 12-byte header datum; index math (4*u16 + 4*((2i)&0x1FFFE)) reproduced from decompiler */
            object_header_datum *entry = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, i);
            if ( entry->datum )
                memory_pool_block_free(object_memory_pool, &entry->datum);
            datum_delete(object_header_data, i);
            entry->datum = 0;
            entry->flags = 0;
        }
        data_make_invalid(object_header_data);
    }
    cluster_partition_make_invalid(&collideable_object_cluster_partition);
    cluster_partition_make_invalid(&noncollideable_object_cluster_partition);
}
