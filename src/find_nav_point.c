/* find_nav_point @0x837ECE78 — looks up a HUD waypoint-arrow definition index by name (case-insensitive)
 * within hud_globals->waypoint.arrows (104-byte entries, name string at each entry's start). Returns -1 if
 * hud_globals isn't loaded, the list is empty, or no entry matches. */

#include "headers/hud_globals.h"
#include "headers/hud_waypoint_arrow.h"

extern int stricmp(const char *a, const char *b);

int find_nav_point(const char *name)
{
    if ( !hud_globals )
        return -1;

    if ( hud_globals->waypoint.arrows.count <= 0 )
        return -1;

    for ( int i = 0; i < hud_globals->waypoint.arrows.count; ++i )
    {
        const char *entry_name = ((const hud_waypoint_arrow *)hud_globals->waypoint.arrows.address)[i].name;
        if ( !stricmp(name, entry_name) )
            return i;
    }

    return -1;
}
