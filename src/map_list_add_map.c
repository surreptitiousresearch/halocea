/* map_list_add_map @0x837672D0 — append a caller-supplied (map_name, pic_name_index) pair to the runtime
 * multiplayer map list, growing the array as needed and copying the (lowercased) map name onto the heap.
 * Sibling of map_list_initialize_known_list.c (which does the same for the builtin table); the decompiler
 * inlined strlen/strcpy as byte loops, reproduced inline here to match. */

#include "headers/multiplayer_map_s.h"
#include "headers/blam_data_globals.h"


extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);
extern char *strlwr(char *string);

void map_list_add_map(char *map_name, int index)
{
    int idx = multiplayer_map_list_count;

    if ( idx >= multiplayer_map_list_size )
    {
        multiplayer_map_list_size += 19;
        multiplayer_maps = dlRealloc(multiplayer_maps, 8 * multiplayer_map_list_size,
            "D:\\Projects\\code\\HCEX\\sources\\interface\\map_list.c", 0xB0u);
        idx = multiplayer_map_list_count;
    }

    multiplayer_maps[idx].name = 0;
    multiplayer_maps[idx].pic_name_index = index;

    const char *p;
    for ( p = map_name; *p; ++p )
        ;
    unsigned int name_len = (unsigned int)(p - map_name);

    multiplayer_maps[idx].name = dlRealloc(multiplayer_maps[idx].name, name_len + 1,
        "D:\\Projects\\code\\HCEX\\sources\\interface\\map_list.c", 0xB7u);

    char *dst = multiplayer_maps[idx].name;
    const char *q = map_name;
    do
        *dst++ = *q;
    while ( *q++ );
    multiplayer_maps[idx].name[name_len] = 0;
    strlwr(multiplayer_maps[idx].name);

    ++multiplayer_map_list_count;
}
