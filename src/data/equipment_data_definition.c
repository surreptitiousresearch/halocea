/* equipment_data_definition @ 0x84177440 (.data, 196 bytes)
 * DB applied_types: object_type_definition equipment_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82114620 -> "equipment"
 *   +0x04 group_tag                  = 0x65716970
 *   +0x08 game_datum_size            = 0x0294
 *   +0x0A placement_tag_block_offset = 0x0258
 *   +0x0C palette_tag_block_offset   = 0x0264
 *   +0x0E placement_tag_block_element_size = 0x0028
 *   +0x10 update_message_type        = 0x00000002
 *   +0x14 initialize                 = 0x00000000
 *   +0x18 dispose                    = 0x00000000
 *   +0x1C initialize_for_new_map     = 0x00000000
 *   +0x20 dispose_from_old_map       = 0x00000000
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x837B5D50 -> equipment_new
 *   +0x2C datum_place                = 0x837B5DC8 -> equipment_place
 *   +0x30 datum_delete               = 0x00000000
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
 *   +0x60 should_render_debug_object_network_visualization = 0x837B5EF8 -> equipment_should_render_debug_object_network_visualization
 *   +0x64 object_new_to_network      = 0x837B6678 -> equipment_new_to_network
 *   +0x68 update_baseline            = 0x837B62A8 -> equipment_update_baseline
 *   +0x6C build_update_delta         = 0x837B6330 -> equipment_build_update_delta
 *   +0x70 process_update_delta       = 0x837B6458 -> equipment_process_update_delta
 *   +0x74 should_try_sending_incremental = 0x837B6660 -> equipment_should_try_sending_incremental
 *   +0x78 should_send_incremental_to_player = 0x837B6668 -> equipment_should_send_incremental_to_player
 *   +0x7C incremental_sent           = 0x837B6670 -> equipment_incremental_sent
 *   +0x80 part_definitions           = 0x84176F90841772B08417744000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/scenario_equipment_datum.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
extern object_type_definition item_data_definition;
extern object_type_definition object_data_definition;
extern int equipment_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits, const message_delta_processor_mode mode);
extern void equipment_incremental_sent(int object_index);
extern uint8_t equipment_new(int equipment_index);
extern int equipment_new_to_network(int object_index, void *buffer, int buffer_size_in_bits);
extern void equipment_place(int equipment_index, scenario_equipment_datum *scenario_equipment);
extern void equipment_process_update_delta(int object_index, message_delta_processor_header *header, network_game_client *client);
extern uint8_t equipment_should_render_debug_object_network_visualization(int equipment_index);
extern uint8_t equipment_should_send_incremental_to_player(int object_index, int player_index);
extern uint8_t equipment_should_try_sending_incremental(int object_index);
extern void equipment_update_baseline(int object_index);

object_type_definition equipment_data_definition = {
    .name = "equipment",
    .group_tag = 'eqip',
    .game_datum_size = 660,
    .placement_tag_block_offset = 600,
    .palette_tag_block_offset = 612,
    .placement_tag_block_element_size = 40,
    .update_message_type = 2,
    .datum_new = &equipment_new,
    .datum_place = &equipment_place,
    .should_render_debug_object_network_visualization = &equipment_should_render_debug_object_network_visualization,
    .object_new_to_network = &equipment_new_to_network,
    .update_baseline = &equipment_update_baseline,
    .build_update_delta = &equipment_build_update_delta,
    .process_update_delta = &equipment_process_update_delta,
    .should_try_sending_incremental = &equipment_should_try_sending_incremental,
    .should_send_incremental_to_player = &equipment_should_send_incremental_to_player,
    .incremental_sent = &equipment_incremental_sent,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &item_data_definition,
    .part_definitions[2] = &equipment_data_definition,
};
