/* item_data_definition @ 0x841772B0 -- third-order .data (data_closure.py); reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

extern object_type_definition object_data_definition;
extern void items_initialize(void);
extern void items_dispose(void);
extern void items_initialize_for_new_map(void);
extern void items_dispose_from_old_map(void);
extern uint8_t item_new(int item_index);
extern void item_delete(int item_index);
extern uint8_t item_update(int item_index);

object_type_definition item_data_definition = {
    .name = "item",
    .group_tag = 'item',
    .game_datum_size = 556,          /* 0x022C (high half of +0x08) */
    .placement_tag_block_offset = -1,        /* 0xFFFF NONE */
    .palette_tag_block_offset = -1,          /* 0xFFFF NONE */
    .placement_tag_block_element_size = -1,  /* 0xFFFF NONE */
    .update_message_type = -1,               /* 0xFFFFFFFF NONE */
    .initialize = &items_initialize,
    .dispose = &items_dispose,
    .initialize_for_new_map = &items_initialize_for_new_map,
    .dispose_from_old_map = &items_dispose_from_old_map,
    .datum_new = &item_new,
    .datum_delete = &item_delete,
    .datum_update = &item_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &item_data_definition,
};
