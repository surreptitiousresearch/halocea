/* objects_dispose @ 0x836EECF8 — tear down object subsystems, pools, cluster partitions */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"
typedef struct memory_pool memory_pool;
#include "headers/cluster_partition.h"
#include "headers/data_array.h"
extern void widgets_dispose(void);
extern void object_types_dispose(void);
extern void lights_dispose(void);
extern uint8_t game_in_editor(void);
extern void data_dispose(data_array *data);
extern void cluster_partition_delete(cluster_partition *partition);
void objects_dispose(void)
{
    widgets_dispose();
    object_types_dispose();
    lights_dispose();
    if ( game_in_editor() )
        data_dispose(object_header_data);
    else if ( object_header_data )
        object_header_data = 0;
    if ( object_memory_pool )
        object_memory_pool = 0;
    cluster_partition_delete(&collideable_object_cluster_partition);
    cluster_partition_delete(&noncollideable_object_cluster_partition);
}
