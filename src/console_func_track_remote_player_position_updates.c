#include "headers/blam_data_globals.h"
/* console_func_track_remote_player_position_updates @0x8376DBB0 — record the name of the remote player whose
 * network position updates should be traced, converting the ASCII console argument to the wide-string debug
 * buffer (up to 0x800 chars). */

extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);

void console_func_track_remote_player_position_updates(char *remote_player_name)
{
    ascii_to_wide(remote_player_name, g_tracked_remote_player_position_updates, 0x800u);
}
