/* vehicle_data_definition @ 0x841771E8 (.data, 196 bytes)
 * DB applied_types: object_type_definition vehicle_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211462C -> "vehicle"
 *   +0x04 group_tag                  = 0x76656869
 *   +0x08 game_datum_size            = 0x05C0
 *   +0x0A placement_tag_block_offset = 0x0240
 *   +0x0C palette_tag_block_offset   = 0x024C
 *   +0x0E placement_tag_block_element_size = 0x0078
 *   +0x10 update_message_type        = 0x00000005
 *   +0x14 initialize                 = 0x8375CCD8 -> vehicles_initialize
 *   +0x18 dispose                    = 0x8375CCF0 -> vehicles_dispose
 *   +0x1C initialize_for_new_map     = 0x8375CCE0 -> vehicles_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x8375CCE8 -> vehicles_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x8375CDD0 -> vehicle_new
 *   +0x2C datum_place                = 0x8375CCF8 -> vehicle_place
 *   +0x30 datum_delete               = 0x8375CF30 -> vehicle_delete
 *   +0x34 datum_update               = 0x83761F70 -> vehicle_update
 *   +0x38 datum_export_function_values = 0x83760620 -> vehicle_export_function_values
 *   +0x3C handle_deleted_object      = 0x00000000
 *   +0x40 handle_region_destroyed    = 0x00000000
 *   +0x44 handle_parent_destroyed    = 0x00000000
 *   +0x48 datum_preprocess_node_orientations = 0x8375D388 -> vehicle_preprocess_node_orientations
 *   +0x4C datum_postprocess_node_matrices = 0x00000000
 *   +0x50 reset                      = 0x8375CD40 -> vehicle_reset
 *   +0x54 disconnect_from_structure_bsp = 0x00000000
 *   +0x58 notify_impulse_sound       = 0x00000000
 *   +0x5C render_debug               = 0x8375DA38 -> vehicle_render_debug
 *   +0x60 should_render_debug_object_network_visualization = 0x8375DA30 -> vehicle_should_render_debug_object_network_visualization
 *   +0x64 object_new_to_network      = 0x8375DBC8 -> vehicle_new_to_network
 *   +0x68 update_baseline            = 0x8375E028 -> vehicle_update_baseline
 *   +0x6C build_update_delta         = 0x8375E0E8 -> vehicle_build_update_delta
 *   +0x70 process_update_delta       = 0x8375E288 -> vehicle_process_update_delta
 *   +0x74 should_try_sending_incremental = 0x8375E5F8 -> vehicle_should_try_sending_incremental
 *   +0x78 should_send_incremental_to_player = 0x8375E600 -> vehicle_should_send_incremental_to_player
 *   +0x7C incremental_sent           = 0x8375E608 -> vehicle_incremental_sent
 *   +0x80 part_definitions           = 0x84176F9084177058841771E800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/real_orientation.h"
#include "headers/scenario_vehicle_datum.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
extern object_type_definition object_data_definition;
extern object_type_definition unit_data_definition;
extern int vehicle_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits, const message_delta_processor_mode mode);
extern void vehicle_delete(int vehicle_index);
extern void vehicle_export_function_values(int vehicle_index);
extern void vehicle_incremental_sent(int object_index);
extern uint8_t vehicle_new(int vehicle_index);
extern int vehicle_new_to_network(int object_index, void *buffer, int buffer_size_in_bits);
extern void vehicle_place(int vehicle_index, scenario_vehicle_datum *scenario_vehicle);
extern void vehicle_preprocess_node_orientations(int vehicle_index, real_orientation *node_orientations);
extern void vehicle_process_update_delta(int object_index, message_delta_processor_header *header, network_game_client *client);
extern void vehicle_render_debug(int vehicle_index);
extern void vehicle_reset(int vehicle_index);
extern uint8_t vehicle_should_render_debug_object_network_visualization(int vehicle_index);
extern uint8_t vehicle_should_send_incremental_to_player(int object_index, int player_index);
extern uint8_t vehicle_should_try_sending_incremental(int object_index);
extern uint8_t vehicle_update(int vehicle_index);
extern void vehicle_update_baseline(int object_index);
extern void vehicles_dispose(void);
extern void vehicles_dispose_from_old_map(void);
extern void vehicles_initialize(void);
extern void vehicles_initialize_for_new_map(void);

object_type_definition vehicle_data_definition = {
    .name = "vehicle",
    .group_tag = 'vehi',
    .game_datum_size = 1472,
    .placement_tag_block_offset = 576,
    .palette_tag_block_offset = 588,
    .placement_tag_block_element_size = 120,
    .update_message_type = 5,
    .initialize = &vehicles_initialize,
    .dispose = &vehicles_dispose,
    .initialize_for_new_map = &vehicles_initialize_for_new_map,
    .dispose_from_old_map = &vehicles_dispose_from_old_map,
    .datum_new = &vehicle_new,
    .datum_place = &vehicle_place,
    .datum_delete = &vehicle_delete,
    .datum_update = &vehicle_update,
    .datum_export_function_values = &vehicle_export_function_values,
    .datum_preprocess_node_orientations = &vehicle_preprocess_node_orientations,
    .reset = &vehicle_reset,
    .render_debug = &vehicle_render_debug,
    .should_render_debug_object_network_visualization = &vehicle_should_render_debug_object_network_visualization,
    .object_new_to_network = &vehicle_new_to_network,
    .update_baseline = &vehicle_update_baseline,
    .build_update_delta = &vehicle_build_update_delta,
    .process_update_delta = &vehicle_process_update_delta,
    .should_try_sending_incremental = &vehicle_should_try_sending_incremental,
    .should_send_incremental_to_player = &vehicle_should_send_incremental_to_player,
    .incremental_sent = &vehicle_incremental_sent,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &unit_data_definition,
    .part_definitions[2] = &vehicle_data_definition,
};
