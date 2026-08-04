/* object_data_definition @ 0x84176F90 -- third-order .data (data_closure.py); reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

extern void object_export_function_values(int object_index);
extern void object_render_debug(int object_index);

object_type_definition object_data_definition = {
    .name = "object",
    .group_tag = 'obje',
    .game_datum_size = 500,          /* 0x01F4 (high half of +0x08) */
    .placement_tag_block_offset = -1,        /* 0xFFFF NONE */
    .palette_tag_block_offset = -1,          /* 0xFFFF NONE */
    .placement_tag_block_element_size = -1,  /* 0xFFFF NONE */
    .update_message_type = -1,               /* 0xFFFFFFFF NONE */
    .datum_export_function_values = &object_export_function_values,
    .render_debug = &object_render_debug,
    .part_definitions[0] = &object_data_definition,
};
