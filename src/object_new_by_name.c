/* object_new_by_name @0x836F2CC4 — instantiate the scenario object referred to by a scenario object
 * name. The name entry (object_names block, stride 36) carries the object type (word +32) and the
 * scenario-datum index (word +34); the object is created from that scenario datum and its type palette.
 * global_object_scenario_index is set to (datum index + 1) during creation, as the engine expects. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_object_datum.h"
#include "headers/scenario_object_name.h"
#include "headers/tag_block.h"
#include "headers/blam_data_globals.h"

#include "headers/scenario.h"
extern tag_block *scenario_get_object_type_scenario_datums(scenario *scenario, short object_type, int *size);
extern tag_block * scenario_get_object_type_scenario_palette(scenario *scenario, int16_t object_type);
extern char *tag_block_get_element_with_size(const tag_block *block, int index, int element_size);
extern int object_new_from_scenario(scenario_object_datum *scenario_object, tag_block *palette);

int object_new_by_name(short name_index)
{
    scenario_object_name *name_entry = (scenario_object_name *)global_scenario->object_names.address + name_index;
    short object_type = name_entry->runtime_object_type;
    short scenario_datum_index = name_entry->runtime_scenario_datum_index;

    int element_size;
    tag_block *scenario_datums = scenario_get_object_type_scenario_datums(global_scenario, object_type, &element_size);
    tag_block *scenario_palette = scenario_get_object_type_scenario_palette(global_scenario, object_type);
    scenario_object_datum *scenario_object =
        (scenario_object_datum *)tag_block_get_element_with_size(scenario_datums, scenario_datum_index, element_size);

    global_object_scenario_index = scenario_datum_index + 1;
    int result = object_new_from_scenario(scenario_object, scenario_palette);
    global_object_scenario_index = 0;
    return result;
}
