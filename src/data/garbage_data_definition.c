/* garbage_data_definition @ 0x84177508 -- second-order .data; object_type_definition reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

extern object_type_definition item_data_definition;
extern object_type_definition object_data_definition;
extern uint8_t garbage_new(int garbage_index);
extern uint8_t garbage_update(int garbage_index);

object_type_definition garbage_data_definition = {
    .name = "garbage",
    .group_tag = 'garb',
    .game_datum_size = 580,
    .placement_tag_block_offset = -1,
    .palette_tag_block_offset = -1,
    .placement_tag_block_element_size = -1,
    .update_message_type = -1,
    .datum_new = &garbage_new,
    .datum_update = &garbage_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &item_data_definition,
    .part_definitions[2] = &garbage_data_definition,
};
