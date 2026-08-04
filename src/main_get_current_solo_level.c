/* main_get_current_solo_level @0x836896D0 — map the current solo map name back to its solo level index. */

#include <stdint.h>
#include "headers/main_globals.h"

extern int16_t main_get_solo_level_from_name(const char *name);

int16_t main_get_current_solo_level(void)
{
    return main_get_solo_level_from_name(main_globals.soloplayer_map_name);
}
