/* objects_initialize_for_new_map @0x836EEB68 — bring up the object system for a new map: init the damage,
 * widget, object-type, and light subsystems; revalidate the object header array; clear the name table; reset
 * both object cluster partitions; and zero the PVS/garbage-collection bookkeeping. */

#include "headers/object_globals.h"
#include "headers/cluster_partition.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void *memset(void *dst, int value, unsigned int n);


extern void damage_initialize_for_new_map(void);
extern void widgets_initialize_for_new_map(void);
extern void object_types_initialize_for_new_map(void);
extern void lights_initialize_for_new_map(void);
extern void data_make_valid(data_array *data);
extern void cluster_partition_make_valid(cluster_partition *partition);

void objects_initialize_for_new_map(void)
{
    damage_initialize_for_new_map();
    widgets_initialize_for_new_map();
    object_types_initialize_for_new_map();
    lights_initialize_for_new_map();

    data_make_valid(object_header_data);
    memset(object_name_list, -1, 0x800u);
    cluster_partition_make_valid(&collideable_object_cluster_partition);
    cluster_partition_make_valid(&noncollideable_object_cluster_partition);

    object_globals->object_marker_initialized = 0;
    memset(object_globals->last_active_cluster_bits, 0, sizeof(object_globals->last_active_cluster_bits));
    memset(object_globals->active_cluster_bits, 0, sizeof(object_globals->active_cluster_bits));
    object_globals->pvs_activation_type = 0;
    global_object_marker = 0;
    object_globals->first_garbage_object_index = -1;
    object_globals->active_garbage_object_count = 0;
    object_globals->last_garbage_warn_time = 0;
}
