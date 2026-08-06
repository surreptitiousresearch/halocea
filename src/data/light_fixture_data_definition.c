/* light_fixture_data_definition @ 0x84177A80 (.data, 196 bytes)
 * DB applied_types: object_type_definition light_fixture_data_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821145DC -> "light_fixture"
 *   +0x04 group_tag                  = 0x6C696669
 *   +0x08 game_datum_size            = 0x022C
 *   +0x0A placement_tag_block_offset = 0x02C4
 *   +0x0C palette_tag_block_offset   = 0x02D0
 *   +0x0E placement_tag_block_element_size = 0x0058
 *   +0x10 update_message_type        = 0xFFFFFFFF
 *   +0x14 initialize                 = 0x837E9990 -> light_fixtures_initialize
 *   +0x18 dispose                    = 0x837E9998 -> light_fixtures_dispose
 *   +0x1C initialize_for_new_map     = 0x837E99A0 -> light_fixtures_initialize_for_new_map
 *   +0x20 dispose_from_old_map       = 0x837E99A8 -> light_fixtures_dispose_from_old_map
 *   +0x24 datum_adjust_placement     = 0x00000000
 *   +0x28 datum_new                  = 0x837E9A40 -> light_fixture_new
 *   +0x2C datum_place                = 0x837E99B0 -> light_fixture_place
 *   +0x30 datum_delete               = 0x837E9A48 -> light_fixture_delete
 *   +0x34 datum_update               = 0x837E9A50 -> light_fixture_update
 *   +0x38 datum_export_function_values = 0x00000000
 *   +0x3C handle_deleted_object      = 0x00000000
 *   +0x40 handle_region_destroyed    = 0x00000000
 *   +0x44 handle_parent_destroyed    = 0x00000000
 *   +0x48 datum_preprocess_node_orientations = 0x00000000
 *   +0x4C datum_postprocess_node_matrices = 0x00000000
 *   +0x50 reset                      = 0x00000000
 *   +0x54 disconnect_from_structure_bsp = 0x00000000
 *   +0x58 notify_impulse_sound       = 0x00000000
 *   +0x5C render_debug               = 0x00000000
 *   +0x60 should_render_debug_object_network_visualization = 0x00000000
 *   +0x64 object_new_to_network      = 0x00000000
 *   +0x68 update_baseline            = 0x00000000
 *   +0x6C build_update_delta         = 0x00000000
 *   +0x70 process_update_delta       = 0x00000000
 *   +0x74 should_try_sending_incremental = 0x00000000
 *   +0x78 should_send_incremental_to_player = 0x00000000
 *   +0x7C incremental_sent           = 0x00000000
 *   +0x80 part_definitions           = 0x84176F908417782884177A8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xC0 next                       = 0x00000000
 * second-order .data; object_type_definition reconstructed from binary bytes.
 */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/scenario_light_fixture_datum.h"
extern object_type_definition device_data_definition;
extern object_type_definition object_data_definition;
extern void light_fixture_delete(int light_fixture_index);
extern uint8_t light_fixture_new(int light_fixture_index);
extern void light_fixture_place(int light_fixture_index, scenario_light_fixture_datum *scenario_light_fixture);
extern uint8_t light_fixture_update(int light_fixture_index);
extern void light_fixtures_dispose(void);
extern void light_fixtures_dispose_from_old_map(void);
extern void light_fixtures_initialize(void);
extern void light_fixtures_initialize_for_new_map(void);

object_type_definition light_fixture_data_definition = {
    .name = "light_fixture",
    .group_tag = 'lifi',
    .game_datum_size = 556,
    .placement_tag_block_offset = 708,
    .palette_tag_block_offset = 720,
    .placement_tag_block_element_size = 88,
    .update_message_type = -1,
    .initialize = &light_fixtures_initialize,
    .dispose = &light_fixtures_dispose,
    .initialize_for_new_map = &light_fixtures_initialize_for_new_map,
    .dispose_from_old_map = &light_fixtures_dispose_from_old_map,
    .datum_new = &light_fixture_new,
    .datum_place = &light_fixture_place,
    .datum_delete = &light_fixture_delete,
    .datum_update = &light_fixture_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &device_data_definition,
    .part_definitions[2] = &light_fixture_data_definition,
};
