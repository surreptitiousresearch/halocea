/* item_data_definition @ 0x841772B0 (.data, 196 bytes)
 * DB applied_types: object_type_definition item_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x820C5904 -> "item"
 *   +0x04 group_tag                  = 0x6974656D
 *   +0x08 game_datum_size            = 0x022C
 *   +0x0A placement_tag_block_offset = 0xFFFF
 *   +0x0C palette_tag_block_offset   = 0xFFFF
 *   +0x0E placement_tag_block_element_size = 0xFFFF
 *   +0x10 update_message_type        = 0xFFFFFFFF
 *   +0x14 initialize                 = 0x83757BB0 -> items_initialize
 *   +0x18 dispose                    = 0x83757BC8 -> items_dispose
 *   +0x1C initialize_for_new_map     = 0x83757BB8 -> items_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x83757BC0 -> items_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x83757BD0 -> item_new
 *   +0x2C datum_place                = 0x00000000
 *   +0x30 datum_delete               = 0x83757C38 -> item_delete
 *   +0x34 datum_update               = 0x83758680 -> item_update
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
 *   +0x80 part_definitions           = 0x84176F90841772B00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * third-order .data (data_closure.py); reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

extern object_type_definition object_data_definition;
extern void items_initialize(void);
extern void items_dispose(void);
extern void items_initialize_for_new_map(void);
extern void items_dispose_from_old_map(void);
extern uint8_t item_new(int item_index);
extern void item_delete(int item_index);
extern uint8_t item_update(int item_index);

object_type_definition item_data_definition = {
    .name = "item",
    .group_tag = 'item',
    .game_datum_size = 556,          /* 0x022C (high half of +0x08) */
    .placement_tag_block_offset = -1,        /* 0xFFFF NONE */
    .palette_tag_block_offset = -1,          /* 0xFFFF NONE */
    .placement_tag_block_element_size = -1,  /* 0xFFFF NONE */
    .update_message_type = -1,               /* 0xFFFFFFFF NONE */
    .initialize = &items_initialize,
    .dispose = &items_dispose,
    .initialize_for_new_map = &items_initialize_for_new_map,
    .dispose_from_old_map = &items_dispose_from_old_map,
    .datum_new = &item_new,
    .datum_delete = &item_delete,
    .datum_update = &item_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &item_data_definition,
};
