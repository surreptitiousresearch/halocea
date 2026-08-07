/* map_list_should_ignore @0x837673D8 — true if the map's filename (lowercased, no directory/extension)
 * exactly matches one of the built-in `ignore_maps` names (single-player/UI/shared-asset "maps" that
 * should never appear in a multiplayer map list), or if it's found by substring in the runtime
 * multiplayer_maps list (mirrors map_list_find_map.c). */

#include <string.h>
#include "headers/multiplayer_map_s.h"
#include "headers/blam_data_globals.h"


extern void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);
/* stricmp declared by <string.h> */
/* strncpy declared by <string.h> */
/* strstr declared by <string.h> */

BOOL map_list_should_ignore(const char *map_path)
{
    char file_name[272];
    _splitpath(map_path, nullptr, nullptr, file_name, nullptr);
    strlwr(file_name);

    for ( int i = 0; i < 13; i++ )
    {
        if ( !stricmp(file_name, ignore_maps[i]) )
            return 1;
    }

    char lowered[320];
    strncpy(lowered, file_name, 0x103u);
    lowered[259] = 0;
    strlwr(lowered);

    if ( multiplayer_map_list_count <= 0 )
        return 0;

    int i;
    for ( i = 0; !strstr(multiplayer_maps[i].name, lowered); ++i )
    {
        if ( i + 1 >= multiplayer_map_list_count )
            return 0;
    }
    return 1;
}
