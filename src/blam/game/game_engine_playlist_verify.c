/* game_engine_playlist_verify @0x83752430 — validate a map/variant selection: the map's leaf name (after
 * any backslash) must be in the map list, the cache file must exist, and — if a variant name is given — it
 * must resolve to a known game variant. Returns nonzero when valid.
 * DEVIATION: game_engine_get_variant_by_name returns a found/not-found result here (other call sites use it
 * as void); declared returning int for this use. */

#include <stdint.h>

#include "headers/game_variant.h"
#include "headers/game_variant.h"
extern char *strrchr(const char *string, int character);
extern int map_list_find_map(const char *map_name);
extern uint8_t cache_file_map_exists(const char *name);
extern uint8_t game_engine_get_variant_by_name(const char *name, game_variant *variant);

uint8_t game_engine_playlist_verify(const char *map_name, const char *variant_name)
{
    char *backslash = strrchr(map_name, '\\');
    const char *leaf_name = backslash ? backslash + 1 : map_name;

    int result = map_list_find_map(leaf_name) != -1;
    if ( (unsigned char)result )
        result = cache_file_map_exists(map_name);
    if ( (unsigned char)result )
    {
        if ( variant_name )
            return game_engine_get_variant_by_name(variant_name, 0);
    }
    return result;
}
