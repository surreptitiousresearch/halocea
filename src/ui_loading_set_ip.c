#include "headers/blam_data_globals.h"
/* ui_loading_set_ip @0x83764458 — widens `ip` into the loading screen's connect-IP display buffer, or clears
 * it when `ip` is null. */

extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);

void ui_loading_set_ip(char *ip)
{
    if (ip)
        ascii_to_wide(ip, loading_screen_connect_ip, 0x40u);
    else
        loading_screen_connect_ip[0] = 0;
}
