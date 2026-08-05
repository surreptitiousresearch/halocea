/* control_data_definition @ 0x841779B8 -- second-order .data; object_type_definition reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/scenario_control_datum.h"
extern object_type_definition device_data_definition;
extern object_type_definition object_data_definition;
extern void control_delete(int control_index);
extern uint8_t control_new(int control_index);
extern void control_place(int control_index, scenario_control_datum *scenario_control);
extern uint8_t control_update(int control_index);
extern void controls_dispose(void);
extern void controls_dispose_from_old_map(void);
extern void controls_initialize(void);
extern void controls_initialize_for_new_map(void);

object_type_definition control_data_definition = {
    .name = "control",
    .group_tag = 'ctrl',
    .game_datum_size = 540,
    .placement_tag_block_offset = 684,
    .palette_tag_block_offset = 696,
    .placement_tag_block_element_size = 64,
    .update_message_type = -1,
    .initialize = &controls_initialize,
    .dispose = &controls_dispose,
    .initialize_for_new_map = &controls_initialize_for_new_map,
    .dispose_from_old_map = &controls_dispose_from_old_map,
    .datum_new = &control_new,
    .datum_place = &control_place,
    .datum_delete = &control_delete,
    .datum_update = &control_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &device_data_definition,
    .part_definitions[2] = &control_data_definition,
};
