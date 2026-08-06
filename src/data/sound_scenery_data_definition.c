/* sound_scenery_data_definition @ 0x84177760 (.data, 196 bytes)
 * DB applied_types: object_type_definition sound_scenery_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821145F4 -> "sound_scenery"
 *   +0x04 group_tag                  = 0x73736365
 *   +0x08 game_datum_size            = 0x01F8
 *   +0x0A placement_tag_block_offset = 0x02DC
 *   +0x0C palette_tag_block_offset   = 0x02E8
 *   +0x0E placement_tag_block_element_size = 0x0028
 *   +0x10 update_message_type        = 0xFFFFFFFF
 *   +0x14 initialize                 = 0x00000000
 *   +0x18 dispose                    = 0x00000000
 *   +0x1C initialize_for_new_map     = 0x00000000
 *   +0x20 dispose_from_old_map       = 0x00000000
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x837E9CF8 -> sound_scenery_new
 *   +0x2C datum_place                = 0x00000000
 *   +0x30 datum_delete               = 0x837E9D30 -> sound_scenery_delete
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
 *   +0x80 part_definitions           = 0x84176F90841777600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

extern object_type_definition object_data_definition;
extern void sound_scenery_delete(int sound_scenery_index);
extern uint8_t sound_scenery_new(int object_index);

object_type_definition sound_scenery_data_definition = {
    .name = "sound_scenery",
    .group_tag = 'ssce',
    .game_datum_size = 504,
    .placement_tag_block_offset = 732,
    .palette_tag_block_offset = 744,
    .placement_tag_block_element_size = 40,
    .update_message_type = -1,
    .datum_new = &sound_scenery_new,
    .datum_delete = &sound_scenery_delete,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &sound_scenery_data_definition,
};
