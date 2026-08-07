/* map_list_find_map @0x8376724C — find the multiplayer map list index whose name contains the given map name
 * (case-insensitive substring match). Returns -1 if not found. */

#include <string.h>
#include "headers/multiplayer_map_s.h"
#include "headers/blam_data_globals.h"


/* strlwr declared by <string.h> */
/* strstr declared by <string.h> */

int map_list_find_map(const char *map_name)
{
    char lowered[320];
    strncpy(lowered, map_name, 0x103u);
    lowered[259] = 0;
    strlwr(lowered);

    if ( multiplayer_map_list_count <= 0 )
        return -1;
    int i;
    for ( i = 0; !strstr(multiplayer_maps[i].name, lowered); ++i )
    {
        if ( i + 1 >= multiplayer_map_list_count )
            return -1;
    }
    return i;
}
