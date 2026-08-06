/* weapon_data_definition @ 0x84177378 (.data, 196 bytes)
 * DB applied_types: object_type_definition weapon_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8202387C -> "weapon"
 *   +0x04 group_tag                  = 0x77656170
 *   +0x08 game_datum_size            = 0x0340
 *   +0x0A placement_tag_block_offset = 0x0270
 *   +0x0C palette_tag_block_offset   = 0x027C
 *   +0x0E placement_tag_block_element_size = 0x005C
 *   +0x10 update_message_type        = 0x00000003
 *   +0x14 initialize                 = 0x836D84B0 -> weapons_initialize
 *   +0x18 dispose                    = 0x836D84C8 -> weapons_dispose
 *   +0x1C initialize_for_new_map     = 0x836D84B8 -> weapons_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x836D84C0 -> weapons_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x836D85D0 -> weapon_new
 *   +0x2C datum_place                = 0x836D84D0 -> weapon_place
 *   +0x30 datum_delete               = 0x836D8740 -> weapon_delete
 *   +0x34 datum_update               = 0x836DD328 -> weapon_update
 *   +0x38 datum_export_function_values = 0x836DAD30 -> weapon_export_function_values
 *   +0x3C handle_deleted_object      = 0x00000000
 *   +0x40 handle_region_destroyed    = 0x00000000
 *   +0x44 handle_parent_destroyed    = 0x00000000
 *   +0x48 datum_preprocess_node_orientations = 0x836D8748 -> weapon_preprocess_node_orientations
 *   +0x4C datum_postprocess_node_matrices = 0x00000000
 *   +0x50 reset                      = 0x00000000
 *   +0x54 disconnect_from_structure_bsp = 0x00000000
 *   +0x58 notify_impulse_sound       = 0x00000000
 *   +0x5C render_debug               = 0x00000000
 *   +0x60 should_render_debug_object_network_visualization = 0x836DA590 -> weapon_should_render_debug_object_network_visualization
 *   +0x64 object_new_to_network      = 0x836DBD20 -> weapon_new_to_network
 *   +0x68 update_baseline            = 0x836DA960 -> weapon_update_baseline
 *   +0x6C build_update_delta         = 0x836DA9E8 -> weapon_build_update_delta
 *   +0x70 process_update_delta       = 0x836DAB10 -> weapon_process_update_delta
 *   +0x74 should_try_sending_incremental = 0x836DAD18 -> weapon_should_try_sending_incremental
 *   +0x78 should_send_incremental_to_player = 0x836DAD20 -> weapon_should_send_incremental_to_player
 *   +0x7C incremental_sent           = 0x836DAD28 -> weapon_incremental_sent
 *   +0x80 part_definitions           = 0x84176F90841772B08417737800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/real_orientation.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/scenario_weapon_datum.h"
extern object_type_definition item_data_definition;
extern object_type_definition object_data_definition;
extern int weapon_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits, const message_delta_processor_mode mode);
extern void weapon_delete(int weapon_index);
extern void weapon_export_function_values(int weapon_index);
extern void weapon_incremental_sent(int object_index);
extern uint8_t weapon_new(int weapon_index);
extern int weapon_new_to_network(int object_index, void *buffer, int buffer_size_in_bits);
extern void weapon_place(int weapon_index, scenario_weapon_datum *scenario_weapon);
extern void weapon_preprocess_node_orientations(int weapon_index, real_orientation *node_orientations);
extern void weapon_process_update_delta(int object_index, message_delta_processor_header *header, network_game_client *client);
extern uint8_t weapon_should_render_debug_object_network_visualization(int weapon_index);
extern uint8_t weapon_should_send_incremental_to_player(int object_index, int player_index);
extern uint8_t weapon_should_try_sending_incremental(int object_index);
extern uint8_t weapon_update(int weapon_index);
extern void weapon_update_baseline(int object_index);
extern void weapons_dispose(void);
extern void weapons_dispose_from_old_map(void);
extern void weapons_initialize(void);
extern void weapons_initialize_for_new_map(void);

object_type_definition weapon_data_definition = {
    .name = "weapon",
    .group_tag = 'weap',
    .game_datum_size = 832,
    .placement_tag_block_offset = 624,
    .palette_tag_block_offset = 636,
    .placement_tag_block_element_size = 92,
    .update_message_type = 3,
    .initialize = &weapons_initialize,
    .dispose = &weapons_dispose,
    .initialize_for_new_map = &weapons_initialize_for_new_map,
    .dispose_from_old_map = &weapons_dispose_from_old_map,
    .datum_new = &weapon_new,
    .datum_place = &weapon_place,
    .datum_delete = &weapon_delete,
    .datum_update = &weapon_update,
    .datum_export_function_values = &weapon_export_function_values,
    .datum_preprocess_node_orientations = &weapon_preprocess_node_orientations,
    .should_render_debug_object_network_visualization = &weapon_should_render_debug_object_network_visualization,
    .object_new_to_network = &weapon_new_to_network,
    .update_baseline = &weapon_update_baseline,
    .build_update_delta = &weapon_build_update_delta,
    .process_update_delta = &weapon_process_update_delta,
    .should_try_sending_incremental = &weapon_should_try_sending_incremental,
    .should_send_incremental_to_player = &weapon_should_send_incremental_to_player,
    .incremental_sent = &weapon_incremental_sent,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &item_data_definition,
    .part_definitions[2] = &weapon_data_definition,
};
