/* unit_data_definition @ 0x84177058 (.data, 196 bytes)
 * DB applied_types: object_type_definition unit_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211463C -> "unit"
 *   +0x04 group_tag                  = 0x756E6974
 *   +0x08 game_datum_size            = 0x04CC
 *   +0x0A placement_tag_block_offset = 0xFFFF
 *   +0x0C palette_tag_block_offset   = 0xFFFF
 *   +0x0E placement_tag_block_element_size = 0xFFFF
 *   +0x10 update_message_type        = 0xFFFFFFFF
 *   +0x14 initialize                 = 0x836C6B40 -> units_initialize
 *   +0x18 dispose                    = 0x836C6B98 -> units_dispose
 *   +0x1C initialize_for_new_map     = 0x836C6B78 -> units_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x836C6B90 -> units_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x836D3F18 -> unit_new
 *   +0x2C datum_place                = 0x00000000
 *   +0x30 datum_delete               = 0x836C6C48 -> unit_delete
 *   +0x34 datum_update               = 0x836D4298 -> unit_update
 *   +0x38 datum_export_function_values = 0x836C6D10 -> unit_export_function_values
 *   +0x3C handle_deleted_object      = 0x836CFEA8 -> unit_handle_deleted_object
 *   +0x40 handle_region_destroyed    = 0x836CCA70 -> unit_handle_region_destroyed
 *   +0x44 handle_parent_destroyed    = 0x00000000
 *   +0x48 datum_preprocess_node_orientations = 0x836CD898 -> unit_preprocess_node_orientations
 *   +0x4C datum_postprocess_node_matrices = 0x836D0F78 -> unit_postprocess_node_matrices
 *   +0x50 reset                      = 0x00000000
 *   +0x54 disconnect_from_structure_bsp = 0x00000000
 *   +0x58 notify_impulse_sound       = 0x837BACD0 -> unit_notify_impulse_sound
 *   +0x5C render_debug               = 0x836C7EF0 -> unit_render_debug
 *   +0x60 should_render_debug_object_network_visualization = 0x00000000
 *   +0x64 object_new_to_network      = 0x00000000
 *   +0x68 update_baseline            = 0x00000000
 *   +0x6C build_update_delta         = 0x00000000
 *   +0x70 process_update_delta       = 0x00000000
 *   +0x74 should_try_sending_incremental = 0x00000000
 *   +0x78 should_send_incremental_to_player = 0x00000000
 *   +0x7C incremental_sent           = 0x00000000
 *   +0x80 part_definitions           = 0x84176F90841770580000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * third-order .data (data_closure.py); reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/real_orientation.h"
#include "headers/real_matrix4x3.h"
struct unit_globals;

extern object_type_definition object_data_definition;
extern void units_initialize(void);
extern void units_dispose(void);
extern void units_initialize_for_new_map(void);
extern void units_dispose_from_old_map(void);
extern uint8_t unit_new(int unit_index);
extern void unit_delete(int unit_index);
extern uint8_t unit_update(int unit_index);
extern void unit_export_function_values(int unit_index);
extern void unit_handle_deleted_object(int unit_index, int deleted_object_index);
extern void unit_handle_region_destroyed(int unit_index, int16_t region_index, unsigned int region_damage_flags);
extern void unit_preprocess_node_orientations(int object_index, real_orientation *node_orientations);
extern void unit_postprocess_node_matrices(int object_index, real_matrix4x3 *node_matrices);
extern void unit_notify_impulse_sound(int unit_index, int sound_definition_index, int impulse_sound_index);
extern void unit_render_debug(int unit_index);

object_type_definition unit_data_definition = {
    .name = "unit",
    .group_tag = 'unit',
    .game_datum_size = 1228,         /* 0x04CC (high half of +0x08) */
    .placement_tag_block_offset = -1,        /* 0xFFFF NONE */
    .palette_tag_block_offset = -1,          /* 0xFFFF NONE */
    .placement_tag_block_element_size = -1,  /* 0xFFFF NONE */
    .update_message_type = -1,               /* 0xFFFFFFFF NONE */
    .initialize = &units_initialize,
    .dispose = &units_dispose,
    .initialize_for_new_map = &units_initialize_for_new_map,
    .dispose_from_old_map = &units_dispose_from_old_map,
    .datum_new = &unit_new,
    .datum_delete = &unit_delete,
    .datum_update = &unit_update,
    .datum_export_function_values = &unit_export_function_values,
    .handle_deleted_object = &unit_handle_deleted_object,
    .handle_region_destroyed = &unit_handle_region_destroyed,
    .datum_preprocess_node_orientations = &unit_preprocess_node_orientations,
    .datum_postprocess_node_matrices = &unit_postprocess_node_matrices,
    .notify_impulse_sound = &unit_notify_impulse_sound,
    .render_debug = &unit_render_debug,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &unit_data_definition,
};
