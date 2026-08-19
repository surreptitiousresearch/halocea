/* objects_initialize @ 0x836EEA68 — object subsystems, object pool + memory pool */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

typedef struct memory_pool memory_pool;
typedef struct cluster_partition cluster_partition;

extern void damage_initialize(void);
extern void widgets_initialize(void);
extern void object_types_initialize(void);
extern void lights_initialize(void);
extern uint8_t game_in_editor(void);
extern data_array *data_new(const char *name, int16_t maximum_count, int16_t size);
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern memory_pool *memory_pool_new(const char *name, int size);
extern memory_pool *game_state_memory_pool_new(const char *name, int size);
extern void *game_state_malloc(const char *name, const char *type, int size);
extern void cluster_partition_new(cluster_partition *partition, const char *name);


void objects_initialize(void)
{
    damage_initialize();
    widgets_initialize();
    object_types_initialize(); /* attested void(void) */
    lights_initialize();

    if ( game_in_editor() )
    {
        object_header_data = data_new("object", 10240, 12);
        object_memory_pool = memory_pool_new("objects", 10485760);
    }
    else
    {
        object_header_data = game_state_data_new("object", 2048, 12);
        object_memory_pool = game_state_memory_pool_new("objects", 0x200000);
    }

    object_globals = game_state_malloc("object globals", 0, 152);
    object_name_list = game_state_malloc("object name list", 0, 2048);
    cluster_partition_new(&collideable_object_cluster_partition, "collideable object");
    cluster_partition_new(&noncollideable_object_cluster_partition, "noncollideable object");
}
