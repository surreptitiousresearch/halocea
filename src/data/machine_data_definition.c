/* machine_data_definition @ 0x841778F0 (.data, 196 bytes)
 * DB applied_types: object_type_definition machine_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821145EC -> "machine"
 *   +0x04 group_tag                  = 0x6D616368
 *   +0x08 game_datum_size            = 0x0228
 *   +0x0A placement_tag_block_offset = 0x0294
 *   +0x0C palette_tag_block_offset   = 0x02A0
 *   +0x0E placement_tag_block_element_size = 0x0040
 *   +0x10 update_message_type        = 0xFFFFFFFF
 *   +0x14 initialize                 = 0x837BBE40 -> machines_initialize
 *   +0x18 dispose                    = 0x837BBE48 -> machines_dispose
 *   +0x1C initialize_for_new_map     = 0x837BBE50 -> machines_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x837BBE58 -> machines_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x837BBF30 -> machine_new
 *   +0x2C datum_place                = 0x837BBE60 -> machine_place
 *   +0x30 datum_delete               = 0x837BBFC0 -> machine_delete
 *   +0x34 datum_update               = 0x837BBFC8 -> machine_update
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
 *   +0x80 part_definitions           = 0x84176F9084177828841778F000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/scenario_machine_datum.h"
extern object_type_definition device_data_definition;
extern object_type_definition object_data_definition;
extern void machine_delete(int machine_index);
extern uint8_t machine_new(int machine_index);
extern void machine_place(int machine_index, scenario_machine_datum *scenario_machine);
extern uint8_t machine_update(int machine_index);
extern void machines_dispose(void);
extern void machines_dispose_from_old_map(void);
extern void machines_initialize(void);
extern void machines_initialize_for_new_map(void);

object_type_definition machine_data_definition = {
    .name = "machine",
    .group_tag = 'mach',
    .game_datum_size = 552,
    .placement_tag_block_offset = 660,
    .palette_tag_block_offset = 672,
    .placement_tag_block_element_size = 64,
    .update_message_type = -1,
    .initialize = &machines_initialize,
    .dispose = &machines_dispose,
    .initialize_for_new_map = &machines_initialize_for_new_map,
    .dispose_from_old_map = &machines_dispose_from_old_map,
    .datum_new = &machine_new,
    .datum_place = &machine_place,
    .datum_delete = &machine_delete,
    .datum_update = &machine_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &device_data_definition,
    .part_definitions[2] = &machine_data_definition,
};
