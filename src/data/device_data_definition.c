/* device_data_definition @ 0x84177828 (.data, 196 bytes)
 * DB applied_types: object_type_definition device_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8202AF50 -> "device"
 *   +0x04 group_tag                  = 0x64657669
 *   +0x08 game_datum_size            = 0x0214
 *   +0x0A placement_tag_block_offset = 0xFFFF
 *   +0x0C palette_tag_block_offset   = 0xFFFF
 *   +0x0E placement_tag_block_element_size = 0xFFFF
 *   +0x10 update_message_type        = 0xFFFFFFFF
 *   +0x14 initialize                 = 0x837B4A08 -> devices_initialize
 *   +0x18 dispose                    = 0x837B4A40 -> devices_dispose
 *   +0x1C initialize_for_new_map     = 0x837B54E8 -> devices_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x837B4A48 -> devices_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x837B4A58 -> device_new
 *   +0x2C datum_place                = 0x00000000
 *   +0x30 datum_delete               = 0x837B5598 -> device_delete
 *   +0x34 datum_update               = 0x837B5658 -> device_update
 *   +0x38 datum_export_function_values = 0x837B4AA0 -> device_export_function_values
 *   +0x3C handle_deleted_object      = 0x00000000
 *   +0x40 handle_region_destroyed    = 0x00000000
 *   +0x44 handle_parent_destroyed    = 0x00000000
 *   +0x48 datum_preprocess_node_orientations = 0x837B4C88 -> device_preprocess_node_orientations
 *   +0x4C datum_postprocess_node_matrices = 0x00000000
 *   +0x50 reset                      = 0x00000000
 *   +0x54 disconnect_from_structure_bsp = 0x00000000
 *   +0x58 notify_impulse_sound       = 0x00000000
 *   +0x5C render_debug               = 0x837B4E30 -> device_render_debug
 *   +0x60 should_render_debug_object_network_visualization = 0x00000000
 *   +0x64 object_new_to_network      = 0x00000000
 *   +0x68 update_baseline            = 0x00000000
 *   +0x6C build_update_delta         = 0x00000000
 *   +0x70 process_update_delta       = 0x00000000
 *   +0x74 should_try_sending_incremental = 0x00000000
 *   +0x78 should_send_incremental_to_player = 0x00000000
 *   +0x7C incremental_sent           = 0x00000000
 *   +0x80 part_definitions           = 0x84176F90841778280000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * third-order .data (data_closure.py); reconstructed from binary bytes.
 */
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
