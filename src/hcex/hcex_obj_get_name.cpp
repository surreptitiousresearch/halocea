/* hcex_obj_get_name @0x83683718 — return the scenario object-name string for a live Blam object id, or
 * NULL when the object has no name assigned (name_index == -1). Looks the object datum up directly in
 * object_header_data (the salted object-header index idiom used throughout action_*.c) rather than
 * going through object_try_and_get_and_verify_type, then indexes global_scenario->object_names by the
 * datum's name_index.
 *
 * Distinct from hcex_obj_name: that function returns the object's MODEL tag path
 * (object_definition.model.name); this one returns the scenario-editor object name
 * (scenario.object_names[name_index].name). Not duplicates. */

#include <stdint.h>
#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/object_datum.h"
#include "../headers/scenario.h"
#include "../headers/scenario_object_name.h"

extern data_array *object_header_data;
extern scenario *global_scenario;

extern "C" const char *hcex_obj_get_name(int id)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;
    int16_t name_index = object->object.name_index;

    if ( name_index == -1 )
        return 0;

    return ((scenario_object_name *)global_scenario->object_names.address)[name_index].name;
}
