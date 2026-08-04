/* placeholder_data_definition @ 0x84177B48 -- second-order .data; object_type_definition reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/scenario_placeholder_datum.h"
extern object_type_definition object_data_definition;
extern void placeholder_delete(int placeholder_index);
extern void placeholder_dispose(void);
extern void placeholder_dispose_from_old_map(void);
extern void placeholder_initialize(void);
extern void placeholder_initialize_for_new_map(void);
extern uint8_t placeholder_new(int object_index);
extern void placeholder_place(int placeholder_index, scenario_placeholder_datum *scenario_placeholder);

object_type_definition placeholder_data_definition = {
    .name = "placeholder",
    .group_tag = 'plac',
    .game_datum_size = 508,
    .placement_tag_block_offset = -1,
    .palette_tag_block_offset = -1,
    .placement_tag_block_element_size = -1,
    .update_message_type = -1,
    .initialize = &placeholder_initialize,
    .dispose = &placeholder_dispose,
    .initialize_for_new_map = &placeholder_initialize_for_new_map,
    .dispose_from_old_map = &placeholder_dispose_from_old_map,
    .datum_new = &placeholder_new,
    .datum_place = &placeholder_place,
    .datum_delete = &placeholder_delete,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &placeholder_data_definition,
};
