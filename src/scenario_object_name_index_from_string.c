/* scenario_object_name_index_from_string @0x837038C0 — linear search of scenario.object_names (36-byte
 * elements, name at offset 0) for an exact match; returns the element index or -1.
 *
 * Deviation: the decompiler inlines the name compare as a byte loop; reproduced with strcmp. */

#include <stdint.h>
#include "headers/scenario.h"

extern int strcmp(const char *a, const char *b);

int16_t scenario_object_name_index_from_string(scenario *scenario, const char *name)
{
    int count = scenario->object_names.count;
    if ( count <= 0 )
        return -1;

    const char *address = (const char *)scenario->object_names.address;
    for ( __int16 i = 0; i < count; i = (__int16)(i + 1) )
        if ( !strcmp(&address[36 * i], name) )
            return i;
    return -1;
}
