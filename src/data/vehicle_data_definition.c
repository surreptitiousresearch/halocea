/* vehicle_data_definition @ 0x841771E8 -- second-order .data; object_type_definition reconstructed from binary bytes. */
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
