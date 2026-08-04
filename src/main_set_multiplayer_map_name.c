/* main_set_multiplayer_map_name @0x83688F58 — set the multiplayer map name, update the loading-screen map
 * label, and begin precaching that map's cache file. */

#include <stdint.h>
#include "headers/main_globals.h"

extern char *strncpy(char *, const char *, unsigned int);
extern void ui_loading_set_map(const char *map);
extern int cache_files_give_time_to_precache(const char *map_name, uint8_t fatal);

void main_set_multiplayer_map_name(const char *name)
{
    strncpy(main_globals.multiplayer_map_name, name, 0xFFu);
    main_globals.multiplayer_map_name[255] = 0;
    ui_loading_set_map(name);
    cache_files_give_time_to_precache(main_globals.multiplayer_map_name, 0);
}
