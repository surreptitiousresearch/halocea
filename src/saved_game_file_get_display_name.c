#include <stdint.h>
#include "headers/blam_data_globals.h"
/* saved_game_file_get_display_name @0x83726B00 — stub in this release: returns a shared, emptied display-name
 * buffer (always the empty string). */


uint16_t *saved_game_file_get_display_name(int index)
{
    display_name[0] = 0;
    return display_name;
}
