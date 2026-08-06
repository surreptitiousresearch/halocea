/* garbage_data_definition @ 0x84177508 (.data, 196 bytes)
 * DB applied_types: object_type_definition garbage_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82114618 -> "garbage"
 *   +0x04 group_tag                  = 0x67617262
 *   +0x08 game_datum_size            = 0x0244
 *   +0x0A placement_tag_block_offset = 0xFFFF
 *   +0x0C palette_tag_block_offset   = 0xFFFF
 *   +0x0E placement_tag_block_element_size = 0xFFFF
 *   +0x10 update_message_type        = 0xFFFFFFFF
 *   +0x14 initialize                 = 0x00000000
 *   +0x18 dispose                    = 0x00000000
 *   +0x1C initialize_for_new_map     = 0x00000000
 *   +0x20 dispose_from_old_map       = 0x00000000
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x837EA090 -> garbage_new
 *   +0x2C datum_place                = 0x00000000
 *   +0x30 datum_delete               = 0x00000000
 *   +0x34 datum_update               = 0x837EA108 -> garbage_update
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
 *   +0x80 part_definitions           = 0x84176F90841772B08417750800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

extern object_type_definition item_data_definition;
extern object_type_definition object_data_definition;
extern uint8_t garbage_new(int garbage_index);
extern uint8_t garbage_update(int garbage_index);

object_type_definition garbage_data_definition = {
    .name = "garbage",
    .group_tag = 'garb',
    .game_datum_size = 580,
    .placement_tag_block_offset = -1,
    .palette_tag_block_offset = -1,
    .placement_tag_block_element_size = -1,
    .update_message_type = -1,
    .datum_new = &garbage_new,
    .datum_update = &garbage_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &item_data_definition,
    .part_definitions[2] = &garbage_data_definition,
};
