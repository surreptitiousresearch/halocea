/* light_fixture_data_definition @ 0x84177A80 -- second-order .data; object_type_definition reconstructed from binary bytes. */
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
