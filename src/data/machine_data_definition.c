/* machine_data_definition @ 0x841778F0 -- second-order .data; object_type_definition reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/object_type_definition.h"

#include "headers/scenario_machine_datum.h"
extern object_type_definition device_data_definition;
extern object_type_definition object_data_definition;
extern void machine_delete(int machine_index);
extern uint8_t machine_new(int machine_index);
extern void machine_place(int machine_index, scenario_machine_datum *scenario_machine);
extern unsigned __int8 machine_update(int machine_index);
extern void machines_dispose(void);
extern void machines_dispose_from_old_map(void);
extern void machines_initialize(void);
extern void machines_initialize_for_new_map(void);

object_type_definition machine_data_definition = {
    .name = "machine",
    .group_tag = 'mach',
    .game_datum_size = 552,
    .placement_tag_block_offset = 660,
    .palette_tag_block_offset = 672,
    .placement_tag_block_element_size = 64,
    .update_message_type = -1,
    .initialize = &machines_initialize,
    .dispose = &machines_dispose,
    .initialize_for_new_map = &machines_initialize_for_new_map,
    .dispose_from_old_map = &machines_dispose_from_old_map,
    .datum_new = &machine_new,
    .datum_place = &machine_place,
    .datum_delete = &machine_delete,
    .datum_update = &machine_update,
    .part_definitions[0] = &object_data_definition,
    .part_definitions[1] = &device_data_definition,
    .part_definitions[2] = &machine_data_definition,
};
