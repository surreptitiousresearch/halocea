/* map_list_initialize_known_list @ 0x837674F0 — build the runtime multiplayer
 * map list from the static builtin table, growing the array as needed and
 * copying each (lowercased) map name onto the heap. The decompiler inlined
 * strlen/strcpy as byte loops; reproduced inline here. */

#include "headers/multiplayer_map_s.h"
#include "headers/blam_data_globals.h"

/* DEVIATION: the decompiler rendered the loop bound as the ADDRESS OF s_file_name_0
 * (0x8417EEC8) — the binary's own `addi r10, r23, (s_file_name_0 - 0x8417EDE0)` @0x837675F8,
 * i.e. &builtin_multiplayer_maps[19].map_name (0x8417EDE0 + 19*12 + 4), which lands on an
 * unrelated global. An address pun; re-expressed here as the array bound. */

extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);
extern char *strlwr(char *string);

void map_list_initialize_known_list(void)
{
    buildin_multiplayer_map_s *src;

    for ( src = builtin_multiplayer_maps; src < &builtin_multiplayer_maps[19]; ++src )
    {
        const char *name = src->map_name;
        int pic_index = src->map_index;
        int idx = multiplayer_map_list_count;
        const char *p;
        unsigned int name_len;
        char *dst;
        const char *q;

        if ( idx >= multiplayer_map_list_size )
        {
            multiplayer_map_list_size += 19;
            multiplayer_maps = dlRealloc(
                multiplayer_maps, 8 * multiplayer_map_list_size,
                "D:\\Projects\\code\\HCEX\\sources\\interface\\map_list.c", 0xB0);
            idx = multiplayer_map_list_count;
        }

        multiplayer_maps[idx].name = 0;
        multiplayer_maps[idx].pic_name_index = pic_index;

        for ( p = name; *p; ++p )
            ;
        name_len = (unsigned int)(p - name);

        multiplayer_maps[idx].name = dlRealloc(
            multiplayer_maps[idx].name, name_len + 1,
            "D:\\Projects\\code\\HCEX\\sources\\interface\\map_list.c", 0xB7);

        dst = multiplayer_maps[idx].name;
        q = name;
        do
            *dst++ = *q;
        while ( *q++ );
        multiplayer_maps[idx].name[name_len] = 0;
        strlwr(multiplayer_maps[idx].name);

        multiplayer_map_list_count = idx + 1;
    }
}
