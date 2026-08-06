/* biped_data_definition @ 0x84177120 (.data, 196 bytes)
 * DB applied_types: object_type_definition biped_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82114634 -> "biped"
 *   +0x04 group_tag                  = 0x62697064
 *   +0x08 game_datum_size            = 0x0550
 *   +0x0A placement_tag_block_offset = 0x0228
 *   +0x0C palette_tag_block_offset   = 0x0234
 *   +0x0E placement_tag_block_element_size = 0x0078
 *   +0x10 update_message_type        = 0x00000004
 *   +0x14 initialize                 = 0x837AC358 -> bipeds_initialize
 *   +0x18 dispose                    = 0x837AC370 -> bipeds_dispose
 *   +0x1C initialize_for_new_map     = 0x837AC360 -> bipeds_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x837AC368 -> bipeds_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x837AC3C0 -> biped_adjust_placement
 *   +0x28 datum_new                  = 0x837B0C38 -> biped_new
 *   +0x2C datum_place                = 0x837AC378 -> biped_place
 *   +0x30 datum_delete               = 0x837AC458 -> biped_delete
 *   +0x34 datum_update               = 0x837B4028 -> biped_update
 *   +0x38 datum_export_function_values = 0x837AC670 -> biped_export_function_values
 *   +0x3C handle_deleted_object      = 0x00000000
 *   +0x40 handle_region_destroyed    = 0x00000000
 *   +0x44 handle_parent_destroyed    = 0x00000000
 *   +0x48 datum_preprocess_node_orientations = 0x837AC758 -> biped_preprocess_node_orientations
 *   +0x4C datum_postprocess_node_matrices = 0x00000000
 *   +0x50 reset                      = 0x837AC760 -> biped_reset
 *   +0x54 disconnect_from_structure_bsp = 0x837AC7E8 -> biped_disconnect_from_structure_bsp
 *   +0x58 notify_impulse_sound       = 0x00000000
 *   +0x5C render_debug               = 0x837AD1C0 -> biped_render_debug
 *   +0x60 should_render_debug_object_network_visualization = 0x837AD1B8 -> biped_should_render_debug_object_network_visualization
 *   +0x64 object_new_to_network      = 0x837AD820 -> biped_new_to_network
 *   +0x68 update_baseline            = 0x837ADC40 -> biped_update_baseline
 *   +0x6C build_update_delta         = 0x837ADCC8 -> biped_build_update_delta
 *   +0x70 process_update_delta       = 0x837ADE18 -> biped_process_update_delta
 *   +0x74 should_try_sending_incremental = 0x837ADFC0 -> biped_should_try_sending_incremental
 *   +0x78 should_send_incremental_to_player = 0x837ADFC8 -> biped_should_send_incremental_to_player
 *   +0x7C incremental_sent           = 0x837ADFD0 -> biped_incremental_sent
 *   +0x80 part_definitions           = 0x84176F90841770588417712000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/object_placement_data.h"
#include "headers/real_orientation.h"
#include "headers/scenario_biped_datum.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
extern object_type_definition object_data_definition;
extern object_type_definition unit_data_definition;
extern void biped_adjust_placement(int object_index, object_placement_data *data);
extern int biped_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits, const message_delta_processor_mode mode);
extern void biped_delete(int biped_index);
extern void biped_disconnect_from_structure_bsp(int biped_index);
extern void biped_export_function_values(int biped_index);
extern void biped_incremental_sent(int object_index);
extern uint8_t biped_new(int biped_index);
extern int biped_new_to_network(int object_index, void *buffer, int buffer_size_in_bits);
extern void biped_place(int biped_index, scenario_biped_datum *scenario_biped);
extern void biped_preprocess_node_orientations(int biped_index, real_orientation *node_orientations);
extern void biped_process_update_delta(int object_index, message_delta_processor_header *header, network_game_client *client);
extern void biped_render_debug(int biped_index);
extern void biped_reset(int biped_index);
extern uint8_t biped_should_render_debug_object_network_visualization(int biped_index);
extern uint8_t biped_should_send_incremental_to_player(int object_index, int player_index);
extern uint8_t biped_should_try_sending_incremental(int object_index);
extern uint8_t biped_update(int biped_index);
extern void biped_update_baseline(int object_index);
extern void bipeds_dispose(void);
extern void bipeds_dispose_from_old_map(void);
extern void bipeds_initialize(void);
extern void bipeds_initialize_for_new_map(void);

object_type_definition biped_data_definition = {
    .name = "biped",
    .group_tag = 'bipd',
    .game_datum_size = 1360,
    .placement_tag_block_offset = 552,
    .palette_tag_block_offset = 564,
    .placement_tag_block_element_size = 120,
    .update_message_type = 4,
    .initialize = &bipeds_initialize,
    .dispose = &bipeds_dispose,
    .initialize_for_new_map = &bipeds_initialize_for_new_map,
    .dispose_from_old_map = &bipeds_dispose_from_old_map,
    .datum_adjust_placement = &biped_adjust_placement,
    .datum_new = &biped_new,
    .datum_place = &biped_place,
    .datum_delete = &biped_delete,
    .datum_update = &biped_update,
    .datum_export_function_values = &biped_export_function_values,
    .datum_preprocess_node_orientations = &biped_preprocess_node_orientations,
    .reset = &biped_reset,
    .disconnect_from_structure_bsp = &biped_disconnect_from_structure_bsp,
    .render_debug = &biped_render_debug,
    .should_render_debug_object_network_visualization = &biped_should_render_debug_object_network_visualization,
    .object_new_to_network = &biped_new_to_network,
    .update_baseline = &biped_update_baseline,
    .build_update_delta = &biped_build_update_delta,
    .process_update_delta = &biped_process_update_delta,
    .should_try_sending_incremental = &biped_should_try_sending_incremental,
    .should_send_incremental_to_player = &biped_should_send_incremental_to_player,
    .incremental_sent = &biped_incremental_sent,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &unit_data_definition,
    .part_definitions[2] = &biped_data_definition,
};
