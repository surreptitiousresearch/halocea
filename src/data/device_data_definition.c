/* device_data_definition @ 0x84177828 -- third-order .data (data_closure.py); reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/data_array.h"
#include "headers/real_orientation.h"

extern object_type_definition object_data_definition;
extern void devices_initialize(void);
extern void devices_dispose(void);
extern void devices_initialize_for_new_map(void);
extern void devices_dispose_from_old_map(void);
extern uint8_t device_new(int device_index);
extern void device_delete(int device_index);
extern uint8_t device_update(int device_index);
extern void device_export_function_values(int device_index);
extern void device_preprocess_node_orientations(int device_index, real_orientation *node_orientations);
extern void device_render_debug(int device_index);

object_type_definition device_data_definition = {
    .name = "device",
    .group_tag = 'devi',
    .game_datum_size = 532,          /* 0x0214 (high half of +0x08) */
    .placement_tag_block_offset = -1,        /* 0xFFFF NONE */
    .palette_tag_block_offset = -1,          /* 0xFFFF NONE */
    .placement_tag_block_element_size = -1,  /* 0xFFFF NONE */
    .update_message_type = -1,               /* 0xFFFFFFFF NONE */
    .initialize = &devices_initialize,
    .dispose = &devices_dispose,
    .initialize_for_new_map = &devices_initialize_for_new_map,
    .dispose_from_old_map = &devices_dispose_from_old_map,
    .datum_new = &device_new,
    .datum_delete = &device_delete,
    .datum_update = &device_update,
    .datum_export_function_values = &device_export_function_values,
    .datum_preprocess_node_orientations = &device_preprocess_node_orientations,
    .render_debug = &device_render_debug,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &device_data_definition,
};
