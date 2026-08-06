/* hs_find_global_by_name @0x8372EF70 — look up a HaloScript global by name. External (engine) globals are
 * searched first and return their index with the HS_GLOBAL_EXTERNAL_FLAG bit set; scenario-defined globals
 * return a plain index. Returns -1 if not found. Scenario global entries are 92 bytes (name at offset 0).
 *
 * Deviation: 92-byte scenario hs_globals stride is a raw offset (the entry's name is its first field). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_global_designator.h"
#include "headers/hs_global_definition.h"
#include "headers/hs_global_internal.h"
#include "headers/blam_data_globals.h"

extern int stricmp(const char *a, const char *b);
extern const int16_t hs_external_global_count;

int16_t hs_find_global_by_name(const char *name)
{
    if ( hs_external_global_count > 0 )
    {
        for ( int i = 0; i < hs_external_global_count; i = (int16_t)(i + 1) )
        {
            if ( !stricmp(name, hs_external_globals[i]->name) )
                return (int16_t)MAKE_EXTERNAL_GLOBAL_DESIGNATOR(i);
        }
    }

    if ( global_scenario_index == -1 )
        return -1;

    scenario *scn = global_scenario;
    if ( scn->hs_globals.count <= 0 )
        return -1;

    for ( int i = 0; i < scn->hs_globals.count; i = (int16_t)(i + 1) )
    {
        if ( !stricmp(name, ((hs_global_internal *)scn->hs_globals.address)[i].name) )
            return HS_GLOBAL_DESIGNATOR_TO_INDEX(i);
    }
    return -1;
}
