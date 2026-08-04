/* biped_data_definition @ 0x84177120 -- second-order .data; object_type_definition reconstructed from binary bytes. */
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
