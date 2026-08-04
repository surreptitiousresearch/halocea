/* hs_object_iterate_names_containing @0x837F7FD0 — walks the scenario's object-name table and invokes
 * the supplied iterator with the index of every object name whose text contains name_string as a
 * substring. Object-name entries are 36 bytes each. */

#include "headers/scenario.h"
#include "headers/scenario_object_name.h"
#include "headers/blam_data_globals.h"

extern char *strstr(const char *haystack, const char *needle);

void hs_object_iterate_names_containing(const char *name_string, void (*iterator)(__int16))
{
    scenario *scenario_definition = global_scenario;
    int name_index = 0;
    for (int i = 0; i < scenario_definition->object_names.count; ++i)
    {
        if (strstr(((scenario_object_name *)scenario_definition->object_names.address)[i].name, name_string))
            iterator((__int16)name_index);
        name_index = (__int16)(i + 1);
    }
}
