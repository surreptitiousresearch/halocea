/* equipment_data_definition @ 0x84177440 -- second-order .data; object_type_definition reconstructed from binary bytes. */
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
