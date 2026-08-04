/* scenery_data_definition @ 0x84177698 -- second-order .data; object_type_definition reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/scenario_scenery_datum.h"
extern object_type_definition object_data_definition;
extern void scenery_delete(int scenery_index);
extern void scenery_dispose(void);
extern void scenery_dispose_from_old_map(void);
extern void scenery_initialize(void);
extern void scenery_initialize_for_new_map(void);
extern uint8_t scenery_new(int object_index);
extern void scenery_place(int scenery_index, scenario_scenery_datum *scenario_scenery);
extern unsigned __int8 scenery_update(int scenery_index);

object_type_definition scenery_data_definition = {
    .name = "scenery",
    .group_tag = 'scen',
    .game_datum_size = 504,
    .placement_tag_block_offset = 528,
    .palette_tag_block_offset = 540,
    .placement_tag_block_element_size = 72,
    .update_message_type = -1,
    .initialize = &scenery_initialize,
    .dispose = &scenery_dispose,
    .initialize_for_new_map = &scenery_initialize_for_new_map,
    .dispose_from_old_map = &scenery_dispose_from_old_map,
    .datum_new = &scenery_new,
    .datum_place = &scenery_place,
    .datum_delete = &scenery_delete,
    .datum_update = &scenery_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &scenery_data_definition,
};
