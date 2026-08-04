/* sound_scenery_data_definition @ 0x84177760 -- second-order .data; object_type_definition reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

extern object_type_definition object_data_definition;
extern void sound_scenery_delete(int sound_scenery_index);
extern uint8_t sound_scenery_new(int object_index);

object_type_definition sound_scenery_data_definition = {
    .name = "sound_scenery",
    .group_tag = 'ssce',
    .game_datum_size = 504,
    .placement_tag_block_offset = 732,
    .palette_tag_block_offset = 744,
    .placement_tag_block_element_size = 40,
    .update_message_type = -1,
    .datum_new = &sound_scenery_new,
    .datum_delete = &sound_scenery_delete,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &sound_scenery_data_definition,
};
