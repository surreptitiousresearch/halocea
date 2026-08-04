/* weapon_data_definition @ 0x84177378 -- second-order .data; object_type_definition reconstructed from binary bytes. */
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
