/* scenario_object_name_index_from_string @0x837038C0 — linear search of scenario.object_names (36-byte
 * elements, name at offset 0) for an exact match; returns the element index or -1.
 *
 * Deviation: the decompiler inlines the name compare as a byte loop; reproduced with strcmp. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_object_name.h"

extern int strcmp(const char *a, const char *b);

int16_t scenario_object_name_index_from_string(scenario *scenario, const char *name)
{
    int count = scenario->object_names.count;
    if ( count <= 0 )
        return -1;

    /* the folded 36 was sizeof(scenario_object_name), whose `name` is at offset 0 */
    const scenario_object_name *object_names =
        (const scenario_object_name *)scenario->object_names.address;
    for ( int16_t i = 0; i < count; i = (int16_t)(i + 1) )
        if ( !strcmp(object_names[i].name, name) )
            return i;
    return -1;
}
