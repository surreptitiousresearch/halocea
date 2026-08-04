/* cache_file_is_open @0x83753920 — whether a .map cache file is currently open.
 * DEVIATION: the decompiler rendered this as an arithmetic identity over open_map_file_index; it reduces
 * to (open_map_file_index != -1). */

#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"


int cache_file_is_open(void)
{
    return cache_file_globals_0.open_map_file_index != -1;
}
