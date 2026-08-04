/* unit_data_definition @ 0x84177058 -- third-order .data (data_closure.py); reconstructed from binary bytes. */
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
