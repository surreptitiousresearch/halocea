/* control_data_definition @ 0x841779B8 (.data, 196 bytes)
 * DB applied_types: object_type_definition control_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8204C5D8 -> "control"
 *   +0x04 group_tag                  = 0x6374726C
 *   +0x08 game_datum_size            = 0x021C
 *   +0x0A placement_tag_block_offset = 0x02AC
 *   +0x0C palette_tag_block_offset   = 0x02B8
 *   +0x0E placement_tag_block_element_size = 0x0040
 *   +0x10 update_message_type        = 0xFFFFFFFF
 *   +0x14 initialize                 = 0x837E9A58 -> controls_initialize
 *   +0x18 dispose                    = 0x837E9A60 -> controls_dispose
 *   +0x1C initialize_for_new_map     = 0x837E9A68 -> controls_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x837E9A70 -> controls_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x837E9B18 -> control_new
 *   +0x2C datum_place                = 0x837E9A78 -> control_place
 *   +0x30 datum_delete               = 0x837E9B20 -> control_delete
 *   +0x34 datum_update               = 0x837E9B28 -> control_update
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
 *   +0x80 part_definitions           = 0x84176F9084177828841779B800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/scenario_control_datum.h"
extern object_type_definition device_data_definition;
extern object_type_definition object_data_definition;
extern void control_delete(int control_index);
extern uint8_t control_new(int control_index);
extern void control_place(int control_index, scenario_control_datum *scenario_control);
extern uint8_t control_update(int control_index);
extern void controls_dispose(void);
extern void controls_dispose_from_old_map(void);
extern void controls_initialize(void);
extern void controls_initialize_for_new_map(void);

object_type_definition control_data_definition = {
    .name = "control",
    .group_tag = 'ctrl',
    .game_datum_size = 540,
    .placement_tag_block_offset = 684,
    .palette_tag_block_offset = 696,
    .placement_tag_block_element_size = 64,
    .update_message_type = -1,
    .initialize = &controls_initialize,
    .dispose = &controls_dispose,
    .initialize_for_new_map = &controls_initialize_for_new_map,
    .dispose_from_old_map = &controls_dispose_from_old_map,
    .datum_new = &control_new,
    .datum_place = &control_place,
    .datum_delete = &control_delete,
    .datum_update = &control_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &device_data_definition,
    .part_definitions[2] = &control_data_definition,
};
