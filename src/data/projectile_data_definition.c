/* projectile_data_definition @ 0x841775D0 -- second-order .data; object_type_definition reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
extern object_type_definition object_data_definition;
extern int projectile_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits, const message_delta_processor_mode mode);
extern void projectile_delete(int projectile_index);
extern void projectile_export_function_values(int projectile_index);
extern void projectile_handle_deleted_object(int projectile_index, int deleted_object_index);
extern uint8_t projectile_handle_parent_destroyed(int projectile_index);
extern void projectile_incremental_sent(int object_index);
extern uint8_t projectile_new(int projectile_index);
extern int projectile_new_to_network(int object_index, void *buffer, int buffer_size_in_bits);
extern void projectile_process_update_delta(int object_index, message_delta_processor_header *header, network_game_client *client);
extern uint8_t projectile_should_render_debug_object_network_visualization(int projectile_index);
extern uint8_t projectile_should_send_incremental_to_player(int object_index, int player_index);
extern uint8_t projectile_should_try_sending_incremental(int object_index);
extern uint8_t projectile_update(int projectile_index);
extern void projectile_update_baseline(int object_index);
extern void projectiles_dispose(void);
extern void projectiles_dispose_from_old_map(void);
extern void projectiles_initialize(void);
extern void projectiles_initialize_for_new_map(void);

object_type_definition projectile_data_definition = {
    .name = "projectile",
    .group_tag = 'proj',
    .game_datum_size = 688,
    .placement_tag_block_offset = -1,
    .palette_tag_block_offset = -1,
    .placement_tag_block_element_size = -1,
    .update_message_type = 1,
    .initialize = &projectiles_initialize,
    .dispose = &projectiles_dispose,
    .initialize_for_new_map = &projectiles_initialize_for_new_map,
    .dispose_from_old_map = &projectiles_dispose_from_old_map,
    .datum_new = &projectile_new,
    .datum_delete = &projectile_delete,
    .datum_update = &projectile_update,
    .datum_export_function_values = &projectile_export_function_values,
    .handle_deleted_object = &projectile_handle_deleted_object,
    .handle_parent_destroyed = &projectile_handle_parent_destroyed,
    .should_render_debug_object_network_visualization = &projectile_should_render_debug_object_network_visualization,
    .object_new_to_network = &projectile_new_to_network,
    .update_baseline = &projectile_update_baseline,
    .build_update_delta = &projectile_build_update_delta,
    .process_update_delta = &projectile_process_update_delta,
    .should_try_sending_incremental = &projectile_should_try_sending_incremental,
    .should_send_incremental_to_player = &projectile_should_send_incremental_to_player,
    .incremental_sent = &projectile_incremental_sent,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &projectile_data_definition,
};
