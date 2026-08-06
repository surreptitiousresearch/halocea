/* projectile_data_definition @ 0x841775D0 (.data, 196 bytes)
 * DB applied_types: object_type_definition projectile_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211460C -> "projectile"
 *   +0x04 group_tag                  = 0x70726F6A
 *   +0x08 game_datum_size            = 0x02B0
 *   +0x0A placement_tag_block_offset = 0xFFFF
 *   +0x0C palette_tag_block_offset   = 0xFFFF
 *   +0x0E placement_tag_block_element_size = 0xFFFF
 *   +0x10 update_message_type        = 0x00000001
 *   +0x14 initialize                 = 0x83759078 -> projectiles_initialize
 *   +0x18 dispose                    = 0x83759090 -> projectiles_dispose
 *   +0x1C initialize_for_new_map     = 0x83759080 -> projectiles_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x83759088 -> projectiles_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x8375B040 -> projectile_new
 *   +0x2C datum_place                = 0x00000000
 *   +0x30 datum_delete               = 0x83759128 -> projectile_delete
 *   +0x34 datum_update               = 0x8375BEF0 -> projectile_update
 *   +0x38 datum_export_function_values = 0x83759D10 -> projectile_export_function_values
 *   +0x3C handle_deleted_object      = 0x837597B0 -> projectile_handle_deleted_object
 *   +0x40 handle_region_destroyed    = 0x00000000
 *   +0x44 handle_parent_destroyed    = 0x8375A538 -> projectile_handle_parent_destroyed
 *   +0x48 datum_preprocess_node_orientations = 0x00000000
 *   +0x4C datum_postprocess_node_matrices = 0x00000000
 *   +0x50 reset                      = 0x00000000
 *   +0x54 disconnect_from_structure_bsp = 0x00000000
 *   +0x58 notify_impulse_sound       = 0x00000000
 *   +0x5C render_debug               = 0x00000000
 *   +0x60 should_render_debug_object_network_visualization = 0x8375A5A0 -> projectile_should_render_debug_object_network_visualization
 *   +0x64 object_new_to_network      = 0x8375A5A8 -> projectile_new_to_network
 *   +0x68 update_baseline            = 0x8375A930 -> projectile_update_baseline
 *   +0x6C build_update_delta         = 0x8375A9B0 -> projectile_build_update_delta
 *   +0x70 process_update_delta       = 0x8375AAC0 -> projectile_process_update_delta
 *   +0x74 should_try_sending_incremental = 0x8375ACC0 -> projectile_should_try_sending_incremental
 *   +0x78 should_send_incremental_to_player = 0x8375ACC8 -> projectile_should_send_incremental_to_player
 *   +0x7C incremental_sent           = 0x8375ACD0 -> projectile_incremental_sent
 *   +0x80 part_definitions           = 0x84176F90841775D00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
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
