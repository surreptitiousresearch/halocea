/* hs_find_script_by_name @ 0x8372EDE8 — linear-search the scenario's hs script table for a
 * script whose name matches; returns its index or -1. Each hs_scripts entry is a 92-byte hs_script
 * whose first field is its name. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_script.h"

int16_t hs_find_script_by_name(const char *name)
{
    int count;
    int i;

    if ( global_scenario_index == -1 )
        return -1;

    count = global_scenario->hs_scripts.count;
    if ( count <= 0 )
        return -1;

    for ( i = 0; i < count; i = (int16_t)(i + 1) )
    {
        const unsigned char *entry_name = (const unsigned char *)((hs_script *)global_scenario->hs_scripts.address)[i].name;
        const char *p = name;
        int diff;
        do
        {
            diff = (unsigned char)*p - *entry_name;
            if ( !*p )
                break;
            ++p;
            ++entry_name;
        }
        while ( !diff );
        if ( !diff )
            return i;
    }
    return -1;
}
