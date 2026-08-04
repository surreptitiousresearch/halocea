#include <stdint.h>
#include "headers/blam_data_globals.h"
/* ui_loading_set_map @0x83764480 — set the friendly map name shown on the loading screen (cleared when the
 * map is null). */

/* param 2 is a wide buffer (DB: unsigned __int16*; loading_screen_map is wchar_t[64]) — not char*. */
extern void map_list_get_friendly_level_name(const char *map_name, uint16_t *name_buf, int name_buf_len);

void ui_loading_set_map(const char *map)
{
    if ( map )
        map_list_get_friendly_level_name(map, loading_screen_map, 64);
    else
        loading_screen_map[0] = 0;
}
