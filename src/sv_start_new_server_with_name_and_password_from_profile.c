/* sv_start_new_server_with_name_and_password_from_profile @0x83766D70 — start a dedicated server using the
 * profile's configured name/password. On this build the server-start path is stubbed: it only defaults an
 * unset server name to "Halo" and always returns 0 (not started). */

#include <stdint.h>
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"

extern void ustrcpy(wchar_t *dest, const wchar_t *src);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

uint8_t sv_start_new_server_with_name_and_password_from_profile(uint8_t force_begin_playlist)
{
    if ( !sv_name_value[0] )
    {
        ustrcpy(sv_name_value, L"Halo");
        ship_terminal_printf(0, "Server name unset, defaulting to %ls", sv_name_value);
    }
    return 0;
}
