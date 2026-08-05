/* hs_object_iterate_names_containing @0x837F7FD0 — walks the scenario's object-name table and invokes
 * the supplied iterator with the index of every object name whose text contains name_string as a
 * substring. Object-name entries are 36 bytes each. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_object_name.h"
#include "headers/blam_data_globals.h"

extern char *strstr(const char *haystack, const char *needle);

void hs_object_iterate_names_containing(const char *name_string, void (*iterator)(int16_t))
{
    scenario *scenario_definition = global_scenario;
    int name_index = 0;
    for (int i = 0; i < scenario_definition->object_names.count; ++i)
    {
        if (strstr(((scenario_object_name *)scenario_definition->object_names.address)[i].name, name_string))
            iterator((int16_t)name_index);
        name_index = (int16_t)(i + 1);
    }
}
