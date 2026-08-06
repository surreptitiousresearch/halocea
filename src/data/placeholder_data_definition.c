/* placeholder_data_definition @ 0x84177B48 (.data, 196 bytes)
 * DB applied_types: object_type_definition placeholder_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821145D0 -> "placeholder"
 *   +0x04 group_tag                  = 0x706C6163
 *   +0x08 game_datum_size            = 0x01FC
 *   +0x0A placement_tag_block_offset = 0xFFFF
 *   +0x0C palette_tag_block_offset   = 0xFFFF
 *   +0x0E placement_tag_block_element_size = 0xFFFF
 *   +0x10 update_message_type        = 0xFFFFFFFF
 *   +0x14 initialize                 = 0x837E9958 -> placeholder_initialize
 *   +0x18 dispose                    = 0x837E9970 -> placeholder_dispose
 *   +0x1C initialize_for_new_map     = 0x837E9960 -> placeholder_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x837E9968 -> placeholder_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x837E9980 -> placeholder_new
 *   +0x2C datum_place                = 0x837E9978 -> placeholder_place
 *   +0x30 datum_delete               = 0x837E9988 -> placeholder_delete
 *   +0x34 datum_update               = 0x00000000
 *   +0x38 datum_export_function_values = 0x00000000
 *   +0x3C handle_deleted_object      = 0x00000000
 *   +0x40 handle_region_destroyed    = 0x00000000
 *   +0x44 handle_parent_destroyed    = 0x00000000
 *   +0x48 datum_preprocess_node_orientations = 0x00000000
 *   +0x4C datum_postprocess_node_matrices = 0x00000000
 *   +0x50 reset                      = 0x00000000
 *   +0x54 disconnect_from_structure_bsp = 0x00000000
 *   +0x58 notify_impulse_sound       = 0x00000000
 *   +0x5C render_debug               = 0x00000000
 *   +0x60 should_render_debug_object_network_visualization = 0x00000000
 *   +0x64 object_new_to_network      = 0x00000000
 *   +0x68 update_baseline            = 0x00000000
 *   +0x6C build_update_delta         = 0x00000000
 *   +0x70 process_update_delta       = 0x00000000
 *   +0x74 should_try_sending_incremental = 0x00000000
 *   +0x78 should_send_incremental_to_player = 0x00000000
 *   +0x7C incremental_sent           = 0x00000000
 *   +0x80 part_definitions           = 0x84176F9084177B480000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/scenario_placeholder_datum.h"
extern object_type_definition object_data_definition;
extern void placeholder_delete(int placeholder_index);
extern void placeholder_dispose(void);
extern void placeholder_dispose_from_old_map(void);
extern void placeholder_initialize(void);
extern void placeholder_initialize_for_new_map(void);
extern uint8_t placeholder_new(int object_index);
extern void placeholder_place(int placeholder_index, scenario_placeholder_datum *scenario_placeholder);

object_type_definition placeholder_data_definition = {
    .name = "placeholder",
    .group_tag = 'plac',
    .game_datum_size = 508,
    .placement_tag_block_offset = -1,
    .palette_tag_block_offset = -1,
    .placement_tag_block_element_size = -1,
    .update_message_type = -1,
    .initialize = &placeholder_initialize,
    .dispose = &placeholder_dispose,
    .initialize_for_new_map = &placeholder_initialize_for_new_map,
    .dispose_from_old_map = &placeholder_dispose_from_old_map,
    .datum_new = &placeholder_new,
    .datum_place = &placeholder_place,
    .datum_delete = &placeholder_delete,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &placeholder_data_definition,
};
